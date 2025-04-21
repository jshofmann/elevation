// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

#if defined( BUILD_PROFILE )

#include <stdarg.h>

#if defined( BUILD_FRAMEPRO )
#include <FramePro/FramePro.h>
#endif

#if defined( BUILD_ORBIT )
#include <Orbit.h>
#endif

namespace ee
{
	// Note: Prefer the non-varargs event functions, as they don't have
	// the overhead of parsing a printf argument string.

	// color should be in the format 0xffRRGGBB
	void PushGraphicsEvent( const char* name, const wchar_t* widename, uint32_t color );
	void PushGraphicsNamedEvent( uint32_t color, const char* format, va_list args );
	void PopGraphicsEvent( void );

	class GraphicsEventImpl
	{
	public:
		// color is assumed to be in the format 0xffRRGGBB
		GraphicsEventImpl( const char* name, const wchar_t* widename, uint32_t color )
		{
			PushGraphicsEvent( name, widename, color );
		}

		~GraphicsEventImpl()
		{
			PopGraphicsEvent();
		}

	}; // class GraphicsEventImpl

	class GraphicsNamedEventImpl
	{
	public:
		// color is assumed to be in the format 0xffRRGGBB
		// Note: format must be the last named argument to satisfy gcc
		GraphicsNamedEventImpl( uint32_t color, const char* format, ... )
		{
			va_list args;
			va_start( args, format );

			PushGraphicsNamedEvent( color, format, args );

			va_end( args );
		}

		~GraphicsNamedEventImpl()
		{
			PopGraphicsEvent();
		}

	}; // class GraphicsNamedEventImpl

} // namespace ee

#if defined( BUILD_FRAMEPRO )

#define eeGraphicsEvent( name ) GraphicsEventImpl __graphicsEvent( (name), L##name, 0 ); FRAMEPRO_NAMED_SCOPE( name )

// printf-style graphics event declaration. Note the use of a non-standard
// GNU C preprocessor extension to make sure that the trailing comma is
// swallowed if __VA_ARGS__ is empty. While non-standard, this syntax is
// accepted by MSVC, clang, and gcc, which are the compilers we care about.
// (it's required by clang; MSVC and gcc will accept , __VA_ARGS__ as-is.)
#define eeGraphicsNamedEvent( format, ... ) GraphicsNamedEventImpl __graphicsEvent( 0, (format), ## __VA_ARGS__ ); FRAMEPRO_SCOPE()

#elif defined( BUILD_ORBIT )

#define eeGraphicsEvent( name ) GraphicsEventImpl __graphicsEvent( (name), L##name, 0 ); ORBIT_SCOPE( name )

#define eeGraphicsNamedEvent( format, ... ) GraphicsNamedEventImpl __graphicsEvent( 0, (format), ## __VA_ARGS__ ); ORBIT_SCOPE()

#else // !defined( BUILD_FRAMEPRO ) && !defined( BUILD_ORBIT )

#define eeGraphicsEvent( name ) GraphicsEventImpl __graphicsEvent( (name), L##name, 0 )

// printf-style graphics event declaration
#define eeGraphicsNamedEvent( format, ... ) GraphicsNamedEventImpl __graphicsEvent( 0, (format), ## __VA_ARGS__ )

#endif // #if !defined( BUILD_FRAMEPRO ) && !defined( BUILD_ORBIT )

// Manual push-pop version for places where the C++ scopes and command-buffer
// structure doesn't allow use of eeGraphics[Named]Event. Every push must be
// matched with a pop of course. Try to use eeGraphics[Named]Event if possible.
#if defined( BUILD_ORBIT )

#define eePushGraphicsEvent( name )	ee::PushGraphicsEvent( (name), L##name, 0 ); ORBIT_START( name )
// The name argument here is used to make matching push/pop events easier
#define eePopGraphicsEvent( name )	ee::PopGraphicsEvent(); ORBIT_STOP()

#else

#define eePushGraphicsEvent( name )	ee::PushGraphicsEvent( (name), L##name, 0 )
// The name argument here is used to make matching push/pop events easier
#define eePopGraphicsEvent( name )	ee::PopGraphicsEvent()

#endif // !defined( BUILD_ORBIT )

#else // if !defined( BUILD_PROFILE )

#define eeInitGraphicsProfiler()
#define eeShutdownGraphicsProfiler()

#define eeGraphicsEvent( name )
#define eeGraphicsNamedEvent( ... )

#define eePushGraphicsEvent( name )
#define eePopGraphicsEvent( name )

#endif // #if defined( BUILD_PROFILE )
