// PathTracer application - part of Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#include "pch.h"

#include "Sphere.h"

#include <ee/math/AABB.h>
#include <ee/math/Math.h>

bool Sphere::Hit( const Ray& ray, float t_min, float t_max, HitRecord& hit ) const
{
	vec3 oc = ray.GetOrigin() - GetCenter( ray.GetTime() );
	float a = Dot( ray.GetDirection(), ray.GetDirection() );
	float b = Dot( oc, ray.GetDirection() );
	float c = Dot( oc, oc ) - mRadius * mRadius;

	// The 4 term in 4 * a * c was cancelled out by the 2 that was in the
	// b term above and the 2 that was in the temp term below
	float discriminant = b * b - a * c;
	if( discriminant > 0.0f )
	{
		float temp = ( -b - sqrtf( discriminant ) ) / a;
		if( temp > t_min && temp < t_max )
		{
			hit.t = temp;
			hit.p = ray.PointAtParameter( hit.t );
			hit.normal = ( hit.p - GetCenter( ray.GetTime() ) ) / mRadius;
			hit.material = mMaterial;
			return true;
		}

		temp = ( -b + sqrtf( discriminant ) ) / a;
		if( temp > t_min && temp < t_max )
		{
			hit.t = temp;
			hit.p = ray.PointAtParameter( hit.t );
			hit.normal = ( hit.p - GetCenter( ray.GetTime() ) ) / mRadius;
			hit.material = mMaterial;
			return true;
		}

	} // if( discriminant > 0.0f )

	return false;
}

bool Sphere::GetBoundingBox( float t0, float t1, AABB& box ) const
{
	// If this sphere is stationary
	if( t0 == t1 )
	{
		box = AABB( mA - vec3( mRadius, mRadius, mRadius ), mA + vec3( mRadius, mRadius, mRadius ) );
	}
	else
	{
		// Return a region enclosing any place this sphere can be
		AABB boxa( mA - vec3( mRadius, mRadius, mRadius ), mA + vec3( mRadius, mRadius, mRadius ) );
		AABB boxb( mB - vec3( mRadius, mRadius, mRadius ), mB + vec3( mRadius, mRadius, mRadius ) );
		box = Enclose( boxa, boxb );
	}

	return true;
}
