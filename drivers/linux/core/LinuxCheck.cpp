// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#include "pch.h"

#include <string.h>
#include <errno.h>

#include "LinuxCheck.h"

using namespace ee;

bool LinuxCheck::check( int result, const char* function, const char* caller, const char* file, const unsigned int line )
{
	if( result == -1 )
	{
		// Format the message so that double-clicking on it in Visual Studio's
		// Output pane will navigate to the location of the failure report.
		eeDebug( "%d(%s): %s: %s returned error %d: %s", file, line, caller, function, errno, strerror( errno ) );

		return false;
	}

	return true;
}
