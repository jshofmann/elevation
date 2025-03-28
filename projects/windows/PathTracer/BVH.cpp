// PathTracer application - part of Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#include "pch.h"

#include "BVH.h"

#include <ee/core/Debug.h>
#include <ee/math/AABB.h>
#include <ee/math/Math.h>

// Predicate functions for qsort()
int boxCompareX( const void* a, const void* b )
{
	const Traceable* at = *reinterpret_cast< const Traceable* const * >( a );
	const Traceable* bt = *reinterpret_cast< const Traceable* const * >( b );

	AABB aBox, bBox;
	if( !at->GetBoundingBox( 0.0f, 0.0f, aBox ) || !bt->GetBoundingBox( 0.0f, 0.0f, bBox ) )
	{
		eeDebug( "A Traceable does not have a bounding box" );
		return 0;
	}

	if( aBox.GetMin().x - bBox.GetMin().x < 0.0f )
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

int boxCompareY( const void* a, const void* b )
{
	const Traceable* at = *reinterpret_cast< const Traceable* const * >( a );
	const Traceable* bt = *reinterpret_cast< const Traceable* const * >( b );

	AABB aBox, bBox;
	if( !at->GetBoundingBox( 0.0f, 0.0f, aBox ) || !bt->GetBoundingBox( 0.0f, 0.0f, bBox ) )
	{
		eeDebug( "A Traceable does not have a bounding box" );
		return 0;
	}

	if( aBox.GetMin().y - bBox.GetMin().y < 0.0f )
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

int boxCompareZ( const void* a, const void* b )
{
	const Traceable* at = *reinterpret_cast< const Traceable* const * >( a );
	const Traceable* bt = *reinterpret_cast< const Traceable* const * >( b );

	AABB aBox, bBox;
	if( !at->GetBoundingBox( 0.0f, 0.0f, aBox ) || !bt->GetBoundingBox( 0.0f, 0.0f, bBox ) )
	{
		eeDebug( "A Traceable does not have a bounding box" );
		return 0;
	}

	if( aBox.GetMin().z - bBox.GetMin().z < 0.0f )
	{
		return -1;
	}
	else
	{
		return 1;
	}
}

BVHNode::BVHNode( Traceable** list, uint32_t listCount, float t0, float t1 )
{
	if( listCount > 2 )
	{
		int axis = int( 3 * RandomFloat() );

		if( axis == 0 ) // x
		{
			qsort( list, listCount, sizeof( Traceable* ), boxCompareX );
		}
		else if( axis == 1 ) // y
		{
			qsort( list, listCount, sizeof( Traceable* ), boxCompareY );
		}
		else // z
		{
			qsort( list, listCount, sizeof( Traceable* ), boxCompareZ );
		}
	}

	if( listCount == 1 )
	{
		mLeft = mRight = list[ 0 ];
	}
	else if( listCount == 2 )
	{
		mLeft = list[ 0 ];
		mRight = list[ 1 ];
	}
	else
	{
		uint32_t halfCount = listCount / 2;
		mLeft = new BVHNode( list, halfCount, t0, t1 );
		mRight = new BVHNode( list + halfCount, listCount - halfCount, t0, t1 );
	}

	AABB leftBounds, rightBounds;
	if( !mLeft->GetBoundingBox( t0, t1, leftBounds ) || !mRight->GetBoundingBox( t0, t1, rightBounds ) )
	{
		eeDebug( "A Traceable is missing its bounding box" );
	}

	mBounds = Enclose( leftBounds, rightBounds );
}

bool BVHNode::Hit( const Ray& ray, float t_min, float t_max, HitRecord& hit ) const
{
	if( mBounds.Hit( ray, t_min, t_max ) )
	{
		HitRecord leftHit, rightHit;
		bool hitLeft = mLeft->Hit( ray, t_min, t_max, leftHit );
		bool hitRight = mRight->Hit( ray, t_min, t_max, rightHit );
		if( hitLeft && hitRight )
		{
			if( leftHit.t < rightHit.t )
			{
				hit = leftHit;
			}
			else
			{
				hit = rightHit;
			}

			return true;
		}
		else if( hitLeft )
		{
			hit = leftHit;
			return true;
		}
		else if( hitRight )
		{
			hit = rightHit;
			return true;
		}
	}

	// else
	return false;
}
