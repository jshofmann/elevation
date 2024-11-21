#pragma once

#include <stdint.h>

namespace ee
{
	namespace PNGWriter
	{
		bool Write( const uint8_t* pixels, uint16_t width, uint16_t height,
					uint8_t bytesPerPixel, const char* filename );

	} // namespace PNGWriter

} // namespace ee
