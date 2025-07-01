// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#include "pch.h"

#include "DataFormat.h"

using namespace ee;

// Returns 0 if there is no well-formed concept of "bits per pixel"
// (e.g. if you pass in kUnknown or a block-compressed format here)
uint32_t ee::GetBytesPerPixel( const DataFormat format )
{
	// clang-format off: preserve the formatting of this switch statement
	switch( format )
	{
	case DataFormat::kUnknown:				return 0;
	case DataFormat::kR32G32B32A32_FLOAT:	return 16;
	case DataFormat::kR32G32B32A32_UINT:	return 16;
	case DataFormat::kR32G32B32A32_SINT:	return 16;
	case DataFormat::kR16G16B16A16_FLOAT:	return 8;
	case DataFormat::kR16G16B16A16_UNORM:	return 8;
	case DataFormat::kR16G16B16A16_UINT:	return 8;
	case DataFormat::kR16G16B16A16_SNORM:	return 8;
	case DataFormat::kR16G16B16A16_SINT:	return 8;
	case DataFormat::kR32G32_FLOAT:			return 8;
	case DataFormat::kR32G32_UINT:			return 8;
	case DataFormat::kR32G32_SINT:			return 8;
	case DataFormat::kR10G10B10A2_UNORM:	return 4;
	case DataFormat::kR10G10B10A2_UINT:		return 4;
	case DataFormat::kR11G11B10_FLOAT:		return 4;
	case DataFormat::kR8G8B8A8_UNORM:		return 4;
	case DataFormat::kR8G8B8A8_UNORM_SRGB:	return 4;
	case DataFormat::kR8G8B8A8_UINT:		return 4;
	case DataFormat::kR8G8B8A8_SNORM:		return 4;
	case DataFormat::kR8G8B8A8_SINT:		return 4;
	case DataFormat::kB8G8R8A8_UNORM:		return 4;
	case DataFormat::kB8G8R8A8_UNORM_SRGB:	return 4;
	case DataFormat::kR16G16_FLOAT:			return 4;
	case DataFormat::kR16G16_UNORM:			return 4;
	case DataFormat::kR16G16_UINT:			return 4;
	case DataFormat::kR16G16_SNORM:			return 4;
	case DataFormat::kR16G16_SINT:			return 4;
	case DataFormat::kR32_FLOAT:			return 4;
	case DataFormat::kR32_UINT:				return 4;
	case DataFormat::kR32_SINT:				return 4;
	case DataFormat::kR8G8_UNORM:			return 2;
	case DataFormat::kR8G8_UINT:			return 2;
	case DataFormat::kR8G8_SNORM:			return 2;
	case DataFormat::kR8G8_SINT:			return 2;
	case DataFormat::kR16_FLOAT:			return 2;
	case DataFormat::kR16_UNORM:			return 2;
	case DataFormat::kR16_UINT:				return 2;
	case DataFormat::kR16_SNORM:			return 2;
	case DataFormat::kR16_SINT:				return 2;
	case DataFormat::kR8_UNORM:				return 1;
	case DataFormat::kR8_UINT:				return 1;
	case DataFormat::kR8_SNORM:				return 1;
	case DataFormat::kR8_SINT:				return 1;
	case DataFormat::kBC1_UNORM:			return 0;
	case DataFormat::kBC1_UNORM_SRGB:		return 0;
	case DataFormat::kBC2_UNORM:			return 0;
	case DataFormat::kBC2_UNORM_SRGB:		return 0;
	case DataFormat::kBC3_UNORM:			return 0;
	case DataFormat::kBC3_UNORM_SRGB:		return 0;
	case DataFormat::kBC4_UNORM:			return 0;
	case DataFormat::kBC4_SNORM:			return 0;
	case DataFormat::kBC5_UNORM:			return 0;
	case DataFormat::kBC5_SNORM:			return 0;
	case DataFormat::kBC6H_UF16:			return 0;
	case DataFormat::kBC6H_SF16:			return 0;
	case DataFormat::kBC7_UNORM:			return 0;
	case DataFormat::kBC7_UNORM_SRGB:		return 0;
	case DataFormat::kD32_FLOAT:			return 4;
	case DataFormat::kD24_UNORM_S8_UINT:	return 4;
	case DataFormat::kD16_UNORM:			return 2;

	} // switch( format )

	// clang-format on

	eeAssertAlways( "GetBytesPerPixel: Unsupported DataFormat value!" );
	return 0;
}

