// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

#include <string_view>
#include <d3d12.h>

namespace ee
{
	namespace dxDebug
	{
		// Why aren't we just using ID3D12Object::SetName() instead?
		// Because ID3D12Object::SetName() takes a LPCWSTR with no LPCSTR option
		void SetObjectName( ID3D12Object* object, const std::string_view& name );
	} // namespace dxDebug

	const char* GetD3DFeatureLevelString( D3D_FEATURE_LEVEL level );
	const char* GetTiledResourcesTierString( D3D12_TILED_RESOURCES_TIER tier );
	const char* GetConservativeRasterizationTierString( D3D12_CONSERVATIVE_RASTERIZATION_TIER tier );
	const char* GetResourceBindingTierString( D3D12_RESOURCE_BINDING_TIER tier );

} // namespace ee
