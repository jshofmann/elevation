// Elevation Engine
//
// Copyright (c) 2026 Azimuth Studios

#pragma once

#include <stdint.h>

namespace ee
{
	// An enumeration of the various formats that data
	// stored in GPU-addressable buffers can be in.
	enum class GpuFormat
	{
		kUnknown			 = 0,
		kR32G32B32A32_FLOAT	 = 1,
		kR32G32B32A32_UINT	 = 2,
		kR32G32B32A32_SINT	 = 3,
		kR16G16B16A16_FLOAT	 = 4,
		kR16G16B16A16_UINT	 = 5,
		kR16G16B16A16_SINT	 = 6,
		kR16G16B16A16_UNORM	 = 7,
		kR16G16B16A16_SNORM	 = 8,
		kR10G10B10A2_UINT	 = 9,
		kR10G10B10A2_UNORM	 = 10,
		kR8G8B8A8_UINT		 = 11,
		kR8G8B8A8_SINT		 = 12,
		kR8G8B8A8_UNORM		 = 13,
		kR8G8B8A8_UNORM_SRGB = 14,
		kR8G8B8A8_SNORM		 = 15,
		kB8G8R8A8_UNORM		 = 16,
		kB8G8R8A8_UNORM_SRGB = 17,
		kR32G32B32_FLOAT	 = 18,
		kR32G32B32_UINT		 = 19,
		kR32G32B32_SINT		 = 20,
		kR11G11B10_FLOAT	 = 21,
		kR32G32_FLOAT		 = 22,
		kR32G32_UINT		 = 23,
		kR32G32_SINT		 = 24,
		kR16G16_FLOAT		 = 25,
		kR16G16_UINT		 = 26,
		kR16G16_SINT		 = 27,
		kR16G16_UNORM		 = 28,
		kR16G16_SNORM		 = 29,
		kR8G8_UINT			 = 30,
		kR8G8_SINT			 = 31,
		kR8G8_UNORM			 = 32,
		kR8G8_SNORM			 = 33,
		kR32_FLOAT			 = 34,
		kR32_UINT			 = 35,
		kR32_SINT			 = 36,
		kR16_FLOAT			 = 37,
		kR16_UINT			 = 38,
		kR16_SINT			 = 39,
		kR16_UNORM			 = 40,
		kR16_SNORM			 = 41,
		kR8_UINT			 = 42,
		kR8_SINT			 = 43,
		kR8_UNORM			 = 44,
		kR8_SNORM			 = 45,
		kD32_FLOAT			 = 46,
		kD24_UNORM_S8_UINT	 = 47,
		kD16_UNORM			 = 48,
		kBC1_UNORM			 = 49,
		kBC1_UNORM_SRGB		 = 50,
		kBC2_UNORM			 = 51,
		kBC2_UNORM_SRGB		 = 52,
		kBC3_UNORM			 = 53,
		kBC3_UNORM_SRGB		 = 54,
		kBC4_UNORM			 = 55,
		kBC4_SNORM			 = 56,
		kBC5_UNORM			 = 57,
		kBC5_SNORM			 = 58,
		kBC6H_UF16			 = 59,
		kBC6H_SF16			 = 60,
		kBC7_UNORM			 = 61,
		kBC7_UNORM_SRGB		 = 62,
		kGpuFormatCount

	}; // enum class GpuFormat

	// Returns 0 if there is no well-formed concept of "bytes per pixel"
	// (e.g. if you pass in kUnknown or a block-compressed format here)
	uint32_t GetBytesPerPixel( const GpuFormat format );

	bool IsGpuFormatDepth( const GpuFormat format );
	bool IsGpuFormatCompressed( const GpuFormat format );

	const char* GpuFormatToString( const GpuFormat format );

} // namespace ee
