// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#include "pch.h"

#include "WinWindow.h"

#include <drivers/windows/core/WinApplication.h>
#include <drivers/windows/core/WinCheck.h>
#include <drivers/windows/core/WinUtil.h>

using namespace ee;

//*****************************************************************************
//*****************************************************************************

// Note: This basic WndProc is usually replaced by an application-specific proc
LRESULT CALLBACK WinWindow::WindowProc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam )
{
	switch( message )
	{
		case WM_SETCURSOR:
		{
			return true;
		}

		case WM_ACTIVATE:
		{
			switch( wparam )
			{
				case WA_ACTIVE:
				case WA_CLICKACTIVE:
					SetFocus( hwnd );
					break;
			}

			return true;
		}

	} // switch( message )

	return DefWindowProc( hwnd, message, wparam, lparam );
}

//*****************************************************************************
//*****************************************************************************

WinWindow::WinWindow()
	: mWindowClassName( "Elevation" )
	, mWindowTitle( "Elevation Window" )
	, mWindowWidth( 1920 )
	, mWindowHeight( 1080 )
	, mClientWidth( 1920 )
	, mClientHeight( 1080 )
	, mAspectRatio( 1920.0f / 1080.0f )
	, mWindowStyle( WS_VISIBLE | WS_POPUP ) // assume fullscreen display
	, mWindowExStyle( 0 )
	, mWindowProc( WinWindow::WindowProc )
	, mHwnd( NULL )
	, mOwnWindow( false )
{
}

WinWindow::~WinWindow()
{
}

void WinWindow::SetWindowClassName( const char* windowClass )
{
	mWindowClassName = windowClass;
}

void WinWindow::SetWindowTitle( const char* windowTitle )
{
	mWindowTitle = windowTitle;
}

void WinWindow::SetWindowProc( WNDPROC proc )
{
	mWindowProc = ( proc != nullptr ) ? proc : WinWindow::WindowProc;
}

bool WinWindow::RegisterWindowClass( void )
{
	WinApplication& application = WinApplication::GetInstance();
	HINSTANCE hinstance = application.GetHInstance();

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof( WNDCLASSEX );

	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS | CS_GLOBALCLASS;
	wcex.lpfnWndProc	= mWindowProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hinstance;
	wcex.hIcon			= LoadIcon( hinstance, LPCTSTR( IDI_APPLICATION ) );
	wcex.hCursor		= NULL;
	wcex.hbrBackground	= NULL;
	wcex.lpszMenuName	= mWindowTitle;
	wcex.lpszClassName	= mWindowClassName;
	wcex.hIconSm		= LoadIcon( hinstance, LPCTSTR( IDI_APPLICATION ) );

	if( RegisterClassEx( &wcex ) == 0 )
	{
		DWORD error = GetLastError();
		// !!! if a logging facility is added use that instead of eeDebug here
		eeDebug( "WinWindow::Initialize: RegisterClassEx() failed with error 0x%08x (%s)!", error, WinUtil::GetErrorString( error ).c_str() );
		return false;
	}

	return true;
}

// create a window to attach to the Direct3D device
bool WinWindow::CreateHWND( uint16_t width, uint16_t height, Application::DisplayMode mode, void* windowdata )
{
	WinApplication& application = WinApplication::GetInstance();
	HINSTANCE hinstance = application.GetHInstance();

	mWindowStyle = GetWindowStyle( mode );
	mWindowExStyle = 0;

	RECT screenRect;
	screenRect.top = screenRect.left = 0;
	screenRect.right = GetSystemMetrics( SM_CXSCREEN );
	screenRect.bottom = GetSystemMetrics( SM_CYSCREEN );

	if( mode == Application::kDisplayFullscreenWindow )
	{
		// In fullscreen window mode we ignore the width and height parameters
		// and use the monitor dimensions instead.
		mWindowExStyle = WS_EX_APPWINDOW;

		RECT rect = screenRect;

		if( eeCheckBool( AdjustWindowRectEx( &rect, mWindowStyle, FALSE, mWindowExStyle ) ) )
		{
			LONG windowWidth = rect.right - rect.left;
			LONG windowHeight = rect.bottom - rect.top;

//			eeDebug( "WinWindow::init: Adjusting %d x %d to %d x %d\n", width, height, windowWidth, windowHeight );

			width = static_cast< uint16_t >( eeClamp( windowWidth, 0L, LONG( USHRT_MAX ) ) );
			height = static_cast< uint16_t >( eeClamp( windowHeight, 0L, LONG( USHRT_MAX ) ) );
		}
		else
		{
			// if we can't get the monitor dimensions then try to use
			// the dimensions passed to this function
			AdjustWindowDimensions( width, height );
		}

		// The client dimensions matches the window dimensions here
		mClientWidth = width;
		mClientHeight = height;
	}
	else if( mode == Application::kDisplayWindowed )
	{
		// If the requested height equals the monitor's height then there won't
		// be room for the title bar so we need to choose a smaller resolution
		if( height >= screenRect.bottom )
		{
			// !!! if a logging facility is added use that instead of eeDebug here
			eeDebug( "WinWindow::Initialize: The requested window resolution of %dx%d will not fit on a %dx%d monitor; downgrading the requested resolution", width, height, screenRect.right, screenRect.bottom );

			uint16_t windowWidth = width, windowHeight = height;
			AdjustWindowDimensions( windowWidth, windowHeight );

			uint16_t borderWidth = windowWidth - width;
			uint16_t borderHeight = windowHeight - height;

			// Shrink the window to fit the monitor
			width -= borderWidth;
			height -= borderHeight;
		}

		// width/height define the client dimensions in this case
		mClientWidth = width;
		mClientHeight = height;

		// Adjust the window dimensions to account for the title bar and borders
		AdjustWindowDimensions( width, height );
	}
	else // mode == kDisplayFullscreen
	{
		// Initialize the client dimensions to the selected resolution
		// The window dimensions will be identical in this case
		mClientWidth = width;
		mClientWidth = height;
	}

	// Note: A width or height of 0 is valid, used by the tools to create
	// the HWND required by OpenGL or DirectX even for display-less tasks
	// like shader compilation or mesh compression.

	mHwnd = CreateWindowEx(
		mWindowExStyle, mWindowClassName, mWindowTitle,
		mWindowStyle, 0, 0, width, height, NULL, NULL, hinstance, windowdata );
	if( mHwnd == NULL )
	{
		DWORD error = GetLastError();
		// !!! if a logging facility is added use that instead of eeDebug here
		eeDebug( "WinWindow::Initialize: CreateWindowEx() failed with error 0x%08x (%s)!", error, WinUtil::GetErrorString( error ).c_str() );
		return false;
	}

	if( mode == Application::kDisplayFullscreenWindow )
	{
		SetWindowPos( mHwnd, NULL, 0, 0, width, height, SWP_NOACTIVATE | SWP_FRAMECHANGED );
	}

	ShowWindow( mHwnd, SW_SHOW );
	UpdateWindow( mHwnd );

	mOwnWindow = true;
	mWindowWidth = width;
	mWindowHeight = height;
	mAspectRatio = mClientWidth / (float) mClientHeight;

	application.SetParentWindow( mHwnd );

	return true;
}

