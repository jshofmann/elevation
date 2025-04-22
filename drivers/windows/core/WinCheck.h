// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

// A group of functions to make testing Win32 or DirectX API result values
// easy and clean; any error results will be pretty-printed to the log system.
// For example:
//
// if( eeCheck( SomeWindowsAPIFunction( ... ) ) )
// {
//     ... we can assume SomeWindowsAPIFunction suceeded here ...
// }
namespace ee
{
	namespace WinCheck
	{
		bool check( HRESULT result, const char* function, const char* caller, const char* file, const unsigned int line );
		bool check( BOOL result, const char* function, const char* caller, const char* file, const unsigned int line );
		bool checkInt( int result, const char* function, const char* caller, const char* file, const unsigned int line );

	} // class WinCheck

} // namespace ee

#if !defined( EE_BUILD_RETAIL )

// Check HRESULT result values; fails if r != S_OK (0)
#define eeCheck( r ) ee::WinCheck::check( r, #r, __FUNCTION__, __FILE__, __LINE__ )

// Check BOOL result values; fails if r != TRUE (1)
#define eeCheckBool( r ) ee::WinCheck::check( r, #r, __FUNCTION__, __FILE__, __LINE__ )

// Check int result values; fails if r == 0
#define eeCheckInt( r ) ee::WinCheck::checkInt( r, #r, __FUNCTION__, __FILE__, __LINE__ )

#else // #if defined( EE_BUILD_RETAIL )

// The r expression be executed in all builds!
#define eeCheck( r ) ( (r) == S_OK )
#define eeCheckBool( r ) ( (r) == TRUE )
#define eeCheckInt( r ) ( (r) != 0 )

#endif // #if defined( EE_BUILD_RETAIL )
