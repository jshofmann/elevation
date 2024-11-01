// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

#include "GlobalSettings.h"

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

		// Displays an alert box using platform services.
		static void DisplayAlert( const char* title, const char* message );

		// Display an alert box with yes/no/cancel buttons.
		// Platforms that don't support input will return yes.
		enum { kOptionYes, kOptionNo, kOptionCancel };
		static int DisplayAlertWithOptions( const char* title, const char* message );

		static Flags GetGlobalFlags( void ) { return mGlobalFlags; }

	protected:
		static Platform	mPlatform;
		static Driver	mDriver;

		static Flags    mGlobalFlags;
	};

	#define eeGetGlobalFlag( f )		System::GetGlobalFlags().GetFlag( f )
	#define eeSetGlobalFlag( f, val )	System::GetGlobalFlags().SetFlag( f, val )
	#define eeToggleGlobalFlag( f )		System::GetGlobalFlags().ToggleFlag( f )

} // namespace ee
