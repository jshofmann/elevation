// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

#include <ee/math/Math.h>
#include <ee/math/vec3.h>

namespace ee
{
	class Perlin
	{
	public:
		float Noise( const vec3& p ) const;

		float Turbulence( const vec3& p, int depth = 7 ) const;

	private:

		static vec3*	mRandomVec3;
		static int*		mPermuteX;
		static int*		mPermuteY;
		static int*		mPermuteZ;

	}; // class Perlin

} // namespace ee
