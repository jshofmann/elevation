// Copyright (C) 2025 Azimuth Studios

#include "pch.h"

#include <d3d12.h>
#include <dxcapi.h>
#include <dxgidebug.h>

#if !defined( BUILD_RETAIL )
	#include <shlobj.h>
#endif

#include <dxerr/dxerr.h>

#include "dx12Device.h"

#include <ee/core/System.h>
#include <ee/io/FilePath.h>
#include <ee/utility/Config.h>
#include <ee/utility/StringUtils.h>

#include <drivers/windows/core/WinCheck.h>
#include <drivers/windows/core/WinApplication.h>
#include <drivers/windows/core/WinWindow.h>
#include <drivers/dx12/dxDebug.h>

#pragma comment( lib, "d3d12.lib" )
#pragma comment( lib, "dxgi.lib" )
#pragma comment( lib, "dxcompiler.lib" )
#pragma comment( lib, "dxguid.lib" )

using namespace ee;

// Level 0: Debug layer disabled (default)
// Level 1: CPU-side debug layer enabled: https://docs.microsoft.com/en-us/windows/win32/direct3d12/direct3d-12-sdklayers-reference
// Level 2: GPU-based validation: https://docs.microsoft.com/en-us/windows/win32/direct3d12/using-d3d12-debug-layer-gpu-based-validation
// Level 3: GPU-based validation with resource state validation
REGISTER_CONFIG_PROPERTY( "Device", "EnableDebugLayer", gGraphicsEnableDebugLayer, int, 0 )

// DRED: Device Removed Extended Data, see https://devblogs.microsoft.com/directx/dred
// If gGraphicsEnableDebugLayer > 0 then all DRED features are automatically enabled
REGISTER_CONFIG_PROPERTY( "Device", "EnableDRED", gGraphicsEnableDRED, bool, false )

// Enabling page fault reporting adds some system memory overhead and
// a small performance hit to object creation and destruction,
// so it's off by default
REGISTER_CONFIG_PROPERTY( "Device", "EnableDREDPageFaults", gGraphicsEnableDREDPageFaults, bool, false )

// Enabling auto-breadcrumbs incurs a 2-5% performance hit, so off by default
REGISTER_CONFIG_PROPERTY( "Device", "EnableDREDBreadcrumbs", gGraphicsEnableDREDBreadcrumbs, bool, false )

bool dx12Device::Initialize( void )
{
#if defined( BUILD_RETAIL )

	// Make sure gGraphicsEnableDebugLayer is 0 even if it's in the config file
	SET_CONFIG_PROPERTY( gGraphicsEnableDebugLayer, 0 );

	// We may want to revisit disabling DRED in Retail builds...
	SET_CONFIG_PROPERTY( gGraphicsEnableDRED, false );
	SET_CONFIG_PROPERTY( gGraphicsEnableDREDPageFaults, false );
	SET_CONFIG_PROPERTY( gGraphicsEnableDREDBreadcrumbs, false );

#else

	// Note: Enabling the debug layer slows down every D3D call!
	if( gGraphicsEnableDebugLayer > 0 )
	{
		eeDebug( "************************************************************************************" );
		eeDebug( "Note: The D3D debug layer is enabled, performance will be severely compromised!" );
		eeDebug( "************************************************************************************" );

		// Use of the D3D debug layer automatically enables use of DRED
		SET_CONFIG_PROPERTY( gGraphicsEnableDRED, true );
		SET_CONFIG_PROPERTY( gGraphicsEnableDREDPageFaults, true );
		SET_CONFIG_PROPERTY( gGraphicsEnableDREDBreadcrumbs, true );
	}

#endif // #if !defined( BUILD_RETAIL )

	return CreateDevice();
}

void dx12Device::Release( void )
{
	if( mPIXCapturerModule != NULL )
	{
		eeCheckBool( FreeLibrary( mPIXCapturerModule ) );
		mPIXCapturerModule = NULL;
	}

	mDXGIFactory.Reset();
	mAdapter.Reset();

	mD3D12Debug.Reset();
	mDevice.Reset();
	mDeviceThreadID = kInvalidThreadID;

	mDxcUtils.Reset();
}