bool ee::IsDataFormatFloat( const DataFormat format )
{
	switch( format )
	{
	case DataFormat::kR32G32B32A32_FLOAT:
	case DataFormat::kR16G16B16A16_FLOAT:
	case DataFormat::kR32G32_FLOAT:
	case DataFormat::kR11G11B10_FLOAT:
	case DataFormat::kR16G16_FLOAT:
	case DataFormat::kR32_FLOAT:
	case DataFormat::kR16_FLOAT:
	case DataFormat::kBC6H_UF16:
	case DataFormat::kBC6H_SF16:
	case DataFormat::kD32_FLOAT:
		return true;

	} // switch( format )

	return false;
}

bool ee::IsDataFormatDepth( const DataFormat format )
{
	switch( format )
	{
	case DataFormat::kD32_FLOAT:
	case DataFormat::kD24_UNORM_S8_UINT:
	case DataFormat::kD16_UNORM:
		return true;

	} // switch( format )

	return false;
}

bool ee::IsDataFormatCompressed( const DataFormat format )
{
	switch( format )
	{
	case DataFormat::kBC1_UNORM:
	case DataFormat::kBC1_UNORM_SRGB:
	case DataFormat::kBC2_UNORM:
	case DataFormat::kBC2_UNORM_SRGB:
	case DataFormat::kBC3_UNORM:
	case DataFormat::kBC3_UNORM_SRGB:
	case DataFormat::kBC4_UNORM:
	case DataFormat::kBC4_SNORM:
	case DataFormat::kBC5_UNORM:
	case DataFormat::kBC5_SNORM:
	case DataFormat::kBC6H_UF16:
	case DataFormat::kBC6H_SF16:
	case DataFormat::kBC7_UNORM:
	case DataFormat::kBC7_UNORM_SRGB:
		return true;

	} // switch( format )

	return false;
}

bool ee::DataFormatHasAlpha( const DataFormat format )
{
	switch( format )
	{
	case DataFormat::kR32G32B32A32_FLOAT:
	case DataFormat::kR32G32B32A32_UINT:
	case DataFormat::kR32G32B32A32_SINT:
	case DataFormat::kR16G16B16A16_FLOAT:
	case DataFormat::kR16G16B16A16_UNORM:
	case DataFormat::kR16G16B16A16_UINT:
	case DataFormat::kR16G16B16A16_SNORM:
	case DataFormat::kR16G16B16A16_SINT:
	case DataFormat::kR10G10B10A2_UNORM:
	case DataFormat::kR10G10B10A2_UINT:
	case DataFormat::kR8G8B8A8_UNORM:
	case DataFormat::kR8G8B8A8_UNORM_SRGB:
	case DataFormat::kR8G8B8A8_UINT:
	case DataFormat::kR8G8B8A8_SNORM:
	case DataFormat::kR8G8B8A8_SINT:
	case DataFormat::kB8G8R8A8_UNORM:
	case DataFormat::kB8G8R8A8_UNORM_SRGB:
	case DataFormat::kBC2_UNORM:
	case DataFormat::kBC2_UNORM_SRGB:
	case DataFormat::kBC3_UNORM:
	case DataFormat::kBC3_UNORM_SRGB:
	// yeah, yeah, I know that BC7 is a "maybe" if it has alpha
	case DataFormat::kBC7_UNORM:
	case DataFormat::kBC7_UNORM_SRGB:
		return true;

	} // switch( format )

	return false;
}

#if !defined( EE_BUILD_RETAIL )

