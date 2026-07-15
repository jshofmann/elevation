// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

#include <stdint.h>

namespace ee
{
	namespace PNGWriter
	{
		// Describes which corner of the image the first row of 'pixels'
		// corresponds to. PNG scanlines are always stored top-to-bottom on
		// disk, so BottomLeft-origin sources (eg. raytraced images, OpenGL
		// readbacks) are flipped during the write.
		enum class ImageOrigin
		{
			kTopLeft,
			kBottomLeft,
		};

		bool Write( const uint8_t* pixels, uint16_t width, uint16_t height,
					uint8_t bytesPerPixel, const char* filename,
					ImageOrigin origin = ImageOrigin::kTopLeft );

	} // namespace PNGWriter

} // namespace ee
