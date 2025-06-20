// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

namespace ee
{
	enum class DataFormat final
	{
		kUnknown			 = 0,
		kR32G32B32A32_FLOAT	 = 1,
		kR32G32B32A32_UINT	 = 2,
		kR32G32B32A32_SINT	 = 3,
		kR16G16B16A16_FLOAT	 = 4,
		kR16G16B16A16_UNORM	 = 5,
		kR16G16B16A16_UINT	 = 6,
		kR16G16B16A16_SNORM	 = 7,
		kR16G16B16A16_SINT	 = 8,
		kR32G32_FLOAT		 = 9,
		kR32G32_UINT		 = 10,
		kR32G32_SINT		 = 11,
		kR10G10B10A2_UNORM	 = 12,
		kR10G10B10A2_UINT	 = 13,
		kR11G11B10_FLOAT	 = 14,
		kR8G8B8A8_UNORM		 = 15,
		kR8G8B8A8_UNORM_SRGB = 16,
		kR8G8B8A8_UINT		 = 17,
		kR8G8B8A8_SNORM		 = 18,
		kR8G8B8A8_SINT		 = 19,
		kB8G8R8A8_UNORM		 = 20,
		kB8G8R8A8_UNORM_SRGB = 21,
		kR16G16_FLOAT		 = 22,
		kR16G16_UNORM		 = 23,
		kR16G16_UINT		 = 24,
		kR16G16_SNORM		 = 25,
		kR16G16_SINT		 = 26,
		kR32_FLOAT			 = 27,
		kR32_UINT			 = 28,
		kR32_SINT			 = 29,
		kR8G8_UNORM			 = 30,
		kR8G8_UINT			 = 31,
		kR8G8_SNORM			 = 32,
		kR8G8_SINT			 = 33,
		kR16_FLOAT			 = 34,
		kR16_UNORM			 = 35,
		kR16_UINT			 = 36,
		kR16_SNORM			 = 37,
		kR16_SINT			 = 38,
		kR8_UNORM			 = 39,
		kR8_UINT			 = 40,
		kR8_SNORM			 = 41,
		kR8_SINT			 = 42,
		kBC1_UNORM			 = 43,
		kBC1_UNORM_SRGB		 = 44,
		kBC2_UNORM			 = 45,
		kBC2_UNORM_SRGB		 = 46,
		kBC3_UNORM			 = 47,
		kBC3_UNORM_SRGB		 = 48,
		kBC4_UNORM			 = 49,
		kBC4_SNORM			 = 50,
		kBC5_UNORM			 = 51,
		kBC5_SNORM			 = 52,
		kBC6H_UF16			 = 53,
		kBC6H_SF16			 = 54,
		kBC7_UNORM			 = 55,
		kBC7_UNORM_SRGB		 = 56,
		kD32_FLOAT			 = 57,
		kD24_UNORM_S8_UINT	 = 58,
		kD16_UNORM			 = 59

	}; // enum class DataFormat

	// Returns 0 if there is no well-formed concept of "bits per pixel"
	// (e.g. if you pass in kUnknown or a block-compressed format here)
	uint32_t GetBytesPerPixel( const DataFormat format );

	inline uint32_t GetBitsPerPixel( const DataFormat format )
	{
		return GetBytesPerPixel( format ) * 8;
	}

	bool IsDataFormatFloat( const DataFormat format );
	bool IsDataFormatDepth( const DataFormat format );
	bool IsDataFormatCompressed( const DataFormat format );
	bool DataFormatHasAlpha( const DataFormat format );

#if !defined( EE_BUILD_RETAIL )
	const char* DataFormatToString( const DataFormat format );
#endif

} // namespace ee
