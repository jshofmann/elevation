// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#include "pch.h"

#include <psapi.h>

#include <drivers/windows/core/WinSystem.h>

using namespace ee;

typedef BOOL ( WINAPI *LPFN_ISWOW64PROCESS ) ( HANDLE, PBOOL );

static LPFN_ISWOW64PROCESS IsWow64ProcessFunction =
	reinterpret_cast< LPFN_ISWOW64PROCESS >(
		GetProcAddress( GetModuleHandle( "kernel32" ), "IsWow64Process" ) );

typedef BOOL ( __stdcall *GetProcessMemoryInfoType )( HANDLE, PPROCESS_MEMORY_COUNTERS, DWORD );

static GetProcessMemoryInfoType GetProcessMemoryInfoPtr = NULL;

static bool gHasOpenModalDialog = false;

//******************************************************************************
//******************************************************************************

WinSystem::WinSystem()
{
	mPlatform = Platform::kWindows;
	mDriver = Driver::kDirectX12;

	eeSetGlobalFlag( GlobalSettings::kDebuggerAttached, IsDebuggerAttached() );
}

// The Driver should only be assigned once on application launch
/* static */ void WinSystem::SetDriver( Driver driver )
{
	mDriver = driver;
}

/* static */ uint32_t System::GetCoreCount( void )
{
	SYSTEM_INFO	info;
	GetSystemInfo( &info );
	return uint32_t( info.dwNumberOfProcessors );
}

/* static */ bool System::IsDebuggerAttached( void )
{
	return ( IsDebuggerPresent() == TRUE );
}


void System::DisplayAlert( const char* title, const char* message )
{
	// find top window
	HWND parent = GetActiveWindow();
	if( parent != NULL )
	{
		parent = GetLastActivePopup( parent );
	}

	if( gHasOpenModalDialog )
	{
		// Don't deadlock the UI by opening two message boxes,
		// use OutputDebugString() instead
		eeDebug( "WinSystem::DisplayAlert: %s: %s", title, message );
	}
	else
	{
		gHasOpenModalDialog = true;
		::MessageBox( parent, message, title, MB_TASKMODAL | MB_SETFOREGROUND | MB_TOPMOST | MB_OK | MB_ICONERROR );
		gHasOpenModalDialog = false;
	}
}

int System::DisplayAlertWithOptions( const char* title, const char* message )
{
	// find top window
	HWND parent = GetActiveWindow();
	if( parent != NULL )
	{
		parent = GetLastActivePopup( parent );
	}

	if( gHasOpenModalDialog )
	{
		// Don't deadlock the UI by opening two message boxes,
		// use OutputDebugString() instead
		eeDebug( "WinSystem::DisplayAlert: %s: %s", title, message );
	}
	else
	{
		gHasOpenModalDialog = true;
		int ret = ::MessageBox( parent, message, title, MB_TASKMODAL | MB_SETFOREGROUND | MB_TOPMOST | MB_YESNOCANCEL | MB_ICONERROR );
		gHasOpenModalDialog = false;

		switch( ret )
		{
		case IDYES:		return kOptionYes;
		case IDNO:		return kOptionNo;
		case IDCANCEL:	return kOptionCancel;
		}
	}

	// default:
	return kOptionYes;
}

/* static */ bool WinSystem::IsWin64( void )
{
	BOOL win64 = FALSE;

	if( IsWow64ProcessFunction != NULL )
	{
		if( !IsWow64ProcessFunction( GetCurrentProcess(), &win64 ) )
		{
			OutputDebugString( "WinSystem::IsWin64: IsWow64Process() failed! Assuming 32-bit Windows..." );
			win64 = FALSE;
		}
	}

	return ( win64 == TRUE ) ? true : false;
}

/* static */ uint32_t WinSystem::GetWorkingSetSize( void )
{
	// Since we have to load and use the psapi.dll for this,
	// I don't think we want to ship with it in.
#if !defined( BUILD_RETAIL )

	static bool inited = false;
	if( !inited )
	{
		inited = true;
		HINSTANCE dllHandle = LoadLibrary( "psapi.dll" );
		if( dllHandle != NULL )
		{
			GetProcessMemoryInfoPtr = (GetProcessMemoryInfoType)
				GetProcAddress( dllHandle, "GetProcessMemoryInfo");
		}
	}

	if( GetProcessMemoryInfoPtr != NULL )
	{
		PROCESS_MEMORY_COUNTERS pmc;
		memset( &pmc, 0, sizeof( pmc ) );
		pmc.cb = sizeof( pmc );

		if( GetProcessMemoryInfoPtr( GetCurrentProcess(), &pmc, sizeof( pmc ) ) )
		{
			return uint32_t( pmc.WorkingSetSize );
		}
	}

#endif // if !defined( BUILD_RETAIL )

	return 0;
}
