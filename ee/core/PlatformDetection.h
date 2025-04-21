// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

///////////////////////////////////////////////////////////////////////////////
// OS and compiler detection
//

// Microsoft Visual Studio is being used,
// but don't define COMPILER_MSVC if the clang-cl toolset is being used
#if defined( _MSC_VER )	&& !defined( __clang__ )
#  define COMPILER_MSVC			1
// clang defines __GNUC__ too so check __clang__ first;
// if using the clang-cl toolset in Visual Studio _MSC_VER will also be defined,
// for our purposes we want COMPILER_CLANG not COMPILER_MSVC in that case.
#elif defined( __clang__ )
#  define COMPILER_CLANG		1
#elif defined( __ghs__ )	// Green Hills also defines __GNUC__
#  define COMPILER_GHS			1
#elif defined( __SNC__ )	// SNC may define __GNUC__ too
#  define COMPILER_SNC			1
#elif defined( __GNUC__ )	// GNU GCC
#  define COMPILER_GCC			1
#else
#  error Unsupported compiler
#endif

// Scarlett and Durango also define WIN32 and WIN64 so test for them first
#if defined( _GAMING_XBOX_SCARLETT ) // Xbox Series X (Anaconda) or Xbox Series S (Lockhart)
#  define BUILD_XSX				1
#  define BUILD_X86				1
#  define BUILD_X64				1
#  define BUILD_LITTLE_ENDIAN	1
#elif defined( _XBOX_ONE )		// aka Durango
#  define BUILD_XB1				1
#  define BUILD_X86				1
#  define BUILD_X64				1
#  define BUILD_LITTLE_ENDIAN	1
#elif defined( _WIN32 ) || defined( _WIN64 ) // Windows PC
// Note: BULD_WINDOWS is used by some Windows SDK headers so avoid that name
#  define BUILD_PC				1
#  define BUILD_LITTLE_ENDIAN	1
#  define BUILD_X86				1
#  if defined( COMPILER_MSVC )
#    if defined( _AMD64_ ) || defined( _M_X64 ) || defined( _M_AMD64 )
#      define BUILD_X64			1
#    endif
#  elif defined( COMPILER_GCC )
#    if defined( _X86_64_ ) || defined( __amd64__ )
#      define BUILD_X64			1
#    endif
#  endif
#elif defined( __ORBIS__ )		// Sony Playstation 4
#  define BUILD_PS4				1
#  define BUILD_X86				1
#  define BUILD_X64				1
#  define BUILD_LITTLE_ENDIAN	1
#elif defined( __PROSPERO__ )	// Sony Playstation 5
#  define BUILD_PS5				1
#  define BUILD_X86				1
#  define BUILD_X64				1
#  define BUILD_LITTLE_ENDIAN	1
#elif defined( __linux__ )      // Includes Stadia and SteamOS
#  define BUILD_LINUX			1
#  define BUILD_X86				1
#  if defined( _X86_64_ ) || defined( __amd64__ )
#    define BUILD_X64			1
#  endif
#  define BUILD_LITTLE_ENDIAN	1
#elif defined( __NX__ )			// Nintendo Switch
#  define BUILD_NX				1
#  define BUILD_ARM				1
#  if defined( __ARM_32BIT_STATE )
#    define BUILD_ARM32			1
#  endif
#  if defined( __ARM_64BIT_STATE )
#    define BUILD_ARM64			1
#  endif
#  define BUILD_LITTLE_ENDIAN	1
#else
#  error Unknown platform
#endif

#if defined( BUILD_X64 ) || defined( BUILD_ARM64 )
#  define BUILD_64BIT 1
#else
#  define BUILD_32BIT 1
#endif

#if defined( _DEBUG ) || defined( BUILD_DEVELOP )
#  define BUILD_DEBUG 1
#endif

// Anything that doesn't need to be in the retail build (profiling stuff, debug
// code) should be wrapped in a #if !defined( BUILD_RETAIL ). This is
// automatically defined by the Retail build configuration, this is here
// just for reference.
#if 0
#  define BUILD_RETAIL	1
#endif

// In non-retail builds we enable profiling tools
#if !defined( BUILD_RETAIL )
#  define BUILD_PROFILE 1
#endif

///////////////////////////////////////////////////////////////////////////////
// Compiler configuration
//