bool dx12Device::CreateDevice( void )
{
	if( !eeCheck( EnableDebugLayer() ) )
	{
		return false;
	}

#if defined( BUILD_PC )

	#if defined( BUILD_PROFILE )

	// Enable connections to PIX if there isn't already a GPU debugger attached
	// and we haven't enabled the debug layers (a documented restriction).
	// This must be done before device creation.
	if( mAllowPIXAttach && !System::IsGPUDebuggerAttached() )
	{
		// It's non-fatal if this fails
		LoadPIXDLL();
	}

	#endif // #if defined( BUILD_PROFILE )

	if( !eeCheck( CreateDeviceFactory() ) )
	{
		return false;
	}

	// The user can override the best-GPU selection here, but note that it
	// does require knowledge of the order in which the GPUs are enumerated
	// (i.e. for debugging use only, pretty much, since the order is something
	//	that's considered to be a video driver implementation detail.)
	// 4294967295 == kInvalidGPUIndex (~0u aka 0xffffffff aka -1)
	// !!! TODO: does this logic belong here or at an application level?
	Config*	 config = Application::GetInstance().GetConfig();
	uint32_t gpuIndex =
		config != nullptr ? ToUInt( config->GetValue( "Device", "GPUIndex", "4294967295" ) ) : kInvalidGPUIndex;

	// First, check if the desired adapter is available
	HRESULT result = mDXGIFactory->EnumAdapterByGpuPreference( gpuIndex != kInvalidGPUIndex ? gpuIndex : 0,
															   DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
															   IID_PPV_ARGS( mAdapter.ReleaseAndGetAddressOf() ) );
	if( FAILED( result ) )
	{
		mAdapter = nullptr;

		// If D3D12Core.dll is missing or an unexpected version, then just bail;
		// iterating over all available adapters won't fix this problem
		if( result == D3D12_ERROR_INVALID_REDIST )
		{
			eeDebug(
				"dx12Device::CreateDevice: D3D12Core.dll is missing or an unexpected version" ); // !!! should log this
		}
		else
		{
			eeDebug(
				"dx12Device::CreateDevice: IDXGIFactory::EnumAdapterByGpuPreference() failed with error 0x%08x: %s",
				result, DXGetErrorString( result ) ); // !!! should log this
		}

		return false;
	}

	// If the requested adapter doesn't support the required D3D feature levle
	if( !eeCheck( VerifyMinimumFeatureLevel( mAdapter.Get(), mFeatureLevel ) ) )
	{
		mAdapter = nullptr;

		// Look for one that does. If there is more than one adapter supporting
		// the required feature level choose the one with the most memory.
		Microsoft::WRL::ComPtr< IDXGIAdapter1 > adapter;
		SIZE_T									maxMemorySize = 0;

		for( gpuIndex = 0; mDXGIFactory->EnumAdapterByGpuPreference(
							   gpuIndex, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
							   IID_PPV_ARGS( adapter.ReleaseAndGetAddressOf() ) ) != DXGI_ERROR_NOT_FOUND;
			 ++gpuIndex )
		{
			if( eeCheck( VerifyMinimumFeatureLevel( adapter.Get(), mFeatureLevel ) ) )
			{
				DXGI_ADAPTER_DESC1 desc;
				adapter->GetDesc1( &desc );

				if( desc.DedicatedVideoMemory < maxMemorySize )
					continue;

				maxMemorySize = desc.DedicatedVideoMemory;

				if( mAdapter != nullptr )
				{
					mAdapter->Release();
				}

				mAdapter = adapter.Detach();
			}

		} // for( each adapter )

	} // if( !eeCheck( VerifyMinimumFeatureLevel( adapter, mFeatureLevel ) ) )

	if( mAdapter == nullptr )
	{
		eeDebug( "dx12Device::CreateDevice: Unable to find a IDXGIAdapter supporting %s",
				 GetD3DFeatureLevelString( mFeatureLevel ) ); // !!! should log this
		return false;
	}

	// Now that we have an adapater, create the device for realsies this time
	if( !eeCheck( D3D12CreateDevice( mAdapter.Get(), mFeatureLevel, __uuidof( ID3D12Device ),
									 reinterpret_cast< LPVOID* >( mDevice.GetAddressOf() ) ) ) )
	{
		return false;
	}

	#if !defined( BUILD_RETAIL )

	bool developerModeEnabled = false;

	// Look in the Windows Registry to determine if Developer Mode is enabled
	HKEY	hKey;
	LSTATUS status = RegOpenKeyEx( HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\AppModelUnlock",
								   0, KEY_READ, &hKey );
	if( status == ERROR_SUCCESS )
	{
		DWORD keyValue, keySize = sizeof( DWORD );
		status = RegQueryValueEx( hKey, "AllowDevelopmentWithoutDevLicense", NULL, NULL,
								  reinterpret_cast< LPBYTE >( &keyValue ), &keySize );
		if( status == ERROR_SUCCESS && keyValue == 1 )
		{
			developerModeEnabled = true;
		}

		RegCloseKey( hKey );
	}

	eeDebugIf( !developerModeEnabled,
			   "dx12Device::CreateDevice: Enable Developer Mode on Windows 10 to get consistent profiling results" );

	// Prevent the GPU from overclocking or underclocking to get consistent timings
	if( developerModeEnabled )
		mDevice->SetStablePowerState( TRUE );

	#endif // #if !defined( BUILD_RETAIL )

#endif // #if defined( BUILD_PC )

	// Shader reflection requires access to the IDxcUtils interface;
	// this is unrelated to the D3D12 device, but dx12Device is not a bad place
	// to stash this pointer, at least for now....
	if( !eeCheck( DxcCreateInstance( CLSID_DxcUtils, IID_PPV_ARGS( &mDxcUtils ) ) ) )
	{
		return false;
	}

	return true;
}

