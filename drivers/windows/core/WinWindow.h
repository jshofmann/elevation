// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

#include <string>

#include <ee/core/Window.h>
#include <ee/graphics/Enums.h>

namespace ee
{
	// Class which can create and/or manage a Windows HWND
	class WinWindow : public Window
	{
	public:
		// Either use these functions and WinWindow will create the window,
		// or use SetHwnd() below to hand WinWindow an existing window.

		// Window interface overrides

		// In WinWindow, GetSize() / SetSize() control the window's client rect.
		// If you need the window's window rect use GetWindowRect() below.
		// width and height should be expressed in pixels.
		virtual void SetSize( uint16_t width, uint16_t height ) override final;

		// WinWindow member functions

		// Note: SetWindowTitle() and SetWindowClassName() will make copies
		// of the input strings.
		void SetWindowTitle( const char* windowTitle );
		void SetWindowClassName( const char* windowClassName );

		void	SetWindowProc( WNDPROC proc );
		WNDPROC GetWindowProc( void ) const;

		// command must be one of the SW_* values accepted by
		// ShowWindow()'s nCmdShow parameter.
		void	SetShowCommand( int command );
		int		GetShowCommand( void ) const;

		// Use RegisterWindowClass if you haven't called RegisterClassEx()
		// yourself. If you don't call SetWindowClassName(), SetWindowTitle(),
		// and/or SetWindowProc() first then it will use "Elevation" and
		// "Elevation Window" as the class name and title respectively
		// and a minimal default wndproc.
		bool RegisterWindowClass( void );

		// windowdata is an optional argument that will be passed to
		// CreateWindowEx as the lpParam argument - which is then passed to the
		// WndProc as WM_CREATE's CREATESTRUCT::lpCreateParams member.
		bool CreateHWND( uint16_t width, uint16_t height, DisplayMode mode, void* windowdata = nullptr );

		// Or hand an already-existing window to this function
		void SetHWND( HWND hwnd, DisplayMode mode = DisplayMode::kWindowed );
		HWND GetHWND( void ) const;

		// Window::GetSize() will return the window's client rect.
		// Use GetWindowSize() if you need the window's window rect
		// (which includes the title bar, borders, etc.)
		void GetWindowSize( uint16_t& width, uint16_t& height ); // in pixels

	private:
		static LRESULT CALLBACK WindowProc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam );

		DWORD GetWindowStyle( DisplayMode mode );

		void AdjustWindowDimensions( uint16_t& width, uint16_t& height );

		std::string mWindowTitle = "Elevation Window";
		std::string mWindowClassName = "Elevation";

		uint16_t mWindowWidth = 1920, mWindowHeight = 1080; // in pixels
		DWORD	 mWindowStyle	= WS_VISIBLE | WS_POPUP; // assume fullscreen display
		DWORD	 mWindowExStyle = 0;
		WNDPROC	 mWindowProc	= WinWindow::WindowProc;
		int		 mShowCommand	= SW_SHOW;
		HWND	 mHwnd			= nullptr;
		bool	 mOwnWindow		= false;

	}; // class WinWindow

	inline WNDPROC WinWindow::GetWindowProc( void ) const
	{
		return mWindowProc;
	}

	inline HWND WinWindow::GetHWND( void ) const
	{
		return mHwnd;
	}

	inline void WinWindow::GetWindowSize( uint16_t& width, uint16_t& height )
	{
		width  = mWindowWidth;
		height = mWindowHeight;
	}

} // namespace ee
