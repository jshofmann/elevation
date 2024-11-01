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
	static const uint32_t BufferSize = MEM_KB( 2 );

	// Debugf( const char* format, ... )
	//
	// On Windows, wrap OutputDebugString() in a printf-style interface.
	//
	//	-> format -- A printf-style format string
	//	-> ...	  -- Any arguments required by format
	void Debugf( const char* format, ... )
	{
		va_list args;

		va_start( args, format );

		char buffer[ BufferSize ];
		vsnprintf( buffer, BufferSize, format, args );

		OutputDebugString( buffer );

		va_end( args );
	}


	// VDebugf( const char* format, va_list args )
	//
	// On Windows, wrap OutputDebugString() in a vprintf-style interface.
	//
	//	-> format -- A printf-style format string
	//	-> args	  -- Any arguments required by format
	void VDebugf( const char* format, va_list args )
	{
		char buffer[ BufferSize ];
		vsnprintf( buffer, BufferSize, format, args );

		OutputDebugString( buffer );
	}

} // namespace ee
