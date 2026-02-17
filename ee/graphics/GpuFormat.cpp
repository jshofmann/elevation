// Elevation Engine
//
// Copyright (c) 2026 Azimuth Studios

#include "pch.h"

#include "GpuFormat.h"

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch" // enumeration values not handled in switch
#endif

using namespace ee;

// Returns 0 if there is no well-formed concept of "bytes per pixel"
// (e.g. if you pass in kUnknown or a block-compressed format here)
uint32_t ee::GetBytesPerPixel( const GpuFormat format )
{
	// clang-format off: preserve the formatting of this switch statement
	switch( format )
	{
	case GpuFormat::kUnknown:				return 0;
	case GpuFormat::kR32G32B32A32_FLOAT:	return 16;
	case GpuFormat::kR32G32B32A32_UINT:		return 16;
	case GpuFormat::kR32G32B32A32_SINT:		return 16;
	case GpuFormat::kR16G16B16A16_FLOAT:	return 8;
	case GpuFormat::kR16G16B16A16_UINT:		return 8;
	case GpuFormat::kR16G16B16A16_SINT:		return 8;
	case GpuFormat::kR16G16B16A16_UNORM:	return 8;
	case GpuFormat::kR16G16B16A16_SNORM:	return 8;
	case GpuFormat::kR10G10B10A2_UINT:		return 4;
	case GpuFormat::kR10G10B10A2_UNORM:		return 4;
	case GpuFormat::kR8G8B8A8_UINT:			return 4;
	case GpuFormat::kR8G8B8A8_SINT:			return 4;
	case GpuFormat::kR8G8B8A8_UNORM:		return 4;
	case GpuFormat::kR8G8B8A8_UNORM_SRGB:	return 4;
	case GpuFormat::kR8G8B8A8_SNORM:		return 4;
	case GpuFormat::kB8G8R8A8_UNORM:		return 4;
	case GpuFormat::kB8G8R8A8_UNORM_SRGB:	return 4;
	case GpuFormat::kR32G32B32_FLOAT:		return 12;
	case GpuFormat::kR32G32B32_UINT:		return 12;
	case GpuFormat::kR32G32B32_SINT:		return 12;
	case GpuFormat::kR11G11B10_FLOAT:		return 4;
	case GpuFormat::kR32G32_FLOAT:			return 8;
	case GpuFormat::kR32G32_UINT:			return 8;
	case GpuFormat::kR32G32_SINT:			return 8;
	case GpuFormat::kR16G16_FLOAT:			return 4;
	case GpuFormat::kR16G16_UINT:			return 4;
	case GpuFormat::kR16G16_SINT:			return 4;
	case GpuFormat::kR16G16_UNORM:			return 4;
	case GpuFormat::kR16G16_SNORM:			return 4;
	case GpuFormat::kR8G8_UINT:				return 2;
	case GpuFormat::kR8G8_SINT:				return 2;
	case GpuFormat::kR8G8_UNORM:			return 2;
	case GpuFormat::kR8G8_SNORM:			return 2;
	case GpuFormat::kR32_FLOAT:				return 4;
	case GpuFormat::kR32_UINT:				return 4;
	case GpuFormat::kR32_SINT:				return 4;
	case GpuFormat::kR16_FLOAT:				return 2;
	case GpuFormat::kR16_UINT:				return 2;
	case GpuFormat::kR16_SINT:				return 2;
	case GpuFormat::kR16_UNORM:				return 2;
	case GpuFormat::kR16_SNORM:				return 2;
	case GpuFormat::kR8_UINT:				return 1;
	case GpuFormat::kR8_SINT:				return 1;
	case GpuFormat::kR8_UNORM:				return 1;
	case GpuFormat::kR8_SNORM:				return 1;
	case GpuFormat::kD32_FLOAT:				return 4;
	case GpuFormat::kD24_UNORM_S8_UINT:		return 4;
	case GpuFormat::kD16_UNORM:				return 2;
	case GpuFormat::kBC1_UNORM:				return 0;
	case GpuFormat::kBC1_UNORM_SRGB:		return 0;
	case GpuFormat::kBC2_UNORM:				return 0;
	case GpuFormat::kBC2_UNORM_SRGB:		return 0;
	case GpuFormat::kBC3_UNORM:				return 0;
	case GpuFormat::kBC3_UNORM_SRGB:		return 0;
	case GpuFormat::kBC4_UNORM:				return 0;
	case GpuFormat::kBC4_SNORM:				return 0;
	case GpuFormat::kBC5_UNORM:				return 0;
	case GpuFormat::kBC5_SNORM:				return 0;
	case GpuFormat::kBC6H_UF16:				return 0;
	case GpuFormat::kBC6H_SF16:				return 0;
	case GpuFormat::kBC7_UNORM:				return 0;
	case GpuFormat::kBC7_UNORM_SRGB:		return 0;

	} // switch( format )

	// clang-format on

	eeAssertAlways( "GetBytesPerPixel: Unsupported GpuFormat value!" );
	return 0;
}

