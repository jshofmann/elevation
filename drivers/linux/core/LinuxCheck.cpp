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
		eeDebug( "%s: %s returned error %d at %s line %d: %s", caller, function, errno, file, line, strerror( errno ) );

		return false;
	}

	return true;
}
