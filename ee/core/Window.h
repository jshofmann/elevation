// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

namespace ee
{
	// Window is an abstract interface to a window displayed by the Application.
	// Its implementation is very platform-specific.
	class Window
	{
	public:
		// Pass the dimensions in pixels that you expect to be drawing to here.
		// (e.g. 1920 x 1080 aka 1080p, 3840x2160 aka 4k)
		// On some platforms (e.g. Windows) this is called the client rect.
		virtual void SetSize( uint16_t width, uint16_t height );
		virtual void GetSize( uint16_t& width, uint16_t& height );

		virtual float GetAspectRatio( void ) const;

	protected:
		uint16_t mWidth = 1920, mHeight = 1080; // in pixels

	}; // class Window

	inline void Window::GetSize( uint16_t& width, uint16_t& height )
	{
		width  = mWidth;
		height = mHeight;
	}

	inline float Window::GetAspectRatio( void ) const
	{
		return mWidth / float( mHeight );
	}

} // namespace ee
