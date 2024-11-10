// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

///////////////////////////////////////////////////////////////////////////////
// System headers
//

#if defined( BUILD_PC )

// Configure what we want out of <windows.h>. 
// See https://learn.microsoft.com/en-us/windows/win32/winprog/using-the-windows-header

// See https://msdn.microsoft.com/en-us/library/6sehtctf.aspx and
// https://devblogs.microsoft.com/oldnewthing/20070411-00/?p=27283
// for the history and meaning of these macros. See also <sdkddkver.h>
// in the Windows Platform SDK.
#  undef  _WIN32_WINNT		// The version macro introduced by Windows NT
#  define _WIN32_WINNT		_WIN32_WINNT_WIN10	// Our min spec is Windows 10

#  undef  _WIN32_WINDOWS	// The version macro introduced by Windows 95
#  define _WIN32_WINDOWS	_WIN32_WINNT

#  undef  WINVER			// The original 16-bit Windows version macro, still in use today
#  define WINVER			_WIN32_WINNT

#  undef  _WIN32_IE
#  define _WIN32_IE			_WIN32_IE_IE110 // Internet Explorer 11.0 or newer

// And because some Microsoft engineers wanted to "clean up" the above mess,
// they introduced NTDDI_VERSION - but the Windows SDK still requires the
// above macros. See http://msdn2.microsoft.com/en-us/library/aa383745.aspx
#  undef  NTDDI_VERSION
#  define NTDDI_VERSION		NTDDI_WIN10	// Our min spec is Windows 10 1507 "Threshold"

// Adding more NOxxx #defines makes the #include of <windows.h> compile quicker
// and results in a smaller exe. In the case of NOMINMAX it also fixes a lot
// of compatibility problems with 3rd party headers like OpenEXR.
// See <windows.h> for the full list of available NOxxx tokens.

#  define NOMINMAX					// Don't #define macros named min and max
//#  define NOGDI
#  define NODRAWTEXT
#  define NOBITMAP
#  define NOMCX						// Include <mcx.h> if you need this
#  define NOSERVICE					// Include <winsvc.h> if you need this
#  define NOHELP					// WinHelp is deprecated
#  define NOCOMM					// Exclude the serial communication API

#  define _USE_MATH_DEFINES			// Define M_PI, etc in <math.h>

#  if !defined( _WINSOCKAPI_ )
#    define _WINSOCKAPI_			// Prevent inclusion of winsock.h in windows.h
#    define UNDEF_WINSOCKAPI
#  endif

#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN		// Exclude rarely-used Windows features
#  endif
#  define VC_EXTRALEAN

#  include <windows.h>
#  include <intsafe.h>

#  if defined( UNDEF_WINSOCKAPI )
#    undef _WINSOCKAPI_
#    undef UNDEF_WINSOCKAPI
#  endif

#elif defined( BUILD_XB1 ) || defined( BUILD_XSX )

#  define NOMINMAX				// Define std::min() and std::max() functions rather than min() and max() macros
#  define _USE_MATH_DEFINES		// Define M_PI, etc in <math.h>

#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN	// Exclude rarely-used Windows features
#  endif
#  define VC_EXTRALEAN

#  include <windows.h>

#elif defined( BUILD_LINUX ) || defined( BUILD_NX ) || defined( BUILD_PS4 ) || defined( BUILD_PS5 )

#  define _USE_MATH_DEFINES	// Define M_PI, etc in <math.h>
#  include <stddef.h>		// defines ptrdiff_t, NULL, and other items

#endif // #elif defined( POSIX-based platforms )
