// PathTracer application - part of Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

#include <stdint.h>

#include "Traceable.h"

using namespace ee;

// Called "hittable_list" in the "Ray Tracing in One Weekend" book
class Scene : public Traceable
{
public:
	Scene();
	~Scene();

	// Traceable interface implementation

	virtual bool Hit( const Ray& r, float t_min, float t_max, HitRecord& rec ) const;

	virtual bool GetBoundingBox( float t0, float t1, AABB& box ) const;

	// Scene member functions

	// This function takes ownership of the Traceable objects in list
	bool Initialize( Traceable** list, uint32_t listSize );
	void Shutdown( void );

	uint32_t GetListSize( void ) const;

private:
	Traceable**	mList;
	uint32_t	mListSize;

}; // class Scene

inline Scene::Scene()
	: mList( nullptr )
	, mListSize( 0 )
{
}

inline Scene::~Scene()
{
	Shutdown();
}

inline uint32_t Scene::GetListSize( void ) const
{
	return mListSize;
}
