// PathTracer application - part of Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

#include <ee/math/AABB.h>

#include "Traceable.h"

using namespace ee;

class Material;

class xyRect : public Traceable
{
public:
	xyRect()
		: mX0( 0.0f ), mX1( 0.0f )
		, mY0( 0.0f ), mY1( 0.0f )
		, mK( 0.0f ), mMaterial( nullptr )
	{}

	xyRect( float x0, float x1, float y0, float y1, float k, Material* material )
		: mX0( x0 ), mX1( x1 )
		, mY0( y0 ), mY1( y1 )
		, mK( k ), mMaterial( material )
	{}

	// Traceable interface implementation

	virtual bool Hit( const Ray& r, float t_min, float t_max, HitRecord& rec ) const;

	virtual bool GetBoundingBox( float t0, float t1, AABB& box ) const;

private:
	float		mX0, mX1;
	float		mY0, mY1;
	float		mK;
	Material*	mMaterial;

}; // class xyRect
