// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

#if defined( __APPLE__ )
#include <TargetConditionals.h>
#endif

///////////////////////////////////////////////////////////////////////////////
// Compiler detection

// Microsoft Visual Studio is being used,
// but don't define EE_COMPILER_MSVC if the clang-cl toolset is being used
#if defined( _MSC_VER ) && !defined( __clang__ )
#  define EE_COMPILER_MSVC			1
// clang defines __GNUC__ too so check __clang__ first;
// if using the clang-cl toolset in Visual Studio _MSC_VER will also be defined,
// for our purposes we want EE_COMPILER_CLANG not EE_COMPILER_MSVC in that case.
#elif defined( __clang__ )
#  define EE_COMPILER_CLANG			1
#elif defined( __GNUC__ )	// GNU GCC
#  define EE_COMPILER_GCC			1
#else
#  error Unsupported compiler
#endif

///////////////////////////////////////////////////////////////////////////////
// CPU architecture detection

#if defined(__i386__) || defined(_M_IX86)
#  define EE_BUILD_X86			1
#elif defined( __x86_64__ ) || defined( _M_X64 ) || defined( __amd64__ ) || defined( _M_AMD64 )
#  define EE_BUILD_X64			1
#elif defined( __arm__ ) || defined( _M_ARM )
#  define EE_BUILD_ARM			1
#  define EE_BUILD_ARM32		1
#elif defined( __aarch64__ ) || defined( _M_ARM64 )
#  define EE_BUILD_ARM			1
#  define EE_BUILD_ARM64		1
#elif defined( __wasm32__ )
#  define EE_BUILD_WASM			1
#  define EE_BUILD_WASM32		1
#elif defined( __wasm64__ )
#  define EE_BUILD_WASM			1
#  define EE_BUILD_WASM64		1
#else
#  error Unsupported CPU architecture
#endif

///////////////////////////////////////////////////////////////////////////////
// OS detection

// Scarlett and Durango also define WIN32 and WIN64 so test for them first
#if defined( _GAMING_XBOX_SCARLETT )	// Xbox Series X (Anaconda) or Xbox Series S (Lockhart)
#  define EE_BUILD_XSX				1
#elif defined( _XBOX_ONE )				// aka Durango
#  define EE_BUILD_XB1				1
#elif defined( _WIN32 ) || defined( _WIN64 ) // Windows PC
// Note: BUILD_WINDOWS is used by some Windows SDK headers so avoid that name
#  define EE_BUILD_WINDOWS			1
#elif defined( __ORBIS__ )		// Sony Playstation 4
#  define EE_BUILD_PS4				1
#  define EE_BUILD_POSIX			1
#elif defined( __PROSPERO__ )	// Sony Playstation 5
#  define EE_BUILD_PS5				1
#  define EE_BUILD_POSIX			1
#elif defined( __NX__ )			// Nintendo Switch
#  define EE_BUILD_NX				1
#elif defined( __APPLE__ )
#  define EE_BUILD_APPLE			1
#  define EE_BUILD_POSIX			1
#  include <TargetConditionals.h>
#  if TARGET_OS_IPHONE			// TARGET_OS_IPHONE also defines TARGET_OS_MAC
#    define EE_BUILD_IOS			1
#  elif TARGET_OS_MAC
#    define EE_BUILD_MACOS			1
#  else
#    error Unsupported Apple platform
#  endif
#  if TARGET_CPU_X86_64
#    define EE_BUILD_X64			1
#  elif TARGET_CPU_ARM64
#    define EE_BUILD_ARM			1
#    define EE_BUILD_ARM64			1
#  endif
#elif defined( __EMSCRIPTEN__ )
#  define EE_BUILD_EMSCRIPTEN		1
#  define EE_BUILD_POSIX			1
#elif defined( __ANDROID__ )	// Android also defines __linux__
#  define EE_BUILD_ANDROID			1
#  define EE_BUILD_POSIX			1
#elif defined( __linux__ )      // Includes Stadia and SteamOS
#  define EE_BUILD_LINUX			1
#  define EE_BUILD_POSIX			1
#else
#  error Unsupported platform
#endif

#if defined( EE_BUILD_X64 ) || defined( EE_BUILD_ARM64 ) || defined( EE_BUILD_WASM64 )
#  define EE_BUILD_64BIT 1
static_assert( sizeof( sizeof( char ) ) == 8, "Expected sizeof( size_t ) == 8" );
#else
#  define EE_BUILD_32BIT 1
static_assert( sizeof( sizeof( char ) ) == 4, "Expected sizeof( size_t ) == 4" );
#endif

