// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

#include <ee/core/PlatformTypes.h>

namespace ee
{
	namespace System
	{
		// These functions are implemented in a platform driver library

		// Platform and Driver are defined in <ee/core/PlatformTypes.h>
		Platform GetPlatform( void );
		Driver GetDriver( void );

		uint32_t GetCoreCount( void );

		bool IsDebuggerAttached( void );

		// Displays an alert box using platform services
		void DisplayAlert( const char* title, const char* message );

		// Display an alert box with yes/no/cancel buttons.
		// Platforms that don't support input will return yes.
		enum { kOptionYes, kOptionNo, kOptionCancel };
		int DisplayAlertWithOptions( const char* title, const char* message );

	} // namespace System

} // namespace ee
