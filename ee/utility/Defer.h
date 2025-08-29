#pragma once

#include <utility> // for std::move, std::forward

namespace ee
{
	// Defer implements the 'scope guard' pattern; it's meant to be used to
	// ensure that some bit of code (usually stuff like fclose() or delete)
	// is called when a function completes execution via one of potentially
	// many exit points. Use it via one of the eeDefer() functions defined
	// below. An example of its use:
	//
	// void ReadFile( filename ) {
	//     FILE* file = fopen( filename );
	//     // assuming file was successfully opened...
	//     auto cleanup = defer( [ file ](){ fclose( file ); } );
	//     // do interesting stuff here. fclose() will be called when ReadFile() exits.
	// }
	template< typename Func >
	class Defer
	{
	public:
		explicit Defer( Func func ) noexcept
			: mDeferredFunc( std::move( func ) )
			, mInvoke( true )
		{}

		Defer( Defer< Func >&& other ) noexcept
			: mDeferredFunc( std::move( other.mDeferredFunc ) )
			, mInvoke( other.mInvoke )
		{
			other.mInvoke = false;
		}

		~Defer() noexcept
		{
			if( mInvoke )
				mDeferredFunc();
		}

	private:
		Defer( const Defer< Func >& ) = delete;
		Defer< Func >& operator = ( const Defer< Func >& ) = delete;
		Defer< Func >& operator = ( Defer&& ) = delete;

		Func mDeferredFunc;
		bool mInvoke;

	}; // class Defer

	template< class Func >
	[[nodiscard]] inline Defer< Func > eeDefer( const Func& func ) noexcept
	{
		return Defer< Func >( func );
	}

	template< class Func >
	[[nodiscard]] inline Defer< Func > eeDefer( Func&& func ) noexcept
	{
		return Defer< Func >( std::forward< Func >( func ) );
	}

} // namespace ee
