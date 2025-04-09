// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

#include <string_view>

namespace ee
{
	namespace HashUtils
	{
		uint16_t CalculateCRC16( const uint8_t* const data, const size_t sizeInBytes );
		uint32_t CalculateCRC32( const uint8_t* const data, const size_t sizeInBytes );
		uint64_t CalculateCRC64( const uint8_t* const data, const size_t sizeInBytes );

		// I use CRC64 hashes for strings, but that should be considered an
		// implementation detail - I may switch  to a DJB hash function,
		// for example. DJB hashes are quicker, and they have a nice
		// uniform distribution, but there is an increased chance of collision
		// vs a CRC64 hash. (A collision has been observed in a large string
		// test database.) These functions assume that the string is limited
		// to the ASCII character set; downcasing avoids using std::tolower()
		// as that function's locale tests make it much slower than the simple
		// old-school 'A' + 32 = 'a' downcasing possible if ASCII is assumed.
		//
		// CalculateStringHash: Hash a string as-is
		// CalculateLowercaseStringHash: Return a case-insensitive string hash
		uint64_t CalculateStringHash( const std::string_view& string );
		uint64_t CalculateLowercaseStringHash( const std::string_view& string );

	} // namespace HashUtils

} // namespace ee
