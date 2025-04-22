// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

#include <stdint.h> // We use int8_t, uint32_t, etc

///////////////////////////////////////////////////////////////////////////////
// Our types

namespace ee
{
	// A Platform is the OS the game's running on
	enum class Platform
	{
		kNative		= 0,  // Whatever the host platform is
		kWindows	= 1,  // Windows 10 or newer
		kLinux		= 2,
		kGeneric	= 3, // All platforms

		kPlatformCount
	};

	// Each Platform supports at least one Driver
	enum class Driver
	{
		kNative		= 0,	// Use the best (or only) driver for the Platform
		kDummy		= 1,	// Platform independent null driver
		kVulkan		= 2,	// Windows 7 and newer, Linux, Android
		kDirectX12	= 3,	// Windows 10 and newer, Scarlett (aka Xbox Series S/X)

		kDriverCount
	};

} // namespace ee

//
// Helpers for 64 bit value handling since they're defined in non-standard ways
//
#if defined( COMPILER_MSVC )

// Microsoft has to do their own thing, of course....
#define QUAD( X )		X##i64
#define UQUAD( X )		X##ui64
#define SQUAD( X )		X##i64
#define FMT_QUAD		"%I64d"
#define FMT_QUADU		"%I64u"
#define FMT_HEXQUAD		"%I64x"
#define FMT_HEXQUADU	"%I64x"

#elif defined( EE_BUILD_64BIT )

// A long is 64 bits in 64-bit builds
#define QUAD( X )		X##L
#define UQUAD( X )		X##UL
#define SQUAD( X )		X##L
#define FMT_QUAD		"%ld"
#define FMT_QUADU		"%lu"
#define FMT_HEXQUAD		"%lx"
#define FMT_HEXQUADU	"%lx"

#else // !defined( COMPILER_MSVC ) and !defined( EE_BUILD_64BIT )

// In 32-bit builds a long is 32 bits, so you need to use long long
#define QUAD( X )		X##LL
#define UQUAD( X )		X##ULL
#define SQUAD( X )		X##LL
#define FMT_QUAD		"%lld"
#define FMT_QUADU		"%llu"
#define FMT_HEXQUAD		"%llx"
#define FMT_HEXQUADU	"%llx"

#endif // !COMPILER_MSVC and !EE_BUILD_64BIT

#if !defined( NULL )
#  define NULL 0
#endif
