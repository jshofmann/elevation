// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

namespace ee
{
	class System
	{
    public:
		// Platform and Driver are defined in <gfc/base/PlatformTypes.h>
		static Platform GetPlatform( void );
		static Driver GetDriver( void );

		// These are "static virtual" functions implemented in a platform driver
		static uint32_t GetCoreCount( void );

		static bool IsDebuggerAttached( void );

		// Displays an alert box using platform services
		static void DisplayAlert( const char* title, const char* message );

		// Display an alert box with yes/no/cancel buttons.
		// Platforms that don't support input will return yes.
		enum { kOptionYes, kOptionNo, kOptionCancel };
		static int DisplayAlertWithOptions( const char* title, const char* message );

	protected:
		static Platform	mPlatform;
		static Driver	mDriver;

	}; // class System

} // namespace ee