HRESULT dx12Device::EnableDebugLayer( void )
{
	if( gGraphicsEnableDebugLayer > 0 )
	{
		// When the debug layer is requested, this is the first D3D call made;
		// if D3D12Core.dll is missing or an unexpected version, bail immediately
		// as all future calls will fail with the same error result and we want
		// to immediately report this rather than having this error be swamped
		// and overwritten by the subsequent error cascade.
		HRESULT result = D3D12GetDebugInterface( IID_PPV_ARGS( mD3D12Debug.GetAddressOf() ) );
		if( result == D3D12_ERROR_INVALID_REDIST )
		{
			return result;
		}

		if( SUCCEEDED( result ) )
		{
			mD3D12Debug->EnableDebugLayer();
		}
		else
		{
			eeDebug( "dx12Device::EnableDebugLayer: D3D12GetDebugInterface( ID3D12Debug ) failed with error 0x%08x: %s",
					 result, DXGetErrorString( result ) ); // !!! should log this
			return result;
		}

		// At level 2 we enable GPU-based validation,
		// with its extra performance hit
		if( gGraphicsEnableDebugLayer > 1 )
		{
			Microsoft::WRL::ComPtr< ID3D12Debug1 > d3d12Debug1;
			if( eeCheck( D3D12GetDebugInterface( IID_PPV_ARGS( d3d12Debug1.GetAddressOf() ) ) ) )
			{
				d3d12Debug1->SetEnableGPUBasedValidation( TRUE );

				Microsoft::WRL::ComPtr< ID3D12Debug2 > d3d12Debug2;
				if( eeCheck( D3D12GetDebugInterface( IID_PPV_ARGS( d3d12Debug2.GetAddressOf() ) ) ) )
				{
					// Level 3 adds resource state validation
					d3d12Debug2->SetGPUBasedValidationFlags(
						gGraphicsEnableDebugLayer > 2 ? D3D12_GPU_BASED_VALIDATION_FLAGS_NONE
													  : D3D12_GPU_BASED_VALIDATION_FLAGS_DISABLE_STATE_TRACKING );

				} // if( eeCheck( D3D12GetDebugInterface( ID3D12Debug2 ) ) ) )

			} // if( eeCheck( D3D12GetDebugInterface( ID3D12Debug1 ) ) ) )

		} // if( gGraphicsEnableDebugLayer > 1 )

	} // if( gGraphicsEnableDebugLayer > 0 )

	if( gGraphicsEnableDRED )
	{
		// enable D3D12 Device Removed Extended Data:
		// https://devblogs.microsoft.com/directx/dred
		Microsoft::WRL::ComPtr< ID3D12DeviceRemovedExtendedDataSettings > dredSettings;
		if( eeCheck( D3D12GetDebugInterface( IID_PPV_ARGS( dredSettings.GetAddressOf() ) ) ) )
		{
			if( gGraphicsEnableDREDPageFaults )
			{
				dredSettings->SetPageFaultEnablement( D3D12_DRED_ENABLEMENT_FORCED_ON );
			}

			if( gGraphicsEnableDREDBreadcrumbs )
			{
				dredSettings->SetAutoBreadcrumbsEnablement( D3D12_DRED_ENABLEMENT_FORCED_ON );
			}

		} //  // if( eeCheck( D3D12GetDebugInterface( ID3D12DREDSettings ) ) ) )

		// Windows 10 20H1 adds DRED 1.2:
		// https://devblogs.microsoft.com/directx/dred-v1-2-supports-pix-marker-and-event-strings-in-auto-breadcrumbs/
		Microsoft::WRL::ComPtr< ID3D12DeviceRemovedExtendedDataSettings1 > dredSettings1;
		if( eeCheck( D3D12GetDebugInterface( IID_PPV_ARGS( dredSettings1.GetAddressOf() ) ) ) )
		{
			dredSettings1->SetBreadcrumbContextEnablement( D3D12_DRED_ENABLEMENT_FORCED_ON );
		}

	} // if( gGraphicsEnableDRED )

	// Disallow attach to PIX if any debug capabilities are enabled.
	// The PIX documentation specifically warns that attaching
	// is not hardened against invalid API usage.
	if( ( gGraphicsEnableDebugLayer > 0 ) || gGraphicsEnableDRED )
	{
		mAllowPIXAttach = false;
	}

	return S_OK;
}

