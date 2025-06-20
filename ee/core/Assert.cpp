// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#include "pch.h"

#include <stdio.h>
#include <stdarg.h>

#include <ee/core/System.h>
#include <ee/core/Assert.h>

namespace ee {

bool gInFatalError = false;

ErrorResult ee::_Assert( const char* function, const char* file, int line, const char* format, ... )
{
	char buffer[ 1024 ];
	va_list args;
	va_start( args, format );
	vsnprintf( buffer, sizeof( buffer ), format, args );
	va_end( args );

	eeDebug( "Assert: function %s file %s line %d %s\n", function, file, line, buffer );

#if !defined( EE_BUILD_DEBUG )

	// Release just logs the error and returns ignore
	return ErrorResult::kIgnore;

#else

	// Debug can get popups
	ErrorResult result = ErrorResult::kBreak;

	// DisplayAlertWithOptions() will cause the message loop to get pumped,
	// which could cause us to trigger another assert and get into an infinite
	// loop. So, this part of the code is not reentrant.
	static bool inAlert = false;
	if( !inAlert )
	{
		inAlert = true;

		// Get the name of the exe, without the path.
		char processFilename[ MAX_PATH ] = "";
		GetModuleFileName( NULL, processFilename, sizeof( processFilename ) );
		const char* lastSlash = strrchr( processFilename, '\\' );
		if( lastSlash != NULL )
		{
			strcpy_s( processFilename, MAX_PATH, lastSlash + 1 );
		}

		DWORD processID = GetCurrentProcessId();

		char title[ 256 ];
		snprintf( title, sizeof( title ), "ASSERT %s pid %u", processFilename, processID );

		char buffer[ 512 ];
		snprintf( buffer, sizeof( buffer ),
				  "Function: %s\n\nFile: %s\nLine: %d\n\n%s\n\n"
				  "\"Yes\" to break, \"No\" to continue, or \"Cancel\" to ignore for the rest of the run.",
				  function, file, line, buffer );

		int option = System::DisplayAlertWithOptions( title, buffer );

		if( option == System::kOptionYes )
			result = ErrorResult::kBreak;
		else if( option == System::kOptionNo )
			result = ErrorResult::kContinue;
		else
			result = ErrorResult::kIgnore;

		inAlert = false;
	}
	else
	{
		// Just continue on any skipped asserts, otherwise you can get into
		// a situation where there's a message box popped up for an assert
		// that you can't click, with forced breakpoints popping up over
		// and over behind it.
		result = ErrorResult::kContinue;
	}

	return result;

#endif // #if !defined( EE_BUILD_DEBUG ) #else
}

void ee::_FatalError( const char* function, const char* file, int line, const char* format, ... )
{
	gInFatalError = true;

	va_list args;
	va_start( args, format );

	char message[ 1024 ];
	vsnprintf( message, sizeof( message ), format, args );

	va_end( args );

	eeDebug( "FATAL ERROR: Function: %s\n\nFile: %s\nLine: %d\n\nError: %s\n",
			 function, file, line, message );

	_SilentFatalError();
}

void ee::_SilentFatalError( void )
{
	gInFatalError = true;
	int* crash = NULL;
	*crash = 0;
}

} // namespace ee
