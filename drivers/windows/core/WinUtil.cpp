// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#include "pch.h"

#include <format>

#include "WinUtil.h"

#include <drivers/windows/core/WinCheck.h>

using namespace ee;

RECT WinUtil::getWindowDimensions( HWND hwnd, uint16_t& width, uint16_t& height )
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

bool WinUtil::isRenderDocAttached( void )
{
	return ( GetModuleHandleA( "renderdoc.dll" ) != NULL );
}

bool WinUtil::isPIXAttached( void )
{
	return ( ( GetModuleHandleA( "WinPixCaptureReplay.dll" ) != NULL ) ||
			 ( GetModuleHandleA( "WinPixGpuCapturer.dll" ) != NULL ) );
}

bool WinUtil::isGPUDebuggerAttached( void )
{
	return isRenderDocAttached() || isPIXAttached();
}

// You will usually want to pass in the results of GetLastError()
std::string WinUtil::getErrorString( DWORD error )
{
    LPVOID message;
    FormatMessage (
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS, NULL, error,
        MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ), // Default language
        (LPTSTR) &message, 0, NULL);

	std::string errorString = std::format( "{}", message );

    LocalFree( message );

    return errorString;
}
