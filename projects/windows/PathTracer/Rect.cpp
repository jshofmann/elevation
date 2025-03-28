// PathTracer application - part of Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#include "pch.h"

#include "Rect.h"
#include "Material.h"

#include <ee/math/AABB.h>

bool xyRect::Hit( const Ray& r, float t_min, float t_max, HitRecord& hit ) const
{
	float t = ( mK - r.GetOrigin().z ) / r.GetDirection().z;
	if( ( t < t_min ) || ( t > t_max ) )
		return false;

	float x = r.GetOrigin().x + t * r.GetDirection().x;
	float y = r.GetOrigin().y + t * r.GetDirection().y;
	if( ( x < mX0 ) || ( x > mX1 ) || ( y < mY0 ) || ( y > mY1 ) )
		return false;

	hit.u = ( x - mX0 ) / ( mX1 - mX0 );
	hit.v = ( y - mY0 ) / ( mY1 - mY0 );
	hit.t = t;
	hit.material = mMaterial;
	hit.p = r.PointAtParameter( t );
	hit.normal = vec3( 0.0f, 0.0f, 1.0f );

	return true;
}

bool xyRect::GetBoundingBox( float t0, float t1, AABB& box ) const
{
	box = AABB( vec3( mX0, mY0, mK - 0.0001f ), vec3( mX1, mY1, mK + 0.0001f ) );
	return true;
}
