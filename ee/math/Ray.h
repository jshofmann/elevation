// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

#include <ee/math/vec3.h>

namespace ee
{
	class Ray
	{
	public:
		Ray() {}
		Ray( const vec3& origin, const vec3& direction, float time = 0.0f );

		inline const vec3& GetOrigin( void ) const;
		inline const vec3& GetDirection( void ) const;
		inline float GetTime( void ) const;

		inline vec3 PointAtParameter( float t ) const;

	private:
		vec3	mOrigin;
		vec3	mDirection;
		float	mTime;

	}; // class Ray

	inline Ray::Ray( const vec3& origin, const vec3& direction, float time )
		: mOrigin( origin )
		, mDirection( direction )
		, mTime( time )
	{
	}

	inline const vec3& Ray::GetOrigin( void ) const
	{
		return mOrigin;
	}

	inline const vec3& Ray::GetDirection( void ) const
	{
		return mDirection;
	}

	inline float Ray::GetTime( void ) const
	{
		return mTime;
	}

	inline vec3 Ray::PointAtParameter( float t ) const
	{
		return mOrigin + t * mDirection;
	}

} // namespace Ray
