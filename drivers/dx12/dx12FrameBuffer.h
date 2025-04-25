// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

#include <string_view>
#include <dxgi.h>

#include <drivers/dx12/dx12Resource.h>

namespace ee
{
	// A resource with a width, height, depth, and pixel format;
	// textures, color buffers, and depth buffers derive from this
	class dx12FrameBuffer : public dx12Resource
	{
	public:
		uint32_t GetWidth( void ) const
		{
			return mWidth;
		}

		uint32_t GetHeight( void ) const
		{
			return mHeight;
		}

		uint32_t GetDepth( void ) const
		{
			return mDepth;
		}

		const DXGI_FORMAT& GetFormat( void ) const
		{
			return mFormat;
		}

	protected:
		void AssociateWithResource( ID3D12Resource*			resource,
									D3D12_RESOURCE_STATES	state,
									const std::string_view& name );

		uint32_t	mWidth	= 0;
		uint32_t	mHeight = 0;
		uint32_t	mDepth	= 0;
		DXGI_FORMAT mFormat = DXGI_FORMAT_UNKNOWN;

	}; // class dx12FrameBuffer

} // namespace ee