#if defined( _DEBUG ) || defined( EE_BUILD_DEVELOP )
#  define EE_BUILD_DEBUG 1
#endif

// Anything that doesn't need to be in the retail build (profiling stuff, debug
// code) should be wrapped in a #if !defined( EE_BUILD_RETAIL ). This is
// automatically defined by the Retail build configuration, this is here
// just for reference.
#if 0
#  define EE_BUILD_RETAIL 1
#endif

// In non-retail builds we enable profiling tools
#if !defined( EE_BUILD_RETAIL )
#  define EE_BUILD_PROFILE 1
#endif

///////////////////////////////////////////////////////////////////////////////
// Compiler configuration

// Turn off some deprecated CRT / Windows SDK function warnings
#if defined( EE_BUILD_WINDOWS ) || defined( EE_BUILD_XB1 ) || defined( EE_BUILD_XSX )
#  if !defined( _CRT_SECURE_NO_WARNINGS )
#    define _CRT_SECURE_NO_WARNINGS 1
#  endif
#  if !defined( _WINSOCK_DEPRECATED_NO_WARNINGS )
#    define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#  endif
#  define _USE_MATH_DEFINES // Define M_PI, etc in <math.h>
#  define _CRT_RAND_S // Define the rand_s function in the CRT
#endif

#if defined( EE_COMPILER_MSVC )
#  define FORCE_INLINE __forceinline
#elif defined( EE_COMPILER_CLANG ) || defined( EE_COMPILER_GCC )
#  if defined( EE_BUILD_DEBUG )
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

#if defined( EE_COMPILER_MSVC )
#  define BEGIN_ALIGN( name, a )	__declspec( align(a) ) name
#  define END_ALIGN( a )
#  define MEMBER_ALIGN( name, a )	__declspec( align(a) ) name
#elif defined( EE_COMPILER_CLANG ) || defined( EE_COMPILER_GCC )
#  define BEGIN_ALIGN( name, a )	name
#  define END_ALIGN( a )			__attribute__(( aligned(a) ))
#  define MEMBER_ALIGN( name, a )	name __attribute__(( aligned(a) ))
#else
#  error No alignment macros defined for this platform
#endif

// On the PC, packing is done via #pragma pack( push, 1 ) / #pragma pack( pop )
#if defined( EE_COMPILER_MSVC )
#  define PACKED
#elif defined( EE_COMPILER_CLANG ) || defined( EE_COMPILER_GCC )
#  define PACKED __attribute__(( packed ))
#else
#  error No packed macro defined for this platform
#endif

// Everyone but Microsoft supports __PRETTY_FUNCTION__
#if defined( EE_COMPILER_MSVC )
#  define eeFunctionName __FUNCSIG__
#else
#  define eeFunctionName __PRETTY_FUNCTION__
#endif

// Branch prediction hints

// Use LIKELY to tell the branch predictor that the expression is expected
// to be true, allows expression to be false with a slight performance penalty
#if defined( EE_COMPILER_CLANG ) || defined( EE_CCOMPILER_GCC )
	#define LIKELY( x )			__builtin_expect( !!(x), 1 )
#else
	// the additional "!!" is used to silence "warning: equality comparison
	// with extraneous parentheses" messages on android
	#define LIKELY( x )			( !!(x) )
#endif

// Use UNLIKELY to tell the branch predictor that the expression is expected
// to be false, allows expression to be true with a slight performance penalty
#if defined( EE_COMPILER_CLANG ) || defined( EE_CCOMPILER_GCC )
	#define UNLIKELY( x )		__builtin_expect( !!(x), 0 )
#else
	// the additional "!!" is used to silence "warning: equality comparison
	// with extraneous parentheses" messages on android
	#define UNLIKELY( x )		( !!(x) )
#endif

///////////////////////////////////////////////////////////////////////////////
// Disable some compiler warnings regarding things that are not problems
// or are benign problems that we will not address. Make these lists
// as short as possible!

#if defined( EE_COMPILER_MSVC )

#  pragma warning( disable : 4100 )	// We don't always use all function parameters
#  pragma warning( disable : 4201 )	// Nameless unions are not ansi but generally well supported
#  pragma warning( disable : 4324 ) // structure was padded due to __declspec(align())
#  pragma warning( disable : 4786 ) // Many symbols resulting from the use of template parameters exceed 255 characters

#endif // #if defined( EE_COMPILER_MSVC )

#if defined( EE_COMPILER_CLANG )

#pragma clang diagnostic ignored "-Wswitch"							// Case not handled in switch statement
#pragma clang diagnostic ignored "-Wunused-parameter"				// Unused function parameter

#endif // #if defined( EE_COMPILER_CLANG )