void WinWindow::SetWindowSize( uint16_t width, uint16_t height )
{
	// width/height define the client region dimensions
	mClientWidth = width;
	mClientHeight = height;

	// If we didn't create the window, don't mess with its dimensions
	if( mOwnWindow )
	{
		AdjustWindowDimensions( width, height );
	}

	mWindowWidth = width;
	mWindowHeight = height;

	mAspectRatio = mClientWidth / float( mClientHeight );

	if( mOwnWindow )
	{
		SetWindowPos( mHwnd, HWND_NOTOPMOST, 0, 0, mWindowWidth, mWindowHeight, SWP_NOMOVE );
	}
}

void WinWindow::SetHWND( HWND hwnd, Application::DisplayMode mode )
{
	mHwnd = hwnd;

    WinApplication& application = WinApplication::GetInstance();
    application.SetParentWindow( hwnd );

	// The window style will change if the display mode changes
	mWindowStyle = GetWindowStyle( mode );

	SetWindowLong( mHwnd, GWL_STYLE, mWindowStyle );
	ShowWindow( mHwnd, SW_SHOW );

	if( mode != Application::kDisplayFullscreen )
	{
		RECT rect;
		if( eeCheckBool( GetWindowRect( hwnd, &rect ) ) )
		{
			mWindowWidth = static_cast< uint16_t >( rect.right - rect.left );
			mWindowHeight = static_cast< uint16_t >( rect.bottom - rect.top );
		}

		if( eeCheckBool( GetClientRect( hwnd, &rect ) ) )
		{
			mClientWidth = static_cast< uint16_t >( rect.right - rect.left );
			mClientHeight = static_cast< uint16_t >( rect.bottom - rect.top );
		}

		mAspectRatio = mClientWidth / static_cast< float >( mClientHeight );
	}
}

DWORD WinWindow::GetWindowStyle( Application::DisplayMode mode )
{
	DWORD fullscreenFlags = WS_VISIBLE | WS_POPUP;
	DWORD windowedFlags = WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

#if !defined( EE_BUILD_RETAIL )
	windowedFlags |= WS_MAXIMIZEBOX;
#endif

	// Application::kDisplayFullscreenWindow uses the same style flags as fullscreen
	return ( mode == Application::kDisplayWindowed ) ? windowedFlags : fullscreenFlags;
}

void WinWindow::AdjustWindowDimensions( uint16_t& width, uint16_t& height )
{
	// Adjust the width and height to account for the title bar and borders
	RECT rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = width - 1;
	rect.bottom = height - 1;

	if( eeCheckBool( AdjustWindowRect( &rect, mWindowStyle, FALSE ) ) )
	{
		LONG windowWidth = rect.right - rect.left + 1;
		LONG windowHeight = rect.bottom - rect.top + 1;

//		eeDebug( "WinWindow::AdjustWindowDimensions: Adjusting %d x %d to %d x %d\n", width, height, windowWidth, windowHeight );

		width = static_cast< uint16_t >( eeClamp( windowWidth, 0L, LONG( USHRT_MAX ) ) );
		height = static_cast< uint16_t >( eeClamp( windowHeight, 0L, LONG( USHRT_MAX ) ) );
	}
}
