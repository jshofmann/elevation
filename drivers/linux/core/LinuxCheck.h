// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

// A group of functions to make testing POSIX API result values easy and clean;
// any error results will be pretty-printed to the log system. For example:
//
// if( eeCheck( SomePOSIXFunction( ... ) ) )
// {
//     ... we can assume SomePOSIXFunction suceeded here ...
// }
namespace ee
{
	class LinuxCheck
	{
    public:
		static bool check( int result, const char* function, const char* caller, const char* file, const unsigned int line );

	}; // class LinuxCheck

} // namespace ee

#if !defined( BUILD_RETAIL )

#define eeCheck( r ) ee::LinuxCheck::check( r, #r, __FUNCTION__, __FILE__, __LINE__ )

#else

// The expression that evaluates to an int must be executed in any case...
#define eeCheck( r ) ( (r) != -1 )

#endif
