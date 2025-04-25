// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

#include <string_view>
#include <d3d12.h>

#include <drivers/dx12/dx12FrameBuffer.h>

namespace ee
{
	// A swap chain's display planes
	class dx12ColorBuffer : public dx12FrameBuffer
	{
	public:
		void CreateFromSwapChain( ID3D12Device* device, ID3D12Resource* resource, const std::string_view& name );
		
	private:
		D3D12_CPU_DESCRIPTOR_HANDLE mSRVHandle = {};
		D3D12_CPU_DESCRIPTOR_HANDLE mRTVHandle = {};

	}; // class dx12ColorBuffer

} // namespace ee
