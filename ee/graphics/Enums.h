// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

namespace ee
{
	enum class DisplayMode
	{
		kWindowed		  = 0,
		kFullscreen		  = 1,
		kFullscreenWindow = 2,
		kCount
	};

	enum class ColorSpace
	{
		ksRGB  = 0, // Standard LDR color space
		kscRGB = 1, // Windows HDR - 16-bit float
		kHDR10 = 2, // Console HDR - 10-bit UNORM
		kCount
	};

	enum class FlipInterval
	{
		k30Hz	   = 0,
		k60Hz	   = 1,
		kImmediate = 2,
		kCount
	};

} // namespace ee
