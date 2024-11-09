#pragma once

#include <ee/math/Math.h>
#include <ee/math/vec3.h>

class Ray;

class AABB
{
public:
	AABB()
		: mMin( 0.0f, 0.0f, 0.0f )
		, mMax( 0.0f, 0.0f, 0.0f )
	{}

	AABB( const vec3& min, const vec3& max )
		: mMin( min )
		, mMax( max )
	{}

	inline const vec3& getMin( void ) const
	{
		return mMin;
	}

	inline const vec3& getMax( void ) const
	{
		return mMax;
	}

	bool hit( const Ray& r, float tmin, float tmax ) const;

private:
	vec3 mMin;
	vec3 mMax;

}; // class AABB

// Return the union of the two axis-aligned bounding boxes
// ("union" is a C++ reserved keyword, of course)
inline AABB enclose( const AABB& box0, const AABB& box1 )
{
	// "min" and "max" are of course #defined by Microsoft....
	vec3 mins( eeMin( box0.getMin().x, box1.getMin().x ),
			   eeMin( box0.getMin().y, box1.getMin().y ),
			   eeMin( box0.getMin().z, box1.getMin().z ) );

	vec3 maxs( eeMax( box0.getMax().x, box1.getMax().x ),
			   eeMax( box0.getMax().y, box1.getMax().y ),
			   eeMax( box0.getMax().z, box1.getMax().z ) );

	return AABB( mins, maxs );
}
