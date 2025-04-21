// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

#include <ee/core/PlatformTypes.h>

namespace ee
{
	namespace System
	{
		// These functions are implemented in a platform driver library,
		// unless they're constexpr - if they're constexpr then C++ requires
		// that they be implemented here, or that we #include the platform
		// driver implementation file here. Declaring them here and implementing
		// them in a platform driver not #included here results in 'expression
		// did not evaluate to a constant' build errors in VS2022. :swear:

		// Platform and Driver are defined in <ee/core/PlatformTypes.h>
		constexpr Platform GetPlatform( void ) noexcept
		{
#if defined( BUILD_PC )
			return Platform::kWindows;
#elif defined( BUILD_LINUX )
			return Platform::kLinux;
#else
			return Platform::kGeneric;
#endif
		}

		Driver GetDriver( void );

		uint32_t GetCoreCount( void );

		// IsDebuggerAttached() covers tools like Visual Studio
		bool IsDebuggerAttached( void );
		// while IsGPUDebuggerAttached() is for stuff like RenderDoc and PIX
		bool IsGPUDebuggerAttached( void );

		// Displays an alert box using platform services
		void DisplayAlert( const char* title, const char* message );

		// Display an alert box with yes/no/cancel buttons.
		// Platforms that don't support input will return yes.
		enum { kOptionYes, kOptionNo, kOptionCancel };
		int DisplayAlertWithOptions( const char* title, const char* message );

	} // namespace System

} // namespace ee
