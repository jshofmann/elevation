// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

#include <stdarg.h>

#if defined( BUILD_RETAIL )

	// Make sure that all debug printouts compile down to nothing in retail builds
	#define eeDebug( x, ... ) (void) 0
	#define veeDebug( x, f )  (void) 0

#else

	// Because these functions are used so much, and they're meant for trivial,
	// ephemeral temporary debug code, provide an alias outside of any namespace
	#define eeDebug  ee::Debugf
	#define veeDebug ee::VDebugf

#endif // if !defined( BUILD_RETAIL )

namespace ee
{
	// These functions are defined in a platform driver
	void Debugf( const char* format, ... );
	void VDebugf( const char* format, va_list args );

} // namespace ee
