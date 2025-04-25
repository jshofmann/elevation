// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

#include <wrl/client.h>
#include <d3d12.h>

namespace ee
{
	class dx12Resource
	{
	public:
		dx12Resource() {}

		dx12Resource( ID3D12Resource* resource, D3D12_RESOURCE_STATES state )
			: mResource( resource )
			, mUsageState( state )
		{
		}

		virtual ~dx12Resource()
		{
			Release();
		}

		virtual void Release( void )
		{
			mResource = nullptr;
			mGPUVirtualAddress = 0;
			++mVersion;
		}

		ID3D12Resource* operator -> ()
		{
			return mResource.Get();
		}

		const ID3D12Resource* operator -> () const
		{
			return mResource.Get();
		}

		ID3D12Resource* GetResource( void )
		{
			return mResource.Get();
		}

		const ID3D12Resource* GetResource( void ) const
		{
			return mResource.Get();
		}

		ID3D12Resource** GetAddressOf( void )
		{
			return mResource.GetAddressOf();
		}

		D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress( void ) const
		{
			return mGPUVirtualAddress;
		}

		uint32_t GetVersion( void ) const
		{
			return mVersion;
		}

	protected:
		Microsoft::WRL::ComPtr< ID3D12Resource > mResource;
		D3D12_RESOURCE_STATES					 mUsageState = D3D12_RESOURCE_STATE_COMMON;
		D3D12_RESOURCE_STATES					 mTransitioningState = D3D12_RESOURCE_STATES( ~0 );
		D3D12_GPU_VIRTUAL_ADDRESS				 mGPUVirtualAddress = 0;

		// For change detection
		uint32_t mVersion = 0;

	}; // class dx12Resource

} // namespace ee
