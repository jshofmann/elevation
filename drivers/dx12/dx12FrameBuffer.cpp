// Elevation Engine
//
// Copyright (C) 2025 Azimuth Studios

#include "pch.h"

#include "dx12FrameBuffer.h"
#include "dxDebug.h"

using namespace ee;

void dx12FrameBuffer::AssociateWithResource( ID3D12Resource*		 resource,
											 D3D12_RESOURCE_STATES	 state,
											 const std::string_view& name )
{
	D3D12_RESOURCE_DESC desc = resource->GetDesc();

	mResource.Attach( resource );
	mUsageState = state;

	mWidth	= uint32_t( desc.Width ); // We don't support large virtual textures - yet
	mHeight = desc.Height;
	mDepth	= desc.DepthOrArraySize;
	mFormat = desc.Format;

	// Why aren't we just using ID3D12Object::SetName() here?
	// Because ID3D12Object::SetName() takes a LPCWSTR with no LPCSTR option.
	dxDebug::SetObjectName( mResource.Get(), name );
}
