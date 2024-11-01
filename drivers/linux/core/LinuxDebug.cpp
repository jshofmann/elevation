// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#include "pch.h"

#include <stdarg.h>
#include <stdio.h>

// Note: These functions are declared in ee/core/Debug.h and defined here,
// as they have driver-specific implementations
#include <ee/core/Debug.h>

namespace ee
{
	// Debugf( const char* format, ... )
	//
	// On Linux write debug printouts to the stdout stream.
	//
	//	-> format -- A printf-style format string
	//	-> ...	  -- Any arguments required by format
	void Debugf( const char* format, ... )
	{
		va_list args;

		va_start( args, format );

		vfprintf( stdout, format, args );

		va_end( args );
	}


	// VDebugf( const char* format, va_list args )
	//
	// On Linux write debug printouts to the stdout stream.
	//
	//	-> format -- A printf-style format string
	//	-> args	  -- Any arguments required by format
	void VDebugf( const char* format, va_list args )
	{
		vfprintf( stdout, format, args );
	}

} // namespace ee
