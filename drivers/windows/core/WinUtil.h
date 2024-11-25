// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

#include <string>

#include <ee/core/Platform.h>

namespace ee
{
	namespace WinUtil
	{
		RECT GetWindowDimensions( HWND hwnd, uint16_t& width, uint16_t& height );

		bool IsPIXAttached( void );
		bool IsRenderDocAttached( void );
		bool IsGPUDebuggerAttached( void );

		// You will usually want to pass in the results of GetLastError() here
		std::string GetErrorString( DWORD error );

	} // namespace WinUtil

} // namespace ee
