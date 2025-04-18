// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#include "pch.h"

#include <ee/graphics/Device.h>

#include <drivers/dx12/dx12Device.h>
//#include <drivers/vulkan/vulDevice.h>

using namespace ee;

namespace ee
{
	std::unique_ptr< Device > MakeDevice( Driver driver )
	{
		/*if( driver == Driver::kVulkan )
		{
			return std::make_unique< vulDevice >();
		}*/

		// DirectX12 is the default driver for the WIndows platform
		return std::make_unique< dx12Device >();
	}

} // namespace ee
