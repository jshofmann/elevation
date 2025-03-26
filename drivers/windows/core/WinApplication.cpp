#include "pch.h"

#include "WinApplication.h"

using namespace ee;

//*****************************************************************************
//*****************************************************************************

WinApplication& WinApplication::GetInstance( void )
{
	return static_cast< WinApplication& >( Application::GetInstance() );
}

//*****************************************************************************
//*****************************************************************************

void WinApplication::Exit( void )
{
	mRunning = false;
}

bool WinApplication::Update( void )
{
	HWND hwnd = mApplicationWindow.GetHWND();
	if( hwnd == NULL )
		return mRunning;

	// Implement a basic message pump as the default option
	MSG msg;
	while( PeekMessage( &msg, hwnd, 0, 0, PM_NOREMOVE ) )
	{
		// !?!? The BOOL that GetMessage() returns can be
		// TRUE, FALSE, or -1. Go Microsoft!
		BOOL hasMessage = GetMessage( &msg, hwnd, 0, 0 );
		if( hasMessage != FALSE )
		{
			if( hasMessage == -1 )
			{
				// log this error
				eeDebug( "WinApplication::update: The Windows API call GetMessage() has returned -1!" );
				mRunning = false;
				return false;
			}
			else
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
				
			}
		}

		if( mAccelTable == NULL || !TranslateAccelerator( msg.hwnd, mAccelTable, &msg ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

	} // while( PeekMessage( &msg, hwnd, 0, 0, PM_NOREMOVE ) )

	return mRunning;
}

void WinApplication::SetHInstance( HINSTANCE hinstance )
{
	mHInstance = hinstance;
}

void WinApplication::SetParentWindow( HWND hwnd )
{
	mParentWindow = hwnd;
}

void WinApplication::SetAccelTable( HACCEL table )
{
	mAccelTable = table;
}
