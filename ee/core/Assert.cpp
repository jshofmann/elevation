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

static void _DisplayError( const char* title, const char* message, const char* function, const char* file, int lineNumber )
{
	char buffer[ 1024 ];
	snprintf( buffer, sizeof( buffer ),
			  "Function: %s\n\nFile: %s\nLine: %d\n\nError: %s\n",
			  function, file, lineNumber, message );

	eeDebug( title, buffer );
}

void ee::_ShowError( const char* function, const char* file, int line, const char* format, ... )
{
	char buffer[ 1024 ];
	va_list args;
	va_start( args, format );
	vsnprintf( buffer, sizeof( buffer ), format, args );
	va_end( args );

	_DisplayError( "ERROR", buffer, function, file, line );

//	eeLog( kAssert, LV_ERROR, "ERROR: function %s file %s line %d %s", function, file, line, buffer );
}

int ee::_Assert( const char* function, const char* file, int line, const char* format, ... )
{
	char buffer[ 1024 ];
	va_list args;
	va_start( args, format );
	vsnprintf( buffer, sizeof( buffer ), format, args );
	va_end( args );

//	eeLog( kAssert, LV_ERROR, "Assert: function %s file %s line %d %s", function, file, line, buffer );

#if !defined( EE_BUILD_DEBUG )

	// Release logs the error and returns ignore
	return ErrorResult::kIgnore;

#else

	// Debug can get popups
	int result = ErrorResult::kBreak;

	static bool inMessage = false;

	// logWithOptions will cause the message loop to get pumped,
	// which could cause us to trigger another assert and get into an infinite
	// loop. So, this part of the code is not reentrant.
	if( !inMessage )
	{
		inMessage = true;

		char title[ 512 ];
		DWORD processID = GetCurrentProcessId();

		// Get the name of the exe, without the path.
		char processFilename[ MAX_PATH ] = "";
		GetModuleFileName( NULL, processFilename, sizeof( processFilename ) );
		const char* lastSlash = strrchr( processFilename, '\\' );
		if( lastSlash != NULL )
		{
			strcpy_s( processFilename, MAX_PATH, lastSlash + 1 );
		}
		snprintf( title, 512, "ASSERT %s pid %u", processFilename, processID );

		char message[ 1024 ];
		snprintf( message, sizeof( message ),
				  "Function: %s\n\nFile: %s\nLine: %d\n\n%s\n\n"
				  "\"Yes\" to break, \"No\" to continue, or \"Cancel\" to ignore for the rest of the run.",
				  function, file, line, buffer );

		int option = System::DisplayAlertWithOptions( title, message );

		if( option == System::kOptionYes )
			result = ErrorResult::kBreak;
		else if( option == System::kOptionNo )
			result = ErrorResult::kContinue;
		else
			result = ErrorResult::kIgnore;

		inMessage = false;
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

void ee::_SilentAssert( const char* function, const char* file, int line, const char* format, ... )
{
	char buffer[ 1024 ];
	va_list args;
	va_start( args, format );
	vsnprintf( buffer, sizeof( buffer ), format, args );
	va_end( args );

	eeDebug( "Assert: function %s file %s line %d %s", function, file, line, buffer );
}

void ee::_FatalError( const char* function, const char* file, int line, const char* format, ... )
{
	gInFatalError = true;

	char buffer[ 1024 ];
	va_list args;
	va_start( args, format );
	vsnprintf( buffer, sizeof( buffer ), format, args );
	va_end( args );

	_DisplayError( "FATAL ERROR", buffer, function, file, line );
	eeDebug( "FATAL ERROR: file %s line %d %s", file, line, buffer );

	_SilentFatalError();
}

void ee::_SilentFatalError( void )
{
	gInFatalError = true;
	int* crash = NULL;
	*crash = 0;
}

} // namespace ee
