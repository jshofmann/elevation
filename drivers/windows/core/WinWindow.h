#pragma once

#include <ee/core/Application.h>

namespace ee
{
	// Class which can create and/or manage a Windows HWND
	class WinWindow
	{
	public:
		WinWindow();
		~WinWindow();

		// Either use these functions and WinWindow will create the window,
		// or use SetHwnd() below to hand WinWindow an existing window.

		// Note: SetWindowClassName() will not copy the given string,
		// it's the caller's responsibility to do string memory management
		void SetWindowClassName( const char* windowClassName );

		// Note: SetWindowTitle() will not copy the given string,
		// it's the caller's responsibility to do string memory management
		void SetWindowTitle( const char* windowTitle );

		void SetWindowProc( WNDPROC proc );
		WNDPROC GetWindowProc( void ) const;

		// Use RegisterWindowClass if you haven't called RegisterClassEx()
		// yourself. If you don't call SetWindowClassName(), SetWindowTitle(),
		// and/or SetWindowProc() first then it will use "Elevation" and
		// "Elevation Window" as the class name and title respectively
		// and a minimal default wndproc.
		bool RegisterWindowClass( void );

		// windowdata is an optional argument that will be passed to
		// CreateWindowEx as the lpParam argument - which is then passed to the
		// WndProc as WM_CREATE's CREATESTRUCT::lpCreateParams member.
		bool CreateHWND( uint16_t width, uint16_t height, Application::DisplayMode mode,
						 void* windowdata = nullptr );

		void SetWindowSize( uint16_t width, uint16_t height );
		void GetWindowSize( uint16_t& width, uint16_t& height );

		// Or hand an already-existing window to this function
		void SetHWND( HWND hwnd, Application::DisplayMode mode = Application::kDisplayWindowed );
		HWND GetHWND( void ) const;

		void GetClientSize( uint16_t& width, uint16_t& height );

		float GetAspectRatio( void ) const;

	private:
		static LRESULT CALLBACK WindowProc( HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam );

		DWORD GetWindowStyle( Application::DisplayMode mode );

		void AdjustWindowDimensions( uint16_t& width, uint16_t& height );

		const char*	mWindowClassName;
		const char*	mWindowTitle;

		uint16_t	mWindowWidth, mWindowHeight;
		uint16_t	mClientWidth, mClientHeight;
		float		mAspectRatio;
		DWORD		mWindowStyle, mWindowExStyle;
		WNDPROC		mWindowProc;
		HWND		mHwnd;
		bool		mOwnWindow;

	}; // class WinWindow

	inline WNDPROC WinWindow::GetWindowProc( void ) const
	{
		return mWindowProc;
	}

	inline void WinWindow::GetWindowSize( uint16_t& width, uint16_t& height )
	{
		width = mWindowWidth;
		height = mWindowHeight;
	}

	inline void WinWindow::GetClientSize( uint16_t& width, uint16_t& height )
	{
		width = mClientWidth;
		height = mClientHeight;
	}

	inline HWND WinWindow::GetHWND( void ) const
	{
		return mHwnd;
	}

	inline float WinWindow::GetAspectRatio( void ) const
	{
		return mAspectRatio;
	}

} // namespace ee
