// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#include "pch.h"

#include <functional>

#include <ee/math/AABB.h>
#include <ee/math/Ray.h>

using namespace ee;

bool AABB::hit( const Ray& r, float tmin, float tmax ) const
{
#if 0 // Reference implementation: simple, clear, but slow

	// iterate over the x, y, and z members of vec3
	for( int i = 0; i < 3; ++i )
	{
		float t0 = ffmin( ( mMin[ i ] - r.getOrigin()[ i ] ) / r.getDirection()[ i ],
						  ( mMax[ i ] - r.getOrigin()[ i ] ) / r.getDirection()[ i ] );

		float t1 = ffmax( ( mMin[ i ] - r.getOrigin()[ i ] ) / r.getDirection()[ i ],
						  ( mMax[ i ] - r.getOrigin()[ i ] ) / r.getDirection()[ i ] );

		tmin = ffmax( t0, tmin );
		tmax = ffmin( t1, tmax );

		if( tmax <= tmin )
			return false;

	} // for( int i = 0; i < 3; ++i )

#else // optimized version

	// iterate over the x, y, and z members of vec3
	for( int i = 0; i < 3; ++i )
	{
		float invD = 1.0f / r.getDirection()[ i ];
		float t0 = ( getMin()[ i ] - r.getOrigin()[ i ] ) * invD;
		float t1 = ( getMax()[ i ] - r.getOrigin()[ i ] ) * invD;
		if( invD < 0.0f )
		{
			// Ensure that the interval endpoints are in increasing order
			std::swap( t0, t1 );
		}

		tmin = t0 > tmin ? t0 : tmin;
		tmax = t1 < tmax ? t1 : tmax;

		if( tmax <= tmin )
			return false;

	} // for( int i = 0; i < 3; ++i )

#endif // #if 0 #else

	return true;
}
