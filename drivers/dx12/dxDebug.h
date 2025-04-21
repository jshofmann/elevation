// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

#include <d3d12.h>

namespace ee
{
	const char* GetD3DFeatureLevelString( D3D_FEATURE_LEVEL level );
	const char* GetTiledResourcesTierString( D3D12_TILED_RESOURCES_TIER tier );
	const char* GetConservativeRasterizationTierString( D3D12_CONSERVATIVE_RASTERIZATION_TIER tier );
	const char* GetResourceBindingTierString( D3D12_RESOURCE_BINDING_TIER tier );

} // namespace ee
