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

	inline void GetDimensions( uint32_t& width, uint32_t& height ) const
	{
		width = mImage.GetWidth();
		height = mImage.GetHeight();
	}

	inline ee::GpuFormat GetPixelFormat( void ) const
	{
		return mImage.GetFormat();
	}

	inline const uint8_t* GetPixels( void ) const
	{
		return mImage.GetPixels();
	}

private:
	ee::Image mImage;

}; // class ImageViewer
