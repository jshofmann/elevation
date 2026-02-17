// Elevation Engine
//
// Copyright (c) 2026 Azimuth Studios

#pragma once

#include <span>

#include "graphics/Image.h"

namespace ee
{
	namespace JPGReader
	{
		// Assumes that jpgFile contains the contents of a .jpg format file.
		// This function will not copy the data in the span given to it.
		bool DecompressImage( std::span< const std::byte > jpgFile, Image& image );

	} // namespace JPGReader

} // namespace ee
