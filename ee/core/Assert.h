// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

///////////////////////////////////////////////////////////////////////////////
//
// eeFatal		Displays an error, then exits the application
// eeAssert		Sends output to a popup window, and forces a breakpoint
//
// Example:
// eeAssert( foos.size() > 0, "Foo %s is empty", foos.getName() );
//
///////////////////////////////////////////////////////////////////////////////

extern bool gInFatalError;

#if !defined( BUILD_RETAIL )
#	define BUILD_HAS_ASSERT 1
#	define BUILD_HAS_ASSERT_RELEASE 1
#	define BUILD_HAS_FATAL 1
#endif

#define eeSilentFatal( condition, ... ) 									\
	{																		\
		if( !(condition) ) {												\
			ee::_SilentFatalError();										\
		}																	\
	}

#if defined( BUILD_HAS_FATAL )
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

#if defined( BUILD_HAS_ASSERT_RELEASE )
	#if	defined( BUILD_DEBUG )
		#define eeAssertRelease eeAssert
	#else
		#define eeAssertRelease( condition, ... )							\
				{															\
					if( !(condition) )										\
					{														\
						if( _DebuggerOnlyDisplay( STRINGIFY( condition ) ) ) \
							BreakPoint;										\
					}														\
				}
	#endif

#else // if !defined( BUILD_HAS_ASSERT_RELEASE )

	#define eeAssertRelease( condition, ... )								\
		{																	\
			(void) sizeof( condition );										\
		}

#endif

#if defined( BUILD_HAS_ASSERT )

	// PC asserts.  Pop up a message box and give the user some options.
	#define eeAssert( condition, ... )										\
		{																	\
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
		}

	#define eeAssertAlways( ... )											\
		{																	\
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
		}

#else // if !defined( BUILD_HAS_ASSERT )

	// This macro body will get rid of unreferenced parameter warnings (at more
	// stringent compiler error levels) and is guaranteed not to emit any code.
	// Cast to void will stop compilers from bitching about statements having
	// no effect.

	#define eeAssert( condition, ... )	( ( void )( 0 ) )
	#define eeAssertAlways( ... )		( ( void )( 0 ) )

#endif // BUILD_HAS_ASSERT

#if defined( BUILD_PC ) && !defined( BUILD_X64 )

	// This is better than DebugBreak, because it will make the debugger go to
	// the correct location in the code. With DebugBreak you have back up the
	// stack one level.
	#define BreakPoint __asm { int 3 }

#elif defined( BUILD_X64 )

	#define BreakPoint DebugBreak()

#else

	#error Define a breakpoint for this platform

#endif

namespace ee
{
	class ErrorResult
	{
	public:
		enum
		{
			kIgnore		= 0,
			kContinue	= 1,
			kBreak		= 2,
			kAbort		= 3
		};
	};

	void _ShowError( const char* function, const char* file, int line, const char* format, ... );

	int  _Assert( const char* function, const char* file, int line, const char* format, ... );
	void _SilentAssert( const char* function, const char* file, int line, const char* format, ... );

	void _FatalError( const char* function, const char* file, int line, const char* format, ... );
	void _SilentFatalError( void );

} // namespace ee
