// VideoPlayer application - part of Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#include "pch.h"

#include <cstdint>

#include <ee/core/Debug.h>
#include <ee/io/File.h>
#include <ee/utility/Config.h>
#include <drivers/Windows/core/WinApplication.h>
#include <drivers/Windows/core/WinWindow.h>
#include <drivers/Windows/core/WinDebug.h>

#include "resource.h"
#include "VideoPlayer.h"

using namespace ee;

class VideoPlayerApplication : public WinApplication
{
public:
	// Application interface implementation

	virtual const char* GetName( void ) const override final { return "VideoPlayer"; }

	virtual int Main( int argCount, const char* args[] ) override final;

	virtual bool Initialize( void ) override final;
	virtual void Shutdown( void ) override final;

	virtual Config* GetConfig( void ) noexcept override final
	{
		return &mConfig;
	}

	// VideoPlayerApplication member functions

	VideoPlayer& GetPlayer( void )
	{
		return mPlayer;
	}

	const VideoPlayer& GetPlayer( void ) const
	{
		return mPlayer;
	}

private:
	static constexpr const char* sConfigName = "VideoPlayer.cfg";

	VideoPlayer mPlayer;
	Config		mConfig;

}; // class VideoPlayerApplication

// Because this object represents the running application (ie .exe file)
// a Meyer singleton is an appropriate pattern to use here
Application& Application::GetInstance( void )
{
	static VideoPlayerApplication application;
	return application;
}

// Message handler for about box
static INT_PTR CALLBACK AboutDialogProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	UNREFERENCED_PARAMETER( lParam );

	switch( message )
	{
	case WM_INITDIALOG:
	{
		return INT_PTR( TRUE );
	}

	case WM_COMMAND:
	{
		if( LOWORD( wParam ) == IDOK || LOWORD( wParam ) == IDCANCEL )
		{
			EndDialog( hDlg, LOWORD( wParam ) );
			return INT_PTR( TRUE );
		}
	}
	break;

	} // switch( message )

	return INT_PTR( FALSE );
}

//
//	FUNCTION: WndProc( HWND, UINT, WPARAM, LPARAM )
//
//	PURPOSE: Processes messages for the main window.
//
static LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
//	eeDebug( "VideoPlayer WndProc: %s wParam = %d lParam = 0x%08x\n", WinDebug::GetMessageString( message ), wParam, lParam );

	switch( message )
	{
	case WM_CREATE:
	{
		CREATESTRUCT* create = reinterpret_cast< CREATESTRUCT* >( lParam );
		VideoPlayerApplication* application = reinterpret_cast< VideoPlayerApplication* >( create->lpCreateParams );

		// Store the pointer in the instance data of the window so it can
		// be retrieved by using GetWindowLongPtr( hWnd, GWLP_USERDATA )
        SetWindowLongPtr( hWnd, GWLP_USERDATA, LONG_PTR( application ) );
	}
	break;

	case WM_COMMAND:
	{
		VideoPlayerApplication* application = reinterpret_cast< VideoPlayerApplication* >( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );

		int wmId = LOWORD( wParam );
		int wmEvent = HIWORD( wParam );

		// Parse the menu selections:
		switch( wmId )
		{
		case IDM_ABOUT:
		{
			DialogBox( application->GetHInstance(), MAKEINTRESOURCE( IDD_ABOUTBOX ), hWnd, AboutDialogProc );
		}
		break;

		case IDM_EXIT:
		{
			application->Exit();
			DestroyWindow( hWnd );
		}
		break;

		default: return DefWindowProc( hWnd, message, wParam, lParam );

		} // switch( wmId )
	}
	break;

	case WM_CLOSE:
	{
		VideoPlayerApplication* application = reinterpret_cast< VideoPlayerApplication* >( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );

		application->Exit();
		DestroyWindow( hWnd );
	}
	break;

	case WM_DESTROY:
	{
		VideoPlayerApplication* application = reinterpret_cast< VideoPlayerApplication* >( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );

		application->Exit();
		PostQuitMessage( 0 );
	}
	break;

	case WM_SIZE:
	{
		switch( wParam )
		{
		case SIZE_MINIMIZED:
		case SIZE_MAXHIDE:
		{
//			PauseGame();
		}
		break;

		case SIZE_RESTORED:
		case SIZE_MAXIMIZED:
		case SIZE_MAXSHOW:
		{
//			ResumeGame();
		}
		break;

		} // switch( wParam )
	}
	break;

	default: return DefWindowProc( hWnd, message, wParam, lParam );

	} // switch( message )

	return 0;
}

bool VideoPlayerApplication::Initialize( void )
{
	mRunning = true;
	std::shared_ptr< File > configFile = std::make_shared< File >( sConfigName );
	mConfig.LoadConfig( configFile );
	return mPlayer.Initialize( 1280,720 );
}

void VideoPlayerApplication::Shutdown( void )
{
	std::shared_ptr< File > configFile = std::make_shared< File >( sConfigName );
	mConfig.SaveConfig( configFile );

	mPlayer.Shutdown();
}

int VideoPlayerApplication::Main( int argCount, const char* args[] )
{
	while( mRunning )
	{
		if( !Update() )
		{
			break;
		}

	} // while( mRunning )

	Shutdown();

	return 0;
}

int APIENTRY wWinMain( _In_ HINSTANCE hInstance,
					   _In_opt_ HINSTANCE hPrevInstance,
					   _In_ LPWSTR lpCmdLine,
					   _In_ int nCmdShow )
{
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );

	static constexpr size_t MAX_LOADSTRING = 100;

	CHAR szTitle[ MAX_LOADSTRING ];			// The title bar text
	CHAR szWindowClass[ MAX_LOADSTRING ];	// the main window class name
	LoadString( hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING );
	LoadString( hInstance, IDC_VIDEOPLAYER, szWindowClass, MAX_LOADSTRING );

	WNDCLASSEX wcex;

	wcex.cbSize			= sizeof( WNDCLASSEX );
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_GLOBALCLASS;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon( hInstance, MAKEINTRESOURCE( IDI_VIDEOPLAYER ) );
	wcex.hCursor		= LoadCursor( nullptr, IDC_ARROW );
	wcex.hbrBackground	= HBRUSH( COLOR_WINDOW + 1 );
	wcex.lpszMenuName	= MAKEINTRESOURCE( IDC_VIDEOPLAYER );
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon( hInstance, MAKEINTRESOURCE( IDI_SMALL ) );

	ATOM wndclass = RegisterClassEx( &wcex );

	// Trigger the instantiation of the VideoPlayerApplication object
	VideoPlayerApplication& application = static_cast< VideoPlayerApplication& >( Application::GetInstance() );
	application.SetHInstance( hInstance );

	HACCEL table = LoadAccelerators( hInstance, MAKEINTRESOURCE( IDC_VIDEOPLAYER ) );
	if( table != nullptr )
	{
		application.SetAccelTable( table );
	}

	if( !application.Initialize() )
	{
		return -1;
	}

	uint16_t width, height;
	application.GetPlayer().GetDimensions( width, height );

	WinWindow& window = application.GetApplicationWindow();

	window.SetWindowClassName( szWindowClass );
	window.SetWindowTitle( szTitle );
	window.SetWindowProc( WndProc );

	if( !window.CreateHWND( width, height, DisplayMode::kWindowed, &application ) )
	{
		return -1;
	}

	return application.Main( 0, nullptr );
}
