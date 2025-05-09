// PathTracer application - part of Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#include "pch.h"

#include <cstdint>
#include <cstdarg>

#include <commctrl.h>

#include <ee/core/Debug.h>
#include <drivers/Windows/core/WinApplication.h>
#include <drivers/Windows/core/WinWindow.h>

#include "resource.h"
#include "PathTracer.h"
#include "ProgressBar.h"

class PathTracerApplication : public WinApplication
{
public:
	// Application interface implementation

	virtual const char* GetName( void ) const override final { return "PathTracer"; }

	virtual int Main( int argCount, const char* args[] ) override final;

	virtual bool Initialize( void ) override final
	{
		mRunning = true;
//		return mTracer.Initialize( 1280,720 );
		return mTracer.Initialize( 400, 200 );
	}

	// PathTracerApplication member functions

	PathTracer& GetTracer( void )
	{
		return mTracer;
	}

	const PathTracer& GetTracer( void ) const
	{
		return mTracer;
	}

	ProgressBar& GetProgressBar( void )
	{
		return mProgressBar;
	}

	HBITMAP GetBitmap( void ) const
	{
		return mBitmap;
	}

	void SetBitmap( HBITMAP bitmap )
	{
		mBitmap = bitmap;
	}

private:
	PathTracer mTracer;
	ProgressBar mProgressBar;
	HBITMAP mBitmap = NULL;

}; // class PathTracerApplication

// Because this object represents the running application (ie .exe file)
// a Meyer singleton is an appropriate pattern to use here
Application& Application::GetInstance( void )
{
	static PathTracerApplication application;
	return application;
}

// Message handler for about box
static INT_PTR CALLBACK About( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
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
	switch( message )
	{
	case WM_CREATE:
	{
		CREATESTRUCT* create = reinterpret_cast< CREATESTRUCT* >( lParam );
		PathTracerApplication* application = reinterpret_cast< PathTracerApplication* >( create->lpCreateParams );

		// Store the pointer in the instance data of the window so it can
		// be retrieved by using GetWindowLongPtr( hWnd, GWLP_USERDATA )
        SetWindowLongPtr( hWnd, GWLP_USERDATA, LONG_PTR( application ) );
	}
	break;

	case WM_COMMAND:
	{
		int wmId = LOWORD( wParam );
		int wmEvent = HIWORD( wParam );

		// Parse the menu selections:
		switch( wmId )
		{
		case IDM_SAVE:
		{
			PathTracerApplication* application = reinterpret_cast< PathTracerApplication* >( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );

			// !!! should open a file chooser here...
			application->GetTracer().SaveImage( "image.tga" );
		}
		break;

		case IDM_ABOUT:
		{
			PathTracerApplication* application = reinterpret_cast< PathTracerApplication* >( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );

			DialogBox( application->GetHInstance(), MAKEINTRESOURCE( IDD_ABOUTBOX ), hWnd, About );
		}
		break;

		case IDM_EXIT:
		{
			DestroyWindow( hWnd );
		}
		break;

		default: return DefWindowProc( hWnd, message, wParam, lParam );

		} // switch( wmId )
	}
	break;

	case WM_CLOSE:
	{
		PathTracerApplication* application = reinterpret_cast< PathTracerApplication* >( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );

		application->Exit();
		DestroyWindow( hWnd );
	}
	break;

	case WM_PAINT:
	{
		PathTracerApplication* application = reinterpret_cast< PathTracerApplication* >( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );

		HBITMAP hbitmap = application->GetBitmap();

		PAINTSTRUCT ps;
		HDC hdc = BeginPaint( hWnd, &ps );

		HDC hdcMem = CreateCompatibleDC( hdc );
		HGDIOBJ oldBitmap = SelectObject( hdcMem, hbitmap );

		BITMAP bitmap;
		GetObject( hbitmap, sizeof( BITMAP ), &bitmap );

		BitBlt( hdc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, hdcMem, 0, 0, SRCCOPY );

		SelectObject( hdcMem, oldBitmap );
		DeleteDC( hdcMem );

		EndPaint( hWnd, &ps );
	}
	break;

	case WM_DESTROY:
	{
		PathTracerApplication* application = reinterpret_cast< PathTracerApplication* >( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );

		application->Exit();
		PostQuitMessage( 0 );
	}
	break;

	default: return DefWindowProc( hWnd, message, wParam, lParam );

	} // switch( message )

	return 0;
}

