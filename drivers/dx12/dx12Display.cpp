// Elevation Engine
//
// Copyright (C) 2025 Azimuth Studios

#include "pch.h"

#include "dx12Display.h"

#include <ee/core/Window.h>
#include <drivers/dx12/dx12Device.h>
#include <drivers/dx12/dx12Utils.h>
#include <drivers/windows/core/WinCheck.h>
#include <drivers/windows/core/WinWindow.h>

using namespace ee;

bool dx12Display::Initialize( GpuFormat format, Window* window, Device* device )
{
	// We can assume that window is a WinWindow here.
	// A HWND is required to create a swap chain.
	if( window == nullptr )
	{
		eeDebug( "dx12Display::Initialize: A Window is required\n" );
		return false;
	}

	uint16_t width, height;
	window->GetSize( width, height );
	
	if( ( width == 0 ) || ( height == 0 ) )
	{
		eeDebug( "dx12Display::Initialize: The Window has invalid dimensions: %d x %d\n", width, height );
		return false;
	}

	// We can assume that device is a dx12Device here.
	// It is required to create a swap chain.
	if( device == nullptr )
	{
		eeDebug( "dx12Display::Initialize: A dx12Device is required\n" );
		return false;
	}

	if( !device->IsInitialized() )
	{
		eeDebug( "dx12Display::Initialize: The dx12Device must be initialized first\n" );
		return false;
	}

	dx12Device* dxDevice = static_cast< dx12Device* >( device );
	WinWindow* winWindow = static_cast< WinWindow* >( window );

	IDXGIFactory6* dxgiFactory = dxDevice->GetDXGIFactory();
	if( dxgiFactory == nullptr )
	{
		eeDebug( "dx12Display::Initialize: The dx12Device is missing its IDXGIFactory\n" );
		return false;
	}

	ID3D12CommandQueue* queue = dxDevice->GetDirectCommandQueue();
	if( dxgiFactory == nullptr )
	{
		eeDebug( "dx12Display::Initialize: The dx12Device is missing its ID3D12CommandQueue\n" );
		return false;
	}

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {
		.Width		 = width,
		.Height		 = height,
		.Format		 = GpuFormatToDXGI_FORMAT( format ),
		.Stereo		 = FALSE,
		.SampleDesc	 = { .Count = 1, .Quality = 0 },
		.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT,
		.BufferCount = kSwapChainBufferCount,
		.Scaling	 = DXGI_SCALING_NONE,
		.SwapEffect	 = DXGI_SWAP_EFFECT_FLIP_DISCARD,
		.AlphaMode	 = DXGI_ALPHA_MODE_IGNORE,
		.Flags		 = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
	  };

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {
		.RefreshRate	  = { 0, 0 },
		.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED,
		.Scaling		  = DXGI_MODE_SCALING_UNSPECIFIED,
		.Windowed		  = TRUE
	 };

	// While IDXGIFactory::CreateSwapChain[For...] calls its first argument pDevice,
	// it actually requires an ID3D12CommandQueue.
	if( !eeCheck( dxgiFactory->CreateSwapChainForHwnd( queue, winWindow->GetHWND(),
													   &swapChainDesc, &fsSwapChainDesc, nullptr,
													   mSwapChain.ReleaseAndGetAddressOf() ) ) )
	{
		return false;
	}

	{
		IDXGISwapChain4* swapchain4 = static_cast< IDXGISwapChain4* >( mSwapChain.Get() );
		Microsoft::WRL::ComPtr< IDXGIOutput >  output;
		Microsoft::WRL::ComPtr< IDXGIOutput6 > output6;
		DXGI_OUTPUT_DESC1 outputDesc;
		UINT colorSpaceSupport;

		// Query support for ST.2084 on the display and set the color space accordingly
		mHDROutputEnabled = eeCheck( swapchain4->GetContainingOutput( &output ) ) && eeCheck( output.As( &output6 ) ) &&
							eeCheck( output6->GetDesc1( &outputDesc ) ) &&
							outputDesc.ColorSpace == DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020 &&
							eeCheck( swapchain4->CheckColorSpaceSupport( DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020,
																		 &colorSpaceSupport ) ) &&
							( colorSpaceSupport & DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG_PRESENT ) &&
							eeCheck( swapchain4->SetColorSpace1( DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020 ) );
	}

	ID3D12Device* d3d12Device = dxDevice->GetD3D12Device();
	// dx12Device::IsInitialized() tests that the ID3D12Device is not nullptr,
	// so a separate nullptr check here would be redundant

	for( uint32_t i = 0; i < kSwapChainBufferCount; ++i )
	{
		Microsoft::WRL::ComPtr< ID3D12Resource > buffer;
		if( eeCheck( mSwapChain->GetBuffer( i, IID_PPV_ARGS( &buffer ) ) ) )
		{
			mFrameBuffers[ i ].CreateFromSwapChain( d3d12Device, buffer.Detach(), "dx12Display::mFrameBuffers" );
		}
		else
		{
			return false;
		}

	} // for( uint32_t i = 0; i < kSwapChainBufferCount; ++i )

	return true;
}

void dx12Display::Release( void ) {}

void dx12Display::EnableHDR( bool useHDR ) {}

void dx12Display::Resize( uint32_t width, uint32_t height ) {}

void dx12Display::Present( void ) {}