HRESULT dx12Device::CreateDeviceFactory( void )
{
	DWORD dxgiFactoryFlags = 0;

	if( gGraphicsEnableDebugLayer > 0 )
	{
		Microsoft::WRL::ComPtr< IDXGIInfoQueue > dxgiInfoQueue;
		if( eeCheck( DXGIGetDebugInterface1( 0, IID_PPV_ARGS( dxgiInfoQueue.GetAddressOf() ) ) ) )
		{
			dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;

			dxgiInfoQueue->SetBreakOnSeverity( DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_ERROR, true );
			dxgiInfoQueue->SetBreakOnSeverity( DXGI_DEBUG_ALL, DXGI_INFO_QUEUE_MESSAGE_SEVERITY_CORRUPTION, true );

			DXGI_INFO_QUEUE_MESSAGE_ID hide[] = {
				80 // IDXGISwapChain::GetContainingOutput: The swapchain's adapter does not control the output on which the swapchain's window resides.
			};

			DXGI_INFO_QUEUE_FILTER filter = {};

			filter.DenyList.NumIDs	= _countof( hide );
			filter.DenyList.pIDList = hide;

			eeCheck( dxgiInfoQueue->AddStorageFilterEntries( DXGI_DEBUG_DXGI, &filter ) );

		} // if( eeCheck( DXGIGetDebugInterface1( ... ) ) )

	} // if( gGraphicsEnableDebugLayer > 0 )

	return CreateDXGIFactory2( dxgiFactoryFlags, IID_PPV_ARGS( mDXGIFactory.ReleaseAndGetAddressOf() ) );
}

