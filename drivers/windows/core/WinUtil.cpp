// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#include "pch.h"

#include <format>

#include "WinUtil.h"

#include <drivers/windows/core/WinCheck.h>

using namespace ee;

RECT WinUtil::GetWindowDimensions( HWND hwnd, uint16_t& width, uint16_t& height )
{
	RECT rect;
	if( !eeCheckBool( GetClientRect( hwnd, &rect ) ) )
	{
		width = height = 0;
		rect.left = rect.top = rect.right = rect.bottom = 0;
		return rect;
	}

	// Calculate the window dimensions and make sure they're valid
	LONG windowWidth = rect.right - rect.left;
	LONG windowHeight = rect.bottom - rect.top;

	width = static_cast< uint16_t >( eeClamp( windowWidth, 0L, LONG( USHRT_MAX ) ) );
	height = static_cast< uint16_t >( eeClamp( windowHeight, 0L, LONG( USHRT_MAX ) ) );

	return rect;
}

bool WinUtil::IsRenderDocAttached( void )
{
	return ( GetModuleHandleA( "renderdoc.dll" ) != NULL );
}

bool WinUtil::IsPIXAttached( void )
{
	return ( ( GetModuleHandleA( "WinPixCaptureReplay.dll" ) != NULL ) ||
			 ( GetModuleHandleA( "WinPixGpuCapturer.dll" ) != NULL ) );
}

bool WinUtil::IsGPUDebuggerAttached( void )
{
	return IsRenderDocAttached() || IsPIXAttached();
}

LARGE_INTEGER WinUtil::ToLARGE_INTEGER( const size_t size )
{
	return { .QuadPart = LONGLONG( size ) };
}

size_t WinUtil::ToSize( const LARGE_INTEGER li )
{
	return size_t( li.QuadPart );
}

// adapted from https://learn.microsoft.com/en-us/windows/win32/netmgmt/looking-up-text-for-error-code-numbers

// Copied out of <lmerr.h>, don't want to drag that whole file in for two #defines
#define NERR_BASE       2100
#define MAX_NERR        (NERR_BASE+899) // This is the last error in NERR range

// You will usually want to pass in the results of GetLastError() here
std::string WinUtil::GetErrorString( DWORD error )
{
	HMODULE netmsgModule = nullptr;

	DWORD formatFlags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;

    // If error is in the network range load the message source
    if( error >= NERR_BASE && error <= MAX_NERR )
	{
        netmsgModule = LoadLibraryEx(
			TEXT( "netmsg.dll" ), nullptr, LOAD_LIBRARY_AS_DATAFILE );

        if( netmsgModule != nullptr )
		{
            formatFlags |= FORMAT_MESSAGE_FROM_HMODULE;
		}
    }

	std::string errorString;

    LPVOID message;
    DWORD length = FormatMessage( formatFlags, netmsgModule, error,
								  MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
								  reinterpret_cast< LPSTR >( &message ), 0, nullptr );
	if( length > 0 )
	{
		errorString = std::format( "{}", reinterpret_cast< LPTSTR >( message ) );
		LocalFree( message );
	}

	if( netmsgModule != nullptr )
	{
		FreeLibrary( netmsgModule );
	}

    return errorString;
}
