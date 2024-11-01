// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

#include <ee/core/Platform.h>
#include <string>

namespace ee
{
	namespace WinUtil
	{
		RECT getWindowDimensions( HWND hwnd, uint16_t& width, uint16_t& height );

		bool isPIXAttached( void );
		bool isRenderDocAttached( void );
		bool isGPUDebuggerAttached( void );

		// You will usually want to pass in the results of GetLastError() here
		std::string getErrorString( DWORD error );

	} // namespace WinUtil

} // namespace ee
