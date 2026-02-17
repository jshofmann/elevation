// Elevation Engine
//
// Copyright (C) 2025 Azimuth Studios

#include "pch.h"

#include <dxgiformat.h>

#include "dx12Utils.h"

using namespace ee;

DXGI_FORMAT ee::GpuFormatToDXGI_FORMAT( const GpuFormat format )
{
	switch( format )
	{
	case GpuFormat::kUnknown:				return DXGI_FORMAT_UNKNOWN;
	case GpuFormat::kR32G32B32A32_FLOAT:	return DXGI_FORMAT_R32G32B32A32_FLOAT;
	case GpuFormat::kR32G32B32A32_UINT:		return DXGI_FORMAT_R32G32B32A32_UINT;
	case GpuFormat::kR32G32B32A32_SINT:		return DXGI_FORMAT_R32G32B32A32_SINT;
	case GpuFormat::kR16G16B16A16_FLOAT:	return DXGI_FORMAT_R16G16B16A16_FLOAT;
	case GpuFormat::kR16G16B16A16_UINT:		return DXGI_FORMAT_R16G16B16A16_UINT;
	case GpuFormat::kR16G16B16A16_SINT:		return DXGI_FORMAT_R16G16B16A16_SINT;
	case GpuFormat::kR16G16B16A16_UNORM:	return DXGI_FORMAT_R16G16B16A16_UNORM;
	case GpuFormat::kR16G16B16A16_SNORM:	return DXGI_FORMAT_R16G16B16A16_SNORM;
	case GpuFormat::kR10G10B10A2_UINT:		return DXGI_FORMAT_R10G10B10A2_UINT;
	case GpuFormat::kR10G10B10A2_UNORM:		return DXGI_FORMAT_R10G10B10A2_UNORM;
	case GpuFormat::kR8G8B8A8_UINT:			return DXGI_FORMAT_R8G8B8A8_UINT;
	case GpuFormat::kR8G8B8A8_SINT:			return DXGI_FORMAT_R8G8B8A8_SINT;
	case GpuFormat::kR8G8B8A8_UNORM:		return DXGI_FORMAT_R8G8B8A8_UNORM;
	case GpuFormat::kR8G8B8A8_UNORM_SRGB:	return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	case GpuFormat::kR8G8B8A8_SNORM:		return DXGI_FORMAT_R8G8B8A8_SNORM;
	case GpuFormat::kB8G8R8A8_UNORM:		return DXGI_FORMAT_B8G8R8A8_UNORM;
	case GpuFormat::kB8G8R8A8_UNORM_SRGB:	return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
	case GpuFormat::kR32G32B32_FLOAT:		return DXGI_FORMAT_R32G32B32_FLOAT;
	case GpuFormat::kR32G32B32_UINT:		return DXGI_FORMAT_R32G32B32_UINT;
	case GpuFormat::kR32G32B32_SINT:		return DXGI_FORMAT_R32G32B32_SINT;
	case GpuFormat::kR11G11B10_FLOAT:		return DXGI_FORMAT_R11G11B10_FLOAT;
	case GpuFormat::kR32G32_FLOAT:			return DXGI_FORMAT_R32G32_FLOAT;
	case GpuFormat::kR32G32_UINT:			return DXGI_FORMAT_R32G32_UINT;
	case GpuFormat::kR32G32_SINT:			return DXGI_FORMAT_R32G32_SINT;
	case GpuFormat::kR16G16_FLOAT:			return DXGI_FORMAT_R16G16_FLOAT;
	case GpuFormat::kR16G16_UINT:			return DXGI_FORMAT_R16G16_UINT;
	case GpuFormat::kR16G16_SINT:			return DXGI_FORMAT_R16G16_SINT;
	case GpuFormat::kR16G16_UNORM:			return DXGI_FORMAT_R16G16_UNORM;
	case GpuFormat::kR16G16_SNORM:			return DXGI_FORMAT_R16G16_SNORM;
	case GpuFormat::kR8G8_UINT:				return DXGI_FORMAT_R8G8_UINT;
	case GpuFormat::kR8G8_SINT:				return DXGI_FORMAT_R8G8_SINT;
	case GpuFormat::kR8G8_UNORM:			return DXGI_FORMAT_R8G8_UNORM;
	case GpuFormat::kR8G8_SNORM:			return DXGI_FORMAT_R8G8_SNORM;
	case GpuFormat::kR32_FLOAT:				return DXGI_FORMAT_R32_FLOAT;
	case GpuFormat::kR32_UINT:				return DXGI_FORMAT_R32_UINT;
	case GpuFormat::kR32_SINT:				return DXGI_FORMAT_R32_SINT;
	case GpuFormat::kR16_FLOAT:				return DXGI_FORMAT_R16_FLOAT;
	case GpuFormat::kR16_UINT:				return DXGI_FORMAT_R16_UINT;
	case GpuFormat::kR16_SINT:				return DXGI_FORMAT_R16_SINT;
	case GpuFormat::kR16_UNORM:				return DXGI_FORMAT_R16_UNORM;
	case GpuFormat::kR16_SNORM:				return DXGI_FORMAT_R16_SNORM;
	case GpuFormat::kR8_UINT:				return DXGI_FORMAT_R8_UINT;
	case GpuFormat::kR8_SINT:				return DXGI_FORMAT_R8_SINT;
	case GpuFormat::kR8_UNORM:				return DXGI_FORMAT_R8_UNORM;
	case GpuFormat::kR8_SNORM:				return DXGI_FORMAT_R8_SNORM;
	case GpuFormat::kD32_FLOAT:				return DXGI_FORMAT_D32_FLOAT;
	case GpuFormat::kD24_UNORM_S8_UINT:		return DXGI_FORMAT_D24_UNORM_S8_UINT;
	case GpuFormat::kD16_UNORM:				return DXGI_FORMAT_D16_UNORM;
	case GpuFormat::kBC1_UNORM:				return DXGI_FORMAT_BC1_UNORM;
	case GpuFormat::kBC1_UNORM_SRGB:		return DXGI_FORMAT_BC1_UNORM_SRGB;
	case GpuFormat::kBC2_UNORM:				return DXGI_FORMAT_BC2_UNORM;
	case GpuFormat::kBC2_UNORM_SRGB:		return DXGI_FORMAT_BC2_UNORM_SRGB;
	case GpuFormat::kBC3_UNORM:				return DXGI_FORMAT_BC3_UNORM;
	case GpuFormat::kBC3_UNORM_SRGB:		return DXGI_FORMAT_BC3_UNORM_SRGB;
	case GpuFormat::kBC4_UNORM:				return DXGI_FORMAT_BC4_UNORM;
	case GpuFormat::kBC4_SNORM:				return DXGI_FORMAT_BC4_SNORM;
	case GpuFormat::kBC5_UNORM:				return DXGI_FORMAT_BC5_UNORM;
	case GpuFormat::kBC5_SNORM:				return DXGI_FORMAT_BC5_SNORM;
	case GpuFormat::kBC6H_UF16:				return DXGI_FORMAT_BC6H_UF16;
	case GpuFormat::kBC6H_SF16:				return DXGI_FORMAT_BC6H_SF16;
	case GpuFormat::kBC7_UNORM:				return DXGI_FORMAT_BC7_UNORM;
	case GpuFormat::kBC7_UNORM_SRGB:		return DXGI_FORMAT_BC7_UNORM_SRGB;

	} // switch( format )

	eeAssertAlways( "Unsupported GpuFormat value %d", static_cast< int >( format ) );
	return DXGI_FORMAT_UNKNOWN;
}
