// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#include "pch.h"

#include <dxerr.h>

#include "WinCheck.h"

#include <drivers/windows/core/WinUtil.h>

using namespace ee;

bool WinCheck::check( HRESULT result, const char* function, const char* caller,
					  const char* file, const unsigned int line )
{
	if( result != S_OK )
	{
		// Format the message so that double-clicking on it in Visual Studio's
		// Output pane will navigate to the location of the failure report.
		eeDebug( "%s(%d): %s: %s returned error %s (0x%08x): %s", file, line,
				 caller, function, DXGetErrorString( result ), result,
				 WinUtil::GetErrorString( result ).c_str() );

		return false;
	}

	return true;
}

bool WinCheck::check( BOOL result, const char* function, const char* caller,
					  const char* file, const unsigned int line )
{
	if( result == FALSE )
	{
		DWORD error = GetLastError();
		eeUnusedVariable( error );

		eeDebug( "%s(%d): %s: %s returned error %s (%d): %s", file, line,
				 caller, function, DXGetErrorString( error ), error,
				 WinUtil::GetErrorString( error ).c_str() );

		return false;
	}

	return true;
}

bool WinCheck::checkInt( int result, const char* function, const char* caller,
						 const char* file, const unsigned int line )
{
	if( result == 0 )
	{
		DWORD error = GetLastError();
		eeUnusedVariable( error );

		eeDebug( "%s(%d): %s: %s returned error %s (0x%08x): %s", file, line,
				 caller, function, DXGetErrorString( error ), error,
				 WinUtil::GetErrorString( error ).c_str() );

		return false;
	}

	return true;
}