static void CopyBitmap( PathTracerApplication& application )
{
	HWND hwnd = application.GetApplicationWindow().GetHWND();

	const PathTracer& tracer = application.GetTracer();

	uint16_t width, height;
	tracer.GetDimensions( width, height );

	uint8_t bytesPerPixel = tracer.GetBytesPerPixel();

	BITMAPINFO info;
	ZeroMemory( &info, sizeof( info ) );

	info.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
	info.bmiHeader.biWidth = width;
	info.bmiHeader.biHeight = height;
	info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biBitCount = bytesPerPixel * 8;
	info.bmiHeader.biCompression = BI_RGB;
	info.bmiHeader.biSizeImage = 0;
	info.bmiHeader.biXPelsPerMeter = 10;
	info.bmiHeader.biYPelsPerMeter = 10;
	info.bmiHeader.biClrUsed = 0;
	info.bmiHeader.biClrImportant = 0;

	info.bmiColors[ 0 ].rgbBlue = 0;
	info.bmiColors[ 0 ].rgbGreen = 0;
	info.bmiColors[ 0 ].rgbRed = 0;
	info.bmiColors[ 0 ].rgbReserved = 0;

	HDC dc = GetDC( hwnd );

	const uint8_t* pixels = tracer.GetPixels();

	void* bits;
	HBITMAP bitmap = CreateDIBSection( dc, &info, DIB_RGB_COLORS, &bits, nullptr, 0 );

	uint32_t pixelsSize = width * height * bytesPerPixel;
	memcpy( bits, pixels, pixelsSize );

	application.SetBitmap( bitmap );

	ReleaseDC( hwnd, dc );
}

static void TracerCompleteCallback( const PathTracer& tracer, const void* data )
{
	if( data == nullptr )
		return;
		
	// data is const void* because PathTracer won't touch it;
	// the const_cast here is safe
	PathTracerApplication* application = reinterpret_cast< PathTracerApplication* >( const_cast< void * >( data ) );

	// Copy the results to the HBITMAP
	CopyBitmap( *application );

	// Make sure that they're visible
	InvalidateRect( application->GetApplicationWindow().GetHWND(), NULL, TRUE );

	application->GetProgressBar().Close();
}

static void TracerProgressCallback( uint16_t step, const void* data )
{
	if( data != nullptr )
	{
		// data is const void* because PathTracer won't touch it;
		// the const_cast here is safe
		PathTracerApplication* application = reinterpret_cast< PathTracerApplication* >( const_cast< void * >( data ) );

		application->GetProgressBar().SetPosition( step );
	}
}

int PathTracerApplication::Main( int argCount, const char* args[] )
{
	mProgressBar.Open( mApplicationWindow.GetHWND(), 100, 1, "PathTracer progress" );

	mTracer.SetProgressCallback( TracerProgressCallback, this );
	mTracer.SetCompleteCallback( TracerCompleteCallback, this );

	mTracer.StartTrace();

	// Spawn the tracing threads
	mTracer.Trace();

	// Main loop:
	while( mRunning )
	{
		if( !Update() )
		{
			break;
		}

	} // while( mRunning )

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
	LoadString( hInstance, IDC_PATHTRACER, szWindowClass, MAX_LOADSTRING );

	// The common controls library must be initialized
	// in order to use a Windows progress bar.
	INITCOMMONCONTROLSEX init;
	memset( &init, 0, sizeof( INITCOMMONCONTROLSEX ) );
	init.dwSize = sizeof( INITCOMMONCONTROLSEX );
	init.dwICC = ICC_PROGRESS_CLASS;
	InitCommonControlsEx( &init );

	WNDCLASSEX wcex;

	wcex.cbSize			= sizeof( WNDCLASSEX );
	wcex.style			= CS_HREDRAW | CS_VREDRAW | CS_GLOBALCLASS;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon( hInstance, MAKEINTRESOURCE( IDI_PATHTRACER ) );
	wcex.hCursor		= LoadCursor( nullptr, IDC_ARROW );
	wcex.hbrBackground	= HBRUSH( COLOR_WINDOW + 1 );
	wcex.lpszMenuName	= MAKEINTRESOURCE( IDC_PATHTRACER );
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon( hInstance, MAKEINTRESOURCE( IDI_SMALL ) );

	ATOM wndclass = RegisterClassEx( &wcex );

	PathTracerApplication& application = static_cast< PathTracerApplication& >( Application::GetInstance() );
	application.SetHInstance( hInstance );

	HACCEL table = LoadAccelerators( hInstance, MAKEINTRESOURCE( IDC_PATHTRACER ) );
	if( table != nullptr )
	{
		application.SetAccelTable( table );
	}

	if( !application.Initialize() )
	{
		return -1;
	}

	uint16_t width, height;
	application.GetTracer().GetDimensions( width, height );

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
