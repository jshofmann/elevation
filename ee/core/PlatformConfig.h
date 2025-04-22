// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

// Define this to enable eeDebug and eeLog printouts in Retail builds
// (Note that eeLog just redirects to eeDebug in this configuration,
// no log files are created because in the Retail configuration the
// game's frequently operating from a read-only file system.)
//#define EE_BUILD_ENABLE_EEDEBUG_IN_RETAIL

///////////////////////////////////////////////////////////////////////////////
// Windows configuration
//

#if defined( EE_BUILD_WINDOWS )

#  define DIRECTINPUT_VERSION 0x0800

#  if defined( EE_BUILD_PROFILE )

//   RAD's Telemetry is available for multiple platforms,
//   but we only have a license for Windows
//#    define EE_BUILD_RAD_TELEMETRY 1

//   Enable this to include support for PureDev's MemPro memory profiler
//   (http://www.puredevsoftware.com/mempro/index.htm)
//#    define EE_BUILD_MEMPRO 1

//   Enable this to include support for the Tracy profiler
//   (https://github.com/wolfpld/tracy)
//   Not 'EE_BUILD_TRACY' because Tracy requires that this specific token be used
//   and defining a EE_BUILD_TRACY that just aliases TRACY_ENABLE seems redundant
//#    define TRACY_ENABLE 1

//   Enable this to include instrumentation data when the Superluminal
//   profiler is used (http://www.superluminal.eu). Note that it is
//   not required in order to use Superluminal, it just adds some extra
//   labels and higher-level events.
//#    define EE_BUILD_SUPERLUMINAL 1

//   FramePro is not supported when /clr is used, as it requires <atomic>
//   and that header file triggers fatal error C1189 when /clr is set:
//   "<atomic> is not supported when compiling with /clr or /clr:pure."
#    if !defined( __CLR_VER )

//     Enable this to include support for PureDev's FramePro profiler
//     (http://www.puredevsoftware.com/framepro/index.htm)
//#      define EE_BUILD_FRAMEPRO 1

#    endif // #if !defined( __CLR_VER )

#  endif // #if defined( EE_BUILD_PROFILE )

// Use of Dear Imgui (https://github.com/ocornut/imgui) is now supported outside of Retail builds
#if !defined( EE_BUILD_RETAIL )
#  define EE_BUILD_ENABLE_IMGUI
#endif

///////////////////////////////////////////////////////////////////////////////
// Xbox One (aka Durango) configuration
//

#elif defined( EE_BUILD_XB1 )

#  if defined( EE_BUILD_PROFILE )
#    define USE_PIX 1 // required by the PIX API
#  endif

///////////////////////////////////////////////////////////////////////////////
// Xbox Series S/X (aka Scarlett) configuration
//

#elif defined( EE_BUILD_XSX )

#  if defined( EE_BUILD_PROFILE )
#    define USE_PIX 1 // required by the PIX API
#  endif

///////////////////////////////////////////////////////////////////////////////
// Nintendo Switch (aka NX) configuration
//

#elif defined( EE_BUILD_NX )

#  if !defined( EE_BUILD_RETAIL ) || defined( EE_BUILD_ENABLE_EEDEBUG_IN_RETAIL )
#    define NN_ENABLE_LOG // Allow use of NN_LOG in NN_SDK_BUILD_RELEASE builds
#  endif

///////////////////////////////////////////////////////////////////////////////
// Playstation 5 (aka Prospero) configuration
//

#elif defined( EE_BUILD_PS5 )

// When using the Debug or Develop build configs
#  if defined( EE_BUILD_DEBUG )
#    define SCE_AGC_DEBUG             // Enable Agc asserts (with a small perf cost)
#    define SCE_AGC_CHECK_ERROR_CODES // Enable use of [[nodiscard]] attributes
#  endif

#endif // platform elif sequence

///////////////////////////////////////////////////////////////////////////////
// Retail configuration
//

#if defined( EE_BUILD_RETAIL )

#  define NTELEMETRY // make sure that RAD's Telemetry is disabled in Retail builds

#endif // #if defined( EE_BUILD_RETAIL )
