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

// You will usually want to pass in the results of GetLastError()
std::string WinUtil::GetErrorString( DWORD error )
{
    LPVOID message;
    FormatMessage (
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error,
        MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), // Default language
        (LPTSTR) &message, 0, NULL);

	std::string errorString = std::format( "{}", (LPTSTR) message );

    LocalFree( message );

    return errorString;
}
