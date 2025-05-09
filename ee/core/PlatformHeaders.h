// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

///////////////////////////////////////////////////////////////////////////////
// System headers
//

#if defined( EE_BUILD_WINDOWS )

// Configure what we want out of <windows.h>. 
// See https://learn.microsoft.com/en-us/windows/win32/winprog/using-the-windows-headers

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

#  define NOGDICAPMASKS				// CC_*, LC_*, PC_*, CP_*, TC_*, RC_
#  define NOVIRTUALKEYCODES			// VK_*
//#  define NOWINMESSAGES			// WM_*, EM_*, LB_*, CB_*
//#  define NOWINSTYLES				// WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
//#  define NOSYSMETRICS			// SM_*
#  define NOMENUS					// MF_*
//#  define NOICONS					// IDI_*
#  define NOKEYSTATES				// MK_*
#  define NOSYSCOMMANDS				// SC_*
//#  define NORASTEROPS				// Binary and Tertiary raster ops
#  define NOATOM					// Atom Manager routines
#  define NOCLIPBOARD				// Clipboard routines
//#  define NOCOLOR					// Screen colors
//#  define NOCTLMGR				// Control and Dialog routines
#  define NODRAWTEXT				// DrawText() and DT_*
//#  define NOGDI					// All GDI defines and routines
#  define NOKERNEL					// All KERNEL defines and routines
//#  define NOUSER					// All USER defines and routines
#  define NONLS						// All NLS defines and routines
#  define NOMEMMGR					// GMEM_*, LMEM_*, GHND, LHND, associated routines
#  define NOMETAFILE				// typedef METAFILEPICT
#  define NOMINMAX					// Macros min(a,b) and max(a,b)
//#  define NOMSG					// typedef MSG and associated routines
#  define NOOPENFILE				// OpenFile(), OemToAnsi, AnsiToOem, and OF_*
#  define NOSCROLL					// SB_* and scrolling routines
#  define NOSERVICE					// All Service Controller routines, SERVICE_ equates, etc.
#  define NOSOUND					// Sound driver routines
#  define NOTEXTMETRIC				// typedef TEXTMETRIC and associated routines
#  define NOWH						// SetWindowsHook and WH_*
//#  define NOWINOFFSETS			// GWL_*, GCL_*, associated routines
#  define NOCOMM					// Exclude the serial communication API
#  define NOKANJI					// Kanji support stuff.
#  define NOHELP					// WinHelp is deprecated
#  define NOPROFILER				// Profiler interface
#  define NODEFERWINDOWPOS			// DeferWindowPos routines
#  define NOMCX						// Modem Configuration Extensions

#  define _USE_MATH_DEFINES			// Define M_PI, etc in <math.h>

#  if !defined( _WINSOCKAPI_ )
#    define _WINSOCKAPI_			// Prevent inclusion of winsock.h in windows.h
#    define UNDEF_WINSOCKAPI
#  endif

#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN		// Exclude rarely-used Windows features
#  endif
#  define VC_EXTRALEAN				// Used by MFC; has no effect on <windows.h>

#  include <windows.h>

#  if defined( UNDEF_WINSOCKAPI )
#    undef _WINSOCKAPI_
#    undef UNDEF_WINSOCKAPI
#  endif

// Define the threading primitives for the host platform
#  include <drivers/windows/core/WinThread.h>

#elif defined( EE_BUILD_XB1 ) || defined( EE_BUILD_XSX )

#  define NOMINMAX				// Define std::min() and std::max() functions rather than min() and max() macros
#  define _USE_MATH_DEFINES		// Define M_PI, etc in <math.h>

#  ifndef WIN32_LEAN_AND_MEAN
#    define WIN32_LEAN_AND_MEAN	// Exclude rarely-used Windows features
#  endif
#  define VC_EXTRALEAN

#  include <windows.h>

// Define the threading primitives for the host platform
#  include <drivers/windows/core/WinThread.h>

#elif defined( EE_BUILD_LINUX ) || defined( EE_BUILD_NX ) || defined( EE_BUILD_PS4 ) || defined( EE_BUILD_PS5 )

#  define _USE_MATH_DEFINES	// Define M_PI, etc in <math.h>
#  include <stddef.h>		// defines ptrdiff_t, NULL, and other items

// Define the threading primitives for the host platform
#  include <drivers/posix/core/PosixThread.h>

#endif // #elif defined( POSIX-based platforms )