const char* DataFormatToString( const DataFormat format )
{
	// clang-format off: preserve the formatting of this switch statement
	switch( format )
	{
	TOKEN_TO_STRING( DataFormat::kUnknown )
	TOKEN_TO_STRING( DataFormat::kR32G32B32A32_FLOAT )
	TOKEN_TO_STRING( DataFormat::kR32G32B32A32_UINT )
	TOKEN_TO_STRING( DataFormat::kR32G32B32A32_SINT )
	TOKEN_TO_STRING( DataFormat::kR16G16B16A16_FLOAT )
	TOKEN_TO_STRING( DataFormat::kR16G16B16A16_UNORM )
	TOKEN_TO_STRING( DataFormat::kR16G16B16A16_UINT )
	TOKEN_TO_STRING( DataFormat::kR16G16B16A16_SNORM )
	TOKEN_TO_STRING( DataFormat::kR16G16B16A16_SINT )
	TOKEN_TO_STRING( DataFormat::kR32G32_FLOAT )
	TOKEN_TO_STRING( DataFormat::kR32G32_UINT )
	TOKEN_TO_STRING( DataFormat::kR32G32_SINT )
	TOKEN_TO_STRING( DataFormat::kR10G10B10A2_UNORM )
	TOKEN_TO_STRING( DataFormat::kR10G10B10A2_UINT )
	TOKEN_TO_STRING( DataFormat::kR11G11B10_FLOAT )
	TOKEN_TO_STRING( DataFormat::kR8G8B8A8_UNORM )
	TOKEN_TO_STRING( DataFormat::kR8G8B8A8_UNORM_SRGB )
	TOKEN_TO_STRING( DataFormat::kR8G8B8A8_UINT )
	TOKEN_TO_STRING( DataFormat::kR8G8B8A8_SNORM )
	TOKEN_TO_STRING( DataFormat::kR8G8B8A8_SINT )
	TOKEN_TO_STRING( DataFormat::kB8G8R8A8_UNORM )
	TOKEN_TO_STRING( DataFormat::kB8G8R8A8_UNORM_SRGB )
	TOKEN_TO_STRING( DataFormat::kR16G16_FLOAT )
	TOKEN_TO_STRING( DataFormat::kR16G16_UNORM )
	TOKEN_TO_STRING( DataFormat::kR16G16_UINT )
	TOKEN_TO_STRING( DataFormat::kR16G16_SNORM )
	TOKEN_TO_STRING( DataFormat::kR16G16_SINT )
	TOKEN_TO_STRING( DataFormat::kR32_FLOAT )
	TOKEN_TO_STRING( DataFormat::kR32_UINT )
	TOKEN_TO_STRING( DataFormat::kR32_SINT )
	TOKEN_TO_STRING( DataFormat::kR8G8_UNORM )
	TOKEN_TO_STRING( DataFormat::kR8G8_UINT )
	TOKEN_TO_STRING( DataFormat::kR8G8_SNORM )
	TOKEN_TO_STRING( DataFormat::kR8G8_SINT )
	TOKEN_TO_STRING( DataFormat::kR16_FLOAT )
	TOKEN_TO_STRING( DataFormat::kR16_UNORM )
	TOKEN_TO_STRING( DataFormat::kR16_UINT )
	TOKEN_TO_STRING( DataFormat::kR16_SNORM )
	TOKEN_TO_STRING( DataFormat::kR16_SINT )
	TOKEN_TO_STRING( DataFormat::kR8_UNORM )
	TOKEN_TO_STRING( DataFormat::kR8_UINT )
	TOKEN_TO_STRING( DataFormat::kR8_SNORM )
	TOKEN_TO_STRING( DataFormat::kR8_SINT )
	TOKEN_TO_STRING( DataFormat::kBC1_UNORM )
	TOKEN_TO_STRING( DataFormat::kBC1_UNORM_SRGB )
	TOKEN_TO_STRING( DataFormat::kBC2_UNORM )
	TOKEN_TO_STRING( DataFormat::kBC2_UNORM_SRGB )
	TOKEN_TO_STRING( DataFormat::kBC3_UNORM )
	TOKEN_TO_STRING( DataFormat::kBC3_UNORM_SRGB )
	TOKEN_TO_STRING( DataFormat::kBC4_UNORM )
	TOKEN_TO_STRING( DataFormat::kBC4_SNORM )
	TOKEN_TO_STRING( DataFormat::kBC5_UNORM )
	TOKEN_TO_STRING( DataFormat::kBC5_SNORM )
	TOKEN_TO_STRING( DataFormat::kBC6H_UF16 )
	TOKEN_TO_STRING( DataFormat::kBC6H_SF16 )
	TOKEN_TO_STRING( DataFormat::kBC7_UNORM )
	TOKEN_TO_STRING( DataFormat::kBC7_UNORM_SRGB )
	TOKEN_TO_STRING( DataFormat::kD32_FLOAT )
	TOKEN_TO_STRING( DataFormat::kD24_UNORM_S8_UINT )
	TOKEN_TO_STRING( DataFormat::kD16_UNORM )

	} // switch( format )

	// clang-format on

	eeAssertAlways( "DataFormatToString: Unknown DataFormat value!" );
	return "<Unknown DataFormat value>";
}

#endif // #if !defined( EE_BUILD_RETAIL )
