// ImageViewer application - part of Elevation Engine
//
// Copyright (c) 2026 Azimuth Studios

#include "pch.h"

#include <cstdint>

#include <ee/core/Debug.h>
#include <ee/io/File.h>
#include <ee/utility/Config.h>
#include <ee/graphics/GpuFormat.h>

#include <drivers/Windows/core/WinApplication.h>
#include <drivers/Windows/core/WinWindow.h>
#include <drivers/Windows/core/WinDebug.h>

#include "resource.h"
#include "ImageViewer.h"

using namespace ee;

class ImageViewerApplication : public WinApplication
{
public:
	// Application interface implementation

	virtual const char* GetName( void ) const override final
	{
		return "ImageViewer";
	}

	virtual bool Initialize( void ) override final
	{
		return Initialize( SW_SHOW );
	}

	virtual void Shutdown( void ) override final;

	virtual Config* GetConfig( void ) noexcept override final
	{
		return &mConfig;
	}

	virtual bool OnStart( void ) override final;
	virtual void OnStop( void ) override final;

	// ImageViewerApplication member functions

	virtual bool Initialize( int nCmdShow );

	ImageViewer& GetViewer( void )
	{
		return mViewer;
	}

	const ImageViewer& GetViewer( void ) const
	{
		return mViewer;
	}

	HBITMAP GetBitmap( void ) const
	{
		return mBitmap;
	}

	void SetBitmap( HBITMAP bitmap )
	{
		mBitmap = bitmap;
	}

	void SetExitCode( int code )
	{
		mExitCode = code;
	}

private:
	static constexpr const char* sConfigName = "ImageViewer.ini";

	ImageViewer mViewer;
	Config		mConfig;
	HBITMAP		mBitmap = NULL;

}; // class ImageViewerApplication

