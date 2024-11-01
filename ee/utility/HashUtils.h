// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

namespace ee
{
	class HashUtils
	{
	public:
		static uint16_t CalculateCRC16( const uint8_t* const data, const size_t sizeInBytes );
		static uint32_t CalculateCRC32( const uint8_t* const data, const size_t sizeInBytes );
		static uint64_t CalculateCRC64( const uint8_t* const data, const size_t sizeInBytes );

	private:
		static const uint16_t CRC16Table[];
		static const uint32_t CRC32Table[];
		static const uint64_t CRC64Table[];

	}; // class HashUtils

} // namespace ee