// Turn off all deprecated CRT / Windows SDK function warnings
#if defined( BUILD_PC ) || defined( BUILD_XB1 ) || defined( BUILD_XSX )
#  if !defined( _CRT_SECURE_NO_DEPRECATE )
#    define _CRT_SECURE_NO_DEPRECATE 1
#  endif
#  if !defined( _CRT_SECURE_NO_WARNINGS )
#    define _CRT_SECURE_NO_WARNINGS 1
#  endif
#  define _CRT_RAND_S // Define the rand_s function in the CRT
#  if !defined( _WINSOCK_DEPRECATED_NO_WARNINGS )
#    define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#  endif
#endif

#if defined( COMPILER_MSVC )
#  define FORCE_INLINE __forceinline
#elif defined( COMPILER_CLANG ) || defined( COMPILER_GCC )
#  if defined( BUILD_DEBUG )
#    define FORCE_INLINE inline
#  else
#    define FORCE_INLINE __attribute__((always_inline)) inline
#  endif
#else
#  error No FORCE_INLINE definition for this platform
#endif

///////////////////////////////////////////////////////////////////////////////
//
// Because different compilers have substantially different mechanisms 
// for specifying alignment requirements, we have to use a couple of macros
// to cover all cases.  Here's a sample of how to use them:
//
// 	class BEGIN_ALIGN( Matrix3, 16 )
// 	{
//		...
// 	} END_ALIGN( 16 );					<- Before semicolon closing class
//

#if defined( COMPILER_MSVC )
#  define BEGIN_ALIGN( name, a )	__declspec( align(a) ) name
#  define END_ALIGN( a )
#  define MEMBER_ALIGN( name, a )	__declspec( align(a) ) name
#elif defined( COMPILER_CLANG ) || defined( COMPILER_GCC )
#  define BEGIN_ALIGN( name, a )	name
#  define END_ALIGN( a )			__attribute__(( aligned(a) ))
#  define MEMBER_ALIGN( name, a )	name __attribute__(( aligned(a) ))
#else
#  error No alignment macros defined for this platform
#endif

// On the PC, packing is done via #pragma pack( push, 1 ) / #pragma pack( pop )
#if defined( COMPILER_MSVC )
#  define PACKED
#elif defined( COMPILER_CLANG ) || defined( COMPILER_GCC )
#  define PACKED __attribute__(( packed ))
#else
#  error No packed macro defined for this platform
#endif

#if defined( COMPILER_CLANG ) || defined( COMPILER_GCC )
#  define eeMaybeUnused __attribute__(( unused ))
#elif defined( COMPILER_MSVC )
// It would be brilliant if MSVC supported __attribute(( unused )) but they
// decided to wait for C++17's [[ maybe_unused ]]. VS2017 won't warn on
// set-but-unused variables (e.g. stuff calculated for a eeAssert test)
// so having eeMaybeUnused being empty there doesn't introduce any new warns.
#  if _MSVC_LANG >= 201703L // C++17 is supported
#    define eeMaybeUnused [[ maybe_unused ]]
#  else
#    define eeMaybeUnused
#  endif
#else
#  error Determine if this compiler supports [[ maybe_unused ]] or __attribute__(( unused ))
#endif

// Everyone but Microsoft supports __PRETTY_FUNCTION__
#if defined( COMPILER_MSVC )
#  define eeFunctionName __FUNCSIG__
#else
#  define eeFunctionName __PRETTY_FUNCTION__
#endif

///////////////////////////////////////////////////////////////////////////////
// Disable some compiler warnings regarding things that are not problems
// or are benign problems that we will not address. Make these lists
// as short as possible!
//

#if defined( COMPILER_MSVC )

#  pragma warning( disable : 4201 )	// Nameless unions are not ansi but generally well supported
#  pragma warning( disable : 4324 ) // structure was padded due to __declspec(align())
#  pragma warning( disable : 4786 ) // Many symbols resulting from the use of template parameters exceed 255 characters

#endif // #if defined( COMPILER_MSVC )

#if defined( COMPILER_CLANG )

#pragma clang diagnostic ignored "-Wswitch"							// Case not handled in switch statement
#pragma clang diagnostic ignored "-Wunused-parameter"				// Unused function parameter

#endif // #if defined( COMPILER_CLANG )
