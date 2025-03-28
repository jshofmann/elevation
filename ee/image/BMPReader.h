// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

#include <stdint.h>

#include "BMPSupport.h"

namespace ee
{
	class BMPReader
	{
	public:
		BMPReader();

		// Assumes that data contains the contents of a .bmp format file.
		// This function will not copy the data array given to it; this data
		// must remain valid until the BMPReader instance is destroyed.
		bool Set( const void* data, uint32_t dataSizeInBytes );

		uint32_t GetImageSize( void ) const;

		inline int GetWidth( void ) const { return mInfoHeader.biWidth; }
		inline int GetHeight( void ) const { return mInfoHeader.biHeight; }
		inline uint16_t GetBitCount( void ) const { return mInfoHeader.biBitCount; }

		inline uint32_t GetCompression( void ) const { return mInfoHeader.biCompression; }

		// If false, the origin is in the bottom left corner
		inline bool HasTopLeftOrigin( void ) const { return mInfoHeader.biSizeImage < 0; }

		inline const uint8_t* GetPixelData( void ) const { return mPixelData; }

	private:
		BitmapFileHeader	mFileHeader;
		BitmapInfoHeader	mInfoHeader;
		const uint8_t*		mPixelData;

	}; // class BMPReader

} // namespace ee
