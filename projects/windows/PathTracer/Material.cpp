// PathTracer application - part of Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#include "pch.h"

#include "Material.h"

bool Glass::Scatter( const Ray& ray, const HitRecord& hit,
					 vec3& attenuation, Ray& scattered ) const
{
	vec3 reflected = Reflect( ray.GetDirection(), hit.normal );

	// Glass absorbs nothing
	attenuation = vec3( 1.0f, 1.0f, 1.0f );

	vec3 outwardNormal;
	float ni_over_nt, cosine;

	if( Dot( ray.GetDirection(), hit.normal ) > 0.0f )
	{
		outwardNormal = -hit.normal;
		ni_over_nt = mRefractIndex;
		cosine = mRefractIndex * Dot( ray.GetDirection(), hit.normal ) / ray.GetDirection().Length();
	}
	else
	{
		outwardNormal = hit.normal;
		ni_over_nt = 1.0f / mRefractIndex;
		cosine = -Dot( ray.GetDirection(), hit.normal ) / ray.GetDirection().Length();
	}

	float reflectProbability;
	vec3 refracted;

	if( Refract( ray.GetDirection(), outwardNormal, ni_over_nt, refracted ) )
	{
		reflectProbability = Schlick( cosine, mRefractIndex );
	}
	else
	{
		reflectProbability = 1.0f;
	}

	if( RandomFloat() < reflectProbability )
	{
		scattered = Ray( hit.p, reflected );
	}
	else
	{
		scattered = Ray( hit.p, refracted );
	}

	return true;
}