bool ee::IsGpuFormatDepth( const GpuFormat format )
{
	switch( format )
	{
	case GpuFormat::kD32_FLOAT:
	case GpuFormat::kD24_UNORM_S8_UINT:
	case GpuFormat::kD16_UNORM:
		return true;

	} // switch( format )

	return false;
}

bool ee::IsGpuFormatCompressed( const GpuFormat format )
{
	switch( format )
	{
	case GpuFormat::kBC1_UNORM:
	case GpuFormat::kBC1_UNORM_SRGB:
	case GpuFormat::kBC2_UNORM:
	case GpuFormat::kBC2_UNORM_SRGB:
	case GpuFormat::kBC3_UNORM:
	case GpuFormat::kBC3_UNORM_SRGB:
	case GpuFormat::kBC4_UNORM:
	case GpuFormat::kBC4_SNORM:
	case GpuFormat::kBC5_UNORM:
	case GpuFormat::kBC5_SNORM:
	case GpuFormat::kBC6H_UF16:
	case GpuFormat::kBC6H_SF16:
	case GpuFormat::kBC7_UNORM:
	case GpuFormat::kBC7_UNORM_SRGB:
		return true;

	} // switch( format )

	return false;
}

const char* ee::GpuFormatToString( const GpuFormat format )
{
	switch( format )
	{
	TOKEN_TO_STRING( GpuFormat::kUnknown )
	TOKEN_TO_STRING( GpuFormat::kR32G32B32A32_FLOAT )
	TOKEN_TO_STRING( GpuFormat::kR32G32B32A32_UINT )
	TOKEN_TO_STRING( GpuFormat::kR32G32B32A32_SINT )
	TOKEN_TO_STRING( GpuFormat::kR16G16B16A16_FLOAT )
	TOKEN_TO_STRING( GpuFormat::kR16G16B16A16_UINT )
	TOKEN_TO_STRING( GpuFormat::kR16G16B16A16_SINT )
	TOKEN_TO_STRING( GpuFormat::kR16G16B16A16_UNORM )
	TOKEN_TO_STRING( GpuFormat::kR16G16B16A16_SNORM )
	TOKEN_TO_STRING( GpuFormat::kR10G10B10A2_UINT )
	TOKEN_TO_STRING( GpuFormat::kR10G10B10A2_UNORM )
	TOKEN_TO_STRING( GpuFormat::kR8G8B8A8_UINT )
	TOKEN_TO_STRING( GpuFormat::kR8G8B8A8_SINT )
	TOKEN_TO_STRING( GpuFormat::kR8G8B8A8_UNORM )
	TOKEN_TO_STRING( GpuFormat::kR8G8B8A8_UNORM_SRGB )
	TOKEN_TO_STRING( GpuFormat::kR8G8B8A8_SNORM )
	TOKEN_TO_STRING( GpuFormat::kB8G8R8A8_UNORM )
	TOKEN_TO_STRING( GpuFormat::kB8G8R8A8_UNORM_SRGB )
	TOKEN_TO_STRING( GpuFormat::kR32G32B32_FLOAT )
	TOKEN_TO_STRING( GpuFormat::kR32G32B32_UINT )
	TOKEN_TO_STRING( GpuFormat::kR32G32B32_SINT )
	TOKEN_TO_STRING( GpuFormat::kR11G11B10_FLOAT )
	TOKEN_TO_STRING( GpuFormat::kR32G32_FLOAT )
	TOKEN_TO_STRING( GpuFormat::kR32G32_UINT )
	TOKEN_TO_STRING( GpuFormat::kR32G32_SINT )
	TOKEN_TO_STRING( GpuFormat::kR16G16_FLOAT )
	TOKEN_TO_STRING( GpuFormat::kR16G16_UINT )
	TOKEN_TO_STRING( GpuFormat::kR16G16_SINT )
	TOKEN_TO_STRING( GpuFormat::kR16G16_UNORM )
	TOKEN_TO_STRING( GpuFormat::kR16G16_SNORM )
	TOKEN_TO_STRING( GpuFormat::kR8G8_UINT )
	TOKEN_TO_STRING( GpuFormat::kR8G8_SINT )
	TOKEN_TO_STRING( GpuFormat::kR8G8_UNORM )
	TOKEN_TO_STRING( GpuFormat::kR8G8_SNORM )
	TOKEN_TO_STRING( GpuFormat::kR32_FLOAT )
	TOKEN_TO_STRING( GpuFormat::kR32_UINT )
	TOKEN_TO_STRING( GpuFormat::kR32_SINT )
	TOKEN_TO_STRING( GpuFormat::kR16_FLOAT )
	TOKEN_TO_STRING( GpuFormat::kR16_UINT )
	TOKEN_TO_STRING( GpuFormat::kR16_SINT )
	TOKEN_TO_STRING( GpuFormat::kR16_UNORM )
	TOKEN_TO_STRING( GpuFormat::kR16_SNORM )
	TOKEN_TO_STRING( GpuFormat::kR8_UINT )
	TOKEN_TO_STRING( GpuFormat::kR8_SINT )
	TOKEN_TO_STRING( GpuFormat::kR8_UNORM )
	TOKEN_TO_STRING( GpuFormat::kR8_SNORM )
	TOKEN_TO_STRING( GpuFormat::kD32_FLOAT )
	TOKEN_TO_STRING( GpuFormat::kD24_UNORM_S8_UINT )
	TOKEN_TO_STRING( GpuFormat::kD16_UNORM )
	TOKEN_TO_STRING( GpuFormat::kBC1_UNORM )
	TOKEN_TO_STRING( GpuFormat::kBC1_UNORM_SRGB )
	TOKEN_TO_STRING( GpuFormat::kBC2_UNORM )
	TOKEN_TO_STRING( GpuFormat::kBC2_UNORM_SRGB )
	TOKEN_TO_STRING( GpuFormat::kBC3_UNORM )
	TOKEN_TO_STRING( GpuFormat::kBC3_UNORM_SRGB )
	TOKEN_TO_STRING( GpuFormat::kBC4_UNORM )
	TOKEN_TO_STRING( GpuFormat::kBC4_SNORM )
	TOKEN_TO_STRING( GpuFormat::kBC5_UNORM )
	TOKEN_TO_STRING( GpuFormat::kBC5_SNORM )
	TOKEN_TO_STRING( GpuFormat::kBC6H_UF16 )
	TOKEN_TO_STRING( GpuFormat::kBC6H_SF16 )
	TOKEN_TO_STRING( GpuFormat::kBC7_UNORM )
	TOKEN_TO_STRING( GpuFormat::kBC7_UNORM_SRGB )
	TOKEN_TO_STRING( GpuFormat::kGpuFormatCount )

	} // switch( format )

	eeAssertAlways( "GpuFormatToString: Unsupported GpuFormat value!" );
	return "Unknown GpuFormat value";
}

#if defined(__clang__)
#pragma clang diagnostic pop
#endif
