// Elevation Engine
//
// Copyright (C) 2025 Azimuth Studios

#include "pch.h"

#include "dx12ColorBuffer.h"

using namespace ee;

void dx12ColorBuffer::CreateFromSwapChain( ID3D12Device* device, ID3D12Resource* resource, const std::string_view& name )
{
	AssociateWithResource( resource, D3D12_RESOURCE_STATE_PRESENT, name );

//	mRTVHandle = Graphics::AllocateDescriptor( D3D12_DESCRIPTOR_HEAP_TYPE_RTV );
	device->CreateRenderTargetView( mResource.Get(), nullptr, mRTVHandle );
}
