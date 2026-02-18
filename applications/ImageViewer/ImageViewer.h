// Elevation Engine
//
// Copyright (c) 2026 Azimuth Studios

#pragma once

#include <stdint.h>

#include <ee/graphics/GpuFormat.h>
#include <ee/graphics/Image.h>

class ImageViewer
{
public:
	bool LoadImage( const char* filename );

	inline void Release( void )
	{
		mImage.Release();
	}

	const ee::Image& GetImage( void ) const { return mImage; }

	inline void GetDimensions( uint32_t& width, uint32_t& height ) const
	{
		width = mImage.GetWidth();
		height = mImage.GetHeight();
	}

private:
	ee::Image mImage;

}; // class ImageViewer
