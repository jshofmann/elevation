// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

#include <stdint.h>

#if defined( EE_COMPILER_MSVC )
	#pragma pack( push, p, 1 )
#elif defined( EE_COMPILER_GCC )
	#pragma pack( push, 1 )
#elif defined( EE_COMPILER_CLANG )
	#pragma pack( 1 )
#endif

namespace ee
{
	// as defined by http://en.wikipedia.org/wiki/Truevision_TGA and
	// http://www.paulbourke.net/dataformats/tga
	// All values are specified in little-endian format;
	// sizeof( TGAHeader ) == 18 bytes per the TGA spec
	class TGAHeader
	{
	public:
		enum
		{
			IMAGE_TYPE_NO_IMAGE_DATA                 = 0,
			IMAGE_TYPE_UNCOMPRESSED_COLOR_MAPPED     = 1,
			IMAGE_TYPE_UNCOMPRESSED_RGB              = 2,
			IMAGE_TYPE_UNCOMPRESSED_GREYSCALE        = 3,
			IMAGE_TYPE_RLE_COLOR_MAPPED              = 9,
			IMAGE_TYPE_RLE_RGB                       = 10,
			IMAGE_TYPE_COMPRESSED_GREYSCALE          = 11,
			IMAGE_TYPE_COMPRESSED_COLOR_MAPPED       = 32,
			IMAGE_TYPE_COMPRESSED_COLOR_MAPPED_4PASS = 33
		};

		uint8_t		IDLength = 0;
		uint8_t		ColormapType = 0; // no colormap included
		uint8_t		ImageType = IMAGE_TYPE_NO_IMAGE_DATA;
		uint16_t	ColormapOrigin = 0;
		uint16_t	ColormapLength = 0;
		uint8_t		ColormapEntrySize = 0; // bytes
		uint16_t	XOrigin = 0; // pixels
		uint16_t	YOrigin = 0; // pixels
		uint16_t	ImageWidth = 0; // pixels
		uint16_t	ImageHeight = 0; // pixels
		uint8_t		BitsPerPixel = 0;
		// ImageDescriptor: 0x20 defines a top-left origin, 0x00 is bottom-left;
		// 0x08 defines an 8-bit alpha channel
		uint8_t		ImageDescriptor = 0x28;

	}; // class TGAHeader

} // namespace ee

#if defined( EE_COMPILER_MSVC )
	#pragma pack( pop, p )
#elif defined( EE_COMPILER_GCC )
	#pragma pack( pop )
#elif defined( EE_COMPILER_CLANG )
	#pragma options align = reset
#endif
