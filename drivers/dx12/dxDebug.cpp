// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#include "pch.h"

#include "dxDebug.h"

using namespace ee;

const char* ee::GetD3DFeatureLevelString( D3D_FEATURE_LEVEL level )
{
	switch( level )
	{
//	case D3D_FEATURE_LEVEL_1_0_GENERIC:
//		return "D3D_FEATURE_LEVEL_1_0_GENERIC";
	case D3D_FEATURE_LEVEL_1_0_CORE:
		return "D3D_FEATURE_LEVEL_1_0_CORE";
	case D3D_FEATURE_LEVEL_9_1:
		return "D3D_FEATURE_LEVEL_9_1";
	case D3D_FEATURE_LEVEL_9_2:
		return "D3D_FEATURE_LEVEL_9_2";
	case D3D_FEATURE_LEVEL_9_3:
		return "D3D_FEATURE_LEVEL_9_3";
	case D3D_FEATURE_LEVEL_10_0:
		return "D3D_FEATURE_LEVEL_10_0";
	case D3D_FEATURE_LEVEL_10_1:
		return "D3D_FEATURE_LEVEL_10_1";
	case D3D_FEATURE_LEVEL_11_0:
		return "D3D_FEATURE_LEVEL_11_0";
	case D3D_FEATURE_LEVEL_11_1:
		return "D3D_FEATURE_LEVEL_11_1";
	case D3D_FEATURE_LEVEL_12_0:
		return "D3D_FEATURE_LEVEL_12_0";
	case D3D_FEATURE_LEVEL_12_1:
		return "D3D_FEATURE_LEVEL_12_1";
	case D3D_FEATURE_LEVEL_12_2:
		return "D3D_FEATURE_LEVEL_12_2";
	}

	// If this is hit check if this enum was changed in <d3dcommon.h>
	eeAssertAlways( "Unsupported D3D_FEATURE_LEVEL value! "
					"Check <d3dcommon.h> for changes." );
	return "Unsupported D3D_FEATURE_LEVEL value";
}

const char* ee::GetTiledResourcesTierString( D3D12_TILED_RESOURCES_TIER tier )
{
	switch( tier )
	{
	case D3D12_TILED_RESOURCES_TIER_NOT_SUPPORTED:
		return "D3D12_TILED_RESOURCES_TIER_NOT_SUPPORTED";
	case D3D12_TILED_RESOURCES_TIER_1:
		return "D3D12_TILED_RESOURCES_TIER_1";
	case D3D12_TILED_RESOURCES_TIER_2:
		return "D3D12_TILED_RESOURCES_TIER_2";
	case D3D12_TILED_RESOURCES_TIER_3:
		return "D3D12_TILED_RESOURCES_TIER_3";
	case D3D12_TILED_RESOURCES_TIER_4:
		return "D3D12_TILED_RESOURCES_TIER_4";
	}

	// If this is hit check if this enum was changed in <d3d12.h>
	eeAssertAlways( "Unsupported D3D12_TILED_RESOURCES_TIER value! "
					"Check <d3d12.h> for changes." );
	return "Unsupported D3D12_TILED_RESOURCES_TIER value";
}

const char* ee::GetConservativeRasterizationTierString( D3D12_CONSERVATIVE_RASTERIZATION_TIER tier )
{
	switch( tier )
	{
	case D3D12_CONSERVATIVE_RASTERIZATION_TIER_NOT_SUPPORTED:
		return "D3D12_CONSERVATIVE_RASTERIZATION_TIER_NOT_SUPPORTED";
	case D3D12_CONSERVATIVE_RASTERIZATION_TIER_1:
		return "D3D12_CONSERVATIVE_RASTERIZATION_TIER_1";
	case D3D12_CONSERVATIVE_RASTERIZATION_TIER_2:
		return "D3D12_CONSERVATIVE_RASTERIZATION_TIER_2";
	case D3D12_CONSERVATIVE_RASTERIZATION_TIER_3:
		return "D3D12_CONSERVATIVE_RASTERIZATION_TIER_3";
	}

	// If this is hit check if this enum was changed in <d3d12.h>
	eeAssertAlways( "Unsupported D3D12_CONSERVATIVE_RASTERIZATION_TIER value! "
					"Check <d3d12.h> for changes." );
	return "Unsupported D3D12_CONSERVATIVE_RASTERIZATION_TIER value";
}

const char* ee::GetResourceBindingTierString( D3D12_RESOURCE_BINDING_TIER tier )
{
	switch( tier )
	{
	case D3D12_RESOURCE_BINDING_TIER_1:
		return "D3D12_RESOURCE_BINDING_TIER_1";
	case D3D12_RESOURCE_BINDING_TIER_2:
		return "D3D12_RESOURCE_BINDING_TIER_2";
	case D3D12_RESOURCE_BINDING_TIER_3:
		return "D3D12_RESOURCE_BINDING_TIER_3";
	}

	// If this is hit check if this enum was changed in <d3d12.h>
	eeAssertAlways( "Unsupported D3D12_RESOURCE_BINDING_TIER value! "
					"Check <d3d12.h> for changes." );
	return "Unsupported D3D12_RESOURCE_BINDING_TIER value";
}