// Because this object represents the running application (ie .exe file)
// a Meyer singleton is an appropriate pattern to use here
Application& Application::GetInstance( void )
{
	static ImageViewerApplication application;
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
//	eeDebug( "ImageViewer WndProc: %s wParam = %d lParam = 0x%08x\n", WinDebug::GetMessageString( message ), wParam, lParam );

	switch( message )
	{
	case WM_CREATE:
	{
		CREATESTRUCT* create = reinterpret_cast< CREATESTRUCT* >( lParam );
		ImageViewerApplication* application = reinterpret_cast< ImageViewerApplication* >( create->lpCreateParams );

		// Store the application pointer in the instance data of the window
		// so it can be retrieved using GetWindowLongPtr( hWnd, GWLP_USERDATA )
		SetWindowLongPtr( hWnd, GWLP_USERDATA, LONG_PTR( application ) );
	}
	break;

	case WM_COMMAND:
	{
		ImageViewerApplication* application =
			reinterpret_cast< ImageViewerApplication* >( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );

		int wmId = LOWORD( wParam );
		// int wmEvent = HIWORD( wParam ); // uncomment if needed

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

		} // switch( wmId )
	}
	break;

	case WM_CLOSE:
	{
		ImageViewerApplication* application =
			reinterpret_cast< ImageViewerApplication* >( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );

		application->Exit();
		DestroyWindow( hWnd );
	}
	break;

	case WM_DESTROY:
	{
		ImageViewerApplication* application =
			reinterpret_cast< ImageViewerApplication* >( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );

		application->Exit();
		PostQuitMessage( 1 );
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

	case WM_QUIT:
	{
		ImageViewerApplication* application =
			reinterpret_cast< ImageViewerApplication* >( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );

		// WM_QUIT's wParam should be returned from WinMain as an int:
		// https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-winmain
		application->SetExitCode( int( wParam ) );
	}
	break;

	case WM_PAINT:
	{
		ImageViewerApplication* application = reinterpret_cast< ImageViewerApplication* >( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );

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

	} // switch( message )

	// default:
	return DefWindowProc( hWnd, message, wParam, lParam );
}

static bool CopyBitmap( ImageViewerApplication& application )
{
	HWND hwnd = application.GetApplicationWindow().GetHWND();

	const ee::Image& image = application.GetViewer().GetImage();

	uint32_t width = image.GetWidth();
	uint32_t height = image.GetHeight();

	uint32_t bytesPerPixel = GetBytesPerPixel( image.GetFormat() );
	if( ( width == 0 ) || ( height == 0 ) || ( bytesPerPixel == 0 ) )
		return false;

	const uint8_t* pixels = image.GetPixels();
	if( pixels == nullptr )
		return false;

	BITMAPINFO info;
	ZeroMemory( &info, sizeof( info ) );

	info.bmiHeader.biSize = sizeof( BITMAPINFOHEADER );
	info.bmiHeader.biWidth = width;
	info.bmiHeader.biHeight = height;
	info.bmiHeader.biPlanes = 1;
	info.bmiHeader.biBitCount = static_cast< WORD >( bytesPerPixel * 8 );
	info.bmiHeader.biCompression = BI_RGB;
	info.bmiHeader.biSizeImage = static_cast< DWORD >( image.GetSizeInBytes() );
	info.bmiHeader.biXPelsPerMeter = 10;
	info.bmiHeader.biYPelsPerMeter = 10;
	info.bmiHeader.biClrUsed = 0;
	info.bmiHeader.biClrImportant = 0;

	info.bmiColors[ 0 ].rgbBlue = 0;
	info.bmiColors[ 0 ].rgbGreen = 0;
	info.bmiColors[ 0 ].rgbRed = 0;
	info.bmiColors[ 0 ].rgbReserved = 0;

	HDC dc = GetDC( hwnd );

	void* bits;
	HBITMAP bitmap = CreateDIBSection( dc, &info, DIB_RGB_COLORS, &bits, nullptr, 0 );

	uint32_t pixelsSize = width * height * bytesPerPixel;
	memcpy( bits, pixels, pixelsSize );

	application.SetBitmap( bitmap );

	ReleaseDC( hwnd, dc );

	return true;
}

bool ImageViewerApplication::Initialize( int nCmdShow )
{
	std::shared_ptr< File > configFile = std::make_shared< File >( sConfigName );
	mConfig.LoadConfig( configFile );

	HACCEL table = LoadAccelerators( mHInstance, MAKEINTRESOURCE( IDC_IMAGEVIEWER ) );
	if( table != nullptr )
	{
		SetAccelTable( table );
	}

	static constexpr size_t MAX_LOADSTRING = 100; // characters

	CHAR szTitle[ MAX_LOADSTRING ];		  // The title bar text
	CHAR szWindowClass[ MAX_LOADSTRING ]; // the main window class name
	LoadString( mHInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING );
	LoadString( mHInstance, IDC_IMAGEVIEWER, szWindowClass, MAX_LOADSTRING );

	WNDCLASSEX wcex;

	wcex.cbSize		   = sizeof( WNDCLASSEX );
	wcex.style		   = CS_HREDRAW | CS_VREDRAW | CS_GLOBALCLASS;
	wcex.lpfnWndProc   = WndProc;
	wcex.cbClsExtra	   = 0;
	wcex.cbWndExtra	   = 0;
	wcex.hInstance	   = mHInstance;
	wcex.hIcon		   = LoadIcon( mHInstance, MAKEINTRESOURCE( IDI_IMAGEVIEWER ) );
	wcex.hCursor	   = LoadCursor( nullptr, IDC_ARROW );
	wcex.hbrBackground = HBRUSH( COLOR_WINDOW + 1 );
	wcex.lpszMenuName  = MAKEINTRESOURCE( IDC_IMAGEVIEWER );
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm	   = LoadIcon( mHInstance, MAKEINTRESOURCE( IDI_SMALL ) );

	/* ATOM wndclass = */ RegisterClassEx( &wcex );

	mApplicationWindow.SetWindowClassName( szWindowClass );
	mApplicationWindow.SetWindowTitle( szTitle );
	mApplicationWindow.SetWindowProc( WndProc );
	mApplicationWindow.SetShowCommand( nCmdShow );

	return true;
}

void ImageViewerApplication::Shutdown( void )
{
//	std::shared_ptr< File > configFile = std::make_shared< File >( sConfigName );
//	mConfig.SaveConfig( configFile );
}

bool ImageViewerApplication::OnStart( void )
{
	if( mViewer.LoadImage( "testorig.jpg" ) )
	{
		uint32_t width, height;
		mViewer.GetDimensions( width, height );

		if( mApplicationWindow.CreateHWND( uint16_t( width ), uint16_t( height ), DisplayMode::kWindowed, this ) )
		{
			// Copy the results to the HBITMAP
			return CopyBitmap( *this );
		}
	}

	return false;
}

void ImageViewerApplication::OnStop( void )
{
	mViewer.Release();
}

int APIENTRY wWinMain( _In_ HINSTANCE	  hInstance,
					   _In_opt_ HINSTANCE hPrevInstance,
					   _In_ LPWSTR		  lpCmdLine,
					   _In_ int			  nCmdShow )
{
	UNREFERENCED_PARAMETER( hPrevInstance );
	UNREFERENCED_PARAMETER( lpCmdLine );

	// Trigger the instantiation of the ImageViewerApplication object
	ImageViewerApplication& application = static_cast< ImageViewerApplication& >( Application::GetInstance() );
	application.SetHInstance( hInstance );

	if( application.Initialize( nCmdShow ) )
	{
		return application.Main( 0, nullptr );
	}

	return 0;
}