HRESULT dx12Device::VerifyMinimumFeatureLevel( IDXGIAdapter1* adapter, D3D_FEATURE_LEVEL level ) const
{
	DXGI_ADAPTER_DESC1 desc;
	adapter->GetDesc1( &desc );

	// Ignore software adapters
	if( desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE )
	{
		return DXGI_ERROR_UNSUPPORTED;
	}

	// D3D12CreateDevice() may return S_FALSE because we're passing NULL for ppDevice
	HRESULT result = D3D12CreateDevice( adapter, level, __uuidof( ID3D12Device ), nullptr );
	if( ( result != S_OK ) && ( result != S_FALSE ) )
	{
		constexpr size_t descriptionSize = 128; // characters
		char description[ descriptionSize ];
		DXGetErrorDescription( result, description, descriptionSize );

		eeDebug( "D3D12CreateDevice( %s ) returned error 0x%08x (%s): %ls\n", GetD3DFeatureLevelString( level ), result,
				 DXGetErrorString( result ), description );

		return result;
	}

	return S_OK;
}

#if defined( BUILD_PROFILE )

bool dx12Device::LoadPIXDLL( void )
{
	// Per https://devblogs.microsoft.com/pix/taking-a-capture/#attach :
	// In order for PIX to attach after launch, the PIX capture DLL must be
	// loaded, and from the PIX install directory.

	LPWSTR wideProgramFilesPath;
	if( !eeCheck( SHGetKnownFolderPath( FOLDERID_ProgramFiles, KF_FLAG_DEFAULT, NULL, &wideProgramFilesPath ) ) )
	{
		return false;
	}

	char   programFilesPath[ MAX_PATH ];
	size_t convertedCount;
	wcstombs_s( &convertedCount, programFilesPath, MAX_PATH, wideProgramFilesPath, _TRUNCATE );

	// Create a path that will enumerate the contents of the Microsoft PIX
	// directory but don't recurse into subdirectories
	FilePath pixPath = FilePath( programFilesPath ).Append( "Microsoft PIX\\*" );

	//	eeDebug( "dx12Device::LoadPIXDLL: Searching %s\n", pixPath.c_str() );

	bool		foundPixInstallation = false;
	std::string latestVersionPath;

	WIN32_FIND_DATA findData;
	HANDLE			findHandle = FindFirstFile( pixPath.c_str(), &findData );
	if( findHandle != INVALID_HANDLE_VALUE )
	{
		do
		{
			// Ignore anything that isn't a directory here (and ignore '.' too)
			if( ( ( findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) == FILE_ATTRIBUTE_DIRECTORY ) &&
				( findData.cFileName[ 0 ] != '.' ) )
			{
				//				eeDebug( "dx12Device::LoadPIXDLL: Examining %s\n", findData.cFileName );

				// Look for the subdirectory with the highest version number
				if( !foundPixInstallation || latestVersionPath.compare( findData.cFileName ) <= 0 )
				{
					foundPixInstallation = true;
					latestVersionPath	 = std::string( findData.cFileName );
				}
			}
		} while( FindNextFile( findHandle, &findData ) != 0 );

	} // if( findHandle != INVALID_HANDLE_VALUE )

	FindClose( findHandle );

	if( !foundPixInstallation )
	{
		// Not a eeLog, this message isn't that important...
		eeDebug( "dx12Device::LoadPIXDLL: A PIX installation was not found in %s\n", pixPath.c_str() );
		return false;
	}

	// Load the WinPixGPUCapturer.dll module from the directory we found

	FilePath capturerPath(
		std::string_view( pixPath.c_str(), pixPath.size() - 1 ) ); // clip off the '*' from the end of pixPath
	capturerPath.Append( latestVersionPath );
	capturerPath.Append( "WinPixGPUCapturer.dll" );

	// Same here, not important enough to rate a eeLog call
	eeDebug( "dx12Device::LoadPIXDLL: Loading %s\n", capturerPath.c_str() );

	// Note, this module will be freed by dx12Device::Release()
	mPIXCapturerModule = LoadLibrary( capturerPath.c_str() );
	return mPIXCapturerModule != NULL;
}

#endif // #if defined( BUILD_PROFILE )
