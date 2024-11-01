// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

namespace ee
{
	template< class T >
	inline const T& eeMin( const T& x, const T& y )
	{
		return ( x < y ) ? x : y;
	}

	template< class T >
	inline const T& eeMin( const T& x, const T& y, const T& z )
	{
		return eeMin( eeMin( x, y ), z );
	}

	template< class T >
	inline const T& eeMin( const T& x, const T& y, const T& z, const T& w )
	{
		return eeMin( eeMin( x, y, z ), w );
	}

	template< class T >
	inline const T& eeMax( const T& x, const T& y )
	{
		return ( x > y ) ? x : y;
	}

	template< class T >
	inline const T& eeMax( const T& x, const T& y, const T& z )
	{
		return eeMax( eeMax( x, y ), z );
	}

	template< class T >
	inline const T& eeMax( const T& x, const T& y, const T& z, const T& w )
	{
		return eeMax( eeMax( x, y, z ), w );
	}

	template< class T >
	inline const T& eeClamp( const T& x, const T& min, const T& max )
	{
		if( x < min )
			return min;
		if( x > max )
			return max;

		return x;
	}

	template< class T >
	inline const T& eeSaturate( const T& x )
	{
		if( x < T( 0 ) )
			return T( 0 );
		if( x > T( 1 ) )
			return T( 1 );

		return x;
	}

	// Mark a variable as "used" without actually using it
	// Useful to suppress "unused variable" warnings resulting from assert tests-
	// e.g. int bytesRead = stream->read( ... ); assert( bytesRead == ... ) will
	// cause an "unused variable 'bytesRead'" warning in builds without assert().
	// See https://herbsutter.com/2009/10/18/mailbag-shutting-up-compiler-warnings/
	// for more information on this technique.
	// This will eventually be obsoleted by std::optional.
	template< class T >
	inline void eeUnusedVariable( const T& ) {}

} // namespace ee
