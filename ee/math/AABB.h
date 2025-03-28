// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

#include <ee/math/Math.h>
#include <ee/math/vec3.h>

namespace ee
{
	class Ray;

	class AABB
	{
	public:
		AABB()
			: mMin( 0.0f, 0.0f, 0.0f )
			, mMax( 0.0f, 0.0f, 0.0f )
		{
		}

		AABB( const vec3& min, const vec3& max )
			: mMin( min )
			, mMax( max )
		{
		}

		inline const vec3& GetMin( void ) const
		{
			return mMin;
		}

		inline const vec3& GetMax( void ) const
		{
			return mMax;
		}

		bool Hit( const Ray& r, float tmin, float tmax ) const;

	private:
		vec3 mMin;
		vec3 mMax;

	}; // class AABB

	// Return the union of the two axis-aligned bounding boxes
	// ("union" is a C++ reserved keyword, of course)
	inline AABB Enclose( const AABB& box0, const AABB& box1 )
	{
		// "min" and "max" are of course #defined by Microsoft....
		vec3 mins( eeMin( box0.GetMin().x, box1.GetMin().x ), eeMin( box0.GetMin().y, box1.GetMin().y ),
				   eeMin( box0.GetMin().z, box1.GetMin().z ) );

		vec3 maxs( eeMax( box0.GetMax().x, box1.GetMax().x ), eeMax( box0.GetMax().y, box1.GetMax().y ),
				   eeMax( box0.GetMax().z, box1.GetMax().z ) );

		return AABB( mins, maxs );
	}

} // namespace ee
