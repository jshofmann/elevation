// Elevation Engine
//
// Copyright (C) 2026 Azimuth Studios

#include "pch.h"

#include <algorithm>

#include "graphics/GpuFormat.h"
#include "graphics/Image.h"

using namespace ee;

// Does not take ownership of the pixels array
Image::Image( uint32_t width, uint32_t height, GpuFormat format, const void* pixels )
	: mWidth( width )
	, mHeight( height )
	, mFormat( format )
	, mPixels( reinterpret_cast< uint8_t* >( const_cast< void* >( pixels ) ) )
	, mPixelsOwned( false )
{
}

bool Image::Allocate( uint32_t width, uint32_t height, GpuFormat format )
{
	Release();

	mWidth  = width;
	mHeight = height;
	mFormat = format;

	size_t size = GetSizeInBytes();
	mPixels = new uint8_t[ size ];
	if( mPixels == nullptr )
		return false;

	mPixelsOwned = true;
	memset( mPixels, 0, size );

	return true;
}

void Image::Release( void )
{
	if( ( mPixels != nullptr ) && mPixelsOwned )
	{
		delete[] mPixels;
	}

	mPixels = nullptr;
	mPixelsOwned = false;
}

void Image::SetPixels( const void* pixels )
{
	Release();

	mPixels = reinterpret_cast< uint8_t* >( const_cast< void* >( pixels ) );
	mPixelsOwned = false;
}

size_t ee::ImageGetSizeInBytes( GpuFormat format, uint32_t width, uint32_t height, uint32_t depth, uint32_t mipCount )
{
    size_t size = 0;
    uint32_t bytesPerPixel = GetBytesPerPixel( format );

	if( IsGpuFormatCompressed( format ) )
	{
		// Replace width and height with the horizontal and vertical block count
		// since BCn-compressed textures are organized into 4x4 pixel blocks
		width = static_cast< uint32_t >( ( width + 3 ) / 4 );
		height = static_cast< uint32_t >( ( height + 3 ) / 4 );
	}

	uint32_t levelWidth = width;
	uint32_t levelHeight = height;
	uint32_t levelDepth = depth;

    for( uint32_t i = 0; i < mipCount; ++i )
	{
        size += bytesPerPixel * levelWidth * levelHeight * levelDepth;

		levelWidth = std::max( levelWidth >> 1, 1u );
		levelHeight = std::max( levelHeight >> 1, 1u );
		levelDepth = std::max( levelDepth >> 1, 1u );
	}

    return size;
}
