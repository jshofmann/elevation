// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#include "pch.h"

#include "WinCheck.h"

#include <drivers/windows/core/WinUtil.h>

using namespace ee;

bool WinCheck::check( HRESULT result, const char* function, const char* caller, const char* file, const unsigned int line )
{
	if( result != S_OK )
	{
		eeDebug( "%s: %s returned error 0x%08x at %s line %d: %s", caller, function, result, file, line, WinUtil::GetErrorString( result ).c_str() );

		return false;
	}

	return true;
}

bool WinCheck::check( BOOL result, const char* function, const char* caller, const char* file, const unsigned int line )
{
	if( result == FALSE )
	{
		DWORD error = GetLastError();
		eeUnusedVariable( error );

		eeDebug( "%s: %s returned error %d at %s line %d: %s", caller, function, error, file, line, WinUtil::GetErrorString( error ).c_str() );

		return false;
	}

	return true;
}

bool WinCheck::checkInt( int result, const char* function, const char* caller, const char* file, const unsigned int line )
{
	if( result == 0 )
	{
		DWORD error = GetLastError();
		eeUnusedVariable( error );

		eeDebug( "%s: %s returned error 0x%08x at %s line %d: %s", caller, function, error, file, line, WinUtil::GetErrorString( error ).c_str() );

		return false;
	}

	return true;
}
