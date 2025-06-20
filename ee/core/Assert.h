// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

///////////////////////////////////////////////////////////////////////////////
//
// eeFatal			If the condition fails displays an error then exits the application
// eeSilentFatal	If the condition fails exits the application
//                  (no error messages displayed)
// eeAssert			Sends output to a popup window where that makes sense,
//                  and forces a breakpoint in all but retail builds
// eeAssertAlways	eeAssert but no conditional;
//                  used to always display the assert dialog
// eeVerify			eeAssert but will always evaluate the conditional,
//                  even in Retail builds
//
// Example:
// eeAssert( foos.size() > 0, "Foo %s is empty", foos.getName() );
//
///////////////////////////////////////////////////////////////////////////////

#if !defined( EE_BUILD_RETAIL )
#	define EE_BUILD_HAS_ASSERT 1
#	define EE_BUILD_HAS_FATAL 1
#endif

#define eeSilentFatal( condition, ... ) 									\
	{																		\
		if( !(condition) ) {												\
			ee::_SilentFatalError();										\
		}																	\
	}

#if defined( EE_BUILD_HAS_FATAL )
	#define eeFatal( condition, ... )										\
		{																	\
			if( !(condition) )												\
			{																\
				ee::_FatalError( __FUNCTION__, __FILE__, __LINE__, "Expression: " STRINGIFY( condition ) "\n\nDescription: " __VA_ARGS__ ); \
			}																\
		}
#else
	#define eeFatal eeSilentFatal
#endif

#if defined( EE_BUILD_HAS_ASSERT )

	// Windows asserts.  Pop up a message box and give the user some options:
	// Ignore disables any future invocations of that assert, Break will
	// stop the app in the debugger at the assertion point, and Continue
	// will resume application execution.
	#define eeAssert( condition, ... )										\
		do {																\
			static bool _IgnoreAssert = false;								\
			if( !_IgnoreAssert && !(condition) )							\
			{																\
				switch( ee::_Assert( __FUNCTION__, __FILE__, __LINE__, "Expression: " STRINGIFY( condition ) "\n\nDescription: " __VA_ARGS__ ) ) \
				{															\
				case ee::ErrorResult::kIgnore:		_IgnoreAssert = true;	break;	\
				case ee::ErrorResult::kBreak:		BreakPoint;				break;	\
				case ee::ErrorResult::kContinue:							break;	\
				}															\
			}																\
		} while( 0 )

	#define eeAssertAlways( ... )											\
		do {																\
			static bool _IgnoreAssert = false;								\
			if( !_IgnoreAssert )											\
			{																\
				switch( ee::_Assert( __FUNCTION__, __FILE__, __LINE__, "Description: " __VA_ARGS__ )) \
				{															\
				case ee::ErrorResult::kIgnore:		_IgnoreAssert = true;	break;	\
				case ee::ErrorResult::kBreak:		BreakPoint;				break;	\
				case ee::ErrorResult::kContinue:							break;	\
				}															\
			}																\
		} while( 0 )

	#define eeVerify eeAssert

#else // if !defined( EE_BUILD_HAS_ASSERT )

	// This macro body will get rid of unreferenced parameter warnings (at more
	// stringent compiler error levels) and is guaranteed not to emit any code.
	// Cast to void will stop compilers from complaining about statements having
	// no effect.

	#define eeAssert( condition, ... )	( ( void )( 0 ) )
	#define eeAssertAlways( ... )		( ( void )( 0 ) )
	#define eeVerify( condition, ... )  do { (condition); } while( 0 )

#endif // EE_BUILD_HAS_ASSERT

#if defined( EE_BUILD_WINDOWS )

#if defined( EE_BUILD_X64 )

	#define BreakPoint DebugBreak()

#elif defined( EE_BUILD_X86 )

	// This is better than DebugBreak, because it will make the debugger go to
	// the correct location in the code. With DebugBreak you have back up the
	// stack one level.
	#define BreakPoint __asm { int 3 }

#elif defined( EE_BUILD_ARM )

	#define BreakPoint __debugbreak()

#else

	#error Define a BreakPoint for this platform

#endif // #elif defined( EE_BUILD_ARM )

#elif defined( EE_BUILD_APPLE )

#if EE_BUILD_X64 // Mac - 64 bit Intel
        
	#define BreakPoint __asm( "int $3" )

#elif EE_BUILD_ARM

	#define BreakPoint __builtin_debugtrap()

#endif // #elif TARGET_CPU_ARM64

#elif defined( EE_BUILD_EMSCRIPTEN )

	#define BreakPoint abort()

#else

#error Define a BreakPoint for this platform

#endif // #elif defined( EE_BUILD_EMSCRIPTEN )

namespace ee
{
	enum class ErrorResult
	{
		kIgnore		= 0,
		kContinue	= 1,
		kBreak		= 2,
		kAbort		= 3
	};

	ErrorResult  _Assert( const char* function, const char* file, int line, const char* format, ... );

	void _FatalError( const char* function, const char* file, int line, const char* format, ... );
	void _SilentFatalError( void );

} // namespace ee
