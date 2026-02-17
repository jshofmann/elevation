// Elevation Engine
//
// Copyright (c) 2026 Azimuth Studios

#pragma once

#include <stdint.h>

#include "graphics/GpuFormat.h"

namespace ee
{
	// Image::GetSizeInBytes() but without the need to create an Image object;
	// also can estimate the storage necessary for array / volume textures,
	// textures with a mip chain, and BCn-format textures.
	size_t ImageGetSizeInBytes( GpuFormat format, uint32_t width, uint32_t height, uint32_t depth = 1, uint32_t mipCount = 1 );

	// A two-dimensional array of pixel values similar to what DirectX calls
	// a 'surface'. Texture mip chains, cubemaps / array / volume textures,
	// are all built from collections of Images. Note that while Image is
	// declared in the ee/graphics directory, most functions that operate
	// on Image objects are defined in the ee/image directory.
	class Image
	{
	public:
		Image() {}

		// Does not take ownership of the pixels array
		Image( uint32_t width, uint32_t height, GpuFormat format, const void* pixels = nullptr );
		~Image() { Release(); }

		bool Allocate( uint32_t width, uint32_t height, GpuFormat format );
		void Release( void );

		uint32_t GetWidth( void ) const { return mWidth; }
		uint32_t GetHeight( void ) const { return mHeight; }
		GpuFormat GetFormat( void ) const { return mFormat; }

		const uint8_t* GetPixels( void ) const { return mPixels; }
		uint8_t* GetPixels( void ) { return mPixels; }

		size_t GetSizeInBytes( void ) const
		{
			return ImageGetSizeInBytes( mFormat, mWidth, mHeight );
		}

		// Does not take ownership of pixels or copy it into local storage;
		// the memory referenced by the pixels pointer must remain valid
		// for as long as this Image object holds that pointer.
		void SetPixels( const void* pixels );

	protected:
		uint32_t	mWidth = 0;
		uint32_t	mHeight = 0;
		GpuFormat	mFormat = GpuFormat::kUnknown;

		uint8_t*	mPixels = nullptr;
		bool		mPixelsOwned = false;

	}; // class Image

} // namespace ee
