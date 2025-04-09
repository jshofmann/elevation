// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#include "pch.h"

#include <ee/core/Debug.h>
#include <ee/utility/HashUtils.h>

namespace ee {

namespace HashUtils
{
	constexpr uint16_t CRC16InitialValue = 0xFFFF;
	constexpr uint32_t CRC32InitialValue = 0xFFFFFFFF;
	constexpr uint64_t CRC64InitialValue = 0xFFFFFFFFFFFFFFFFull;

} // namespace HashUtils

// This code is derived from the public domain CRC-32 v1.04 program
// written by Craig Bruce on 5-Dec-1994 and available at
// http://www.csbruce.com/~csbruce/software/crc32.c. His attribution:
//
// "Based on 'File Verification Using CRC' by Mark R. Nelson in Dr. Dobb's
//	Journal, May 1992, pp. 64-67.  This program DOES generate the same CRC
//	values as GZIP, PKZIP, and ZMODEM."
//
// Additional features are derived from the paper "A Painless Guide to CRC
// Error Detection Algorithms" by Ross N. Williams and code posted at
// http://www.ross.net/crc/download/crc_v3.txt.

#include "CRC16.inl"
#include "CRC32.inl"
#include "CRC64.inl"

// HashUtils::CalculateCRC16( const uint8_t* const data, const size_t length )
//
// Calculate and return the CRC-16 hash code for the given data.
//
//	-> data	  -- The byte array to generate the hash code for
//	-> length -- The size in bytes of the data array
// <-		  -- The CRC-16 hash code for the data array
uint16_t HashUtils::CalculateCRC16( const uint8_t* const data, const size_t length )
{
	uint16_t crc = CRC16InitialValue;
	const uint8_t* current = data;
	size_t count = length;

	while( count-- )
	{
		crc = ( CRC16Table[ ( crc ^ *current++ ) & 0xFFL ] ^ ( crc >> 8 ) );
	}

	return ( crc ^ CRC16InitialValue );
}


// HashUtils::CalculateCRC32( const uint8_t* const data, const size_t length )
//
// Calculate and return the CRC-32 hash code for the given data.
//
//	-> data	  -- The byte array to generate the hash code for
//	-> length -- The size in bytes of the data array
// <-		  -- The CRC-32 hash code for the data array
uint32_t HashUtils::CalculateCRC32( const uint8_t* const data, const size_t length )
{
	uint32_t crc = CRC32InitialValue;
	const uint8_t* current = data;
	size_t count = length;

	while( count-- )
	{
		crc = ( CRC32Table[ ( crc ^ *current++ ) & 0xFFL ] ^ ( crc >> 8 ) );
	}

	return ( crc ^ CRC32InitialValue );
}


// HashUtils::CalculateCRC64( const uint8_t* const data, const size_t length )
//
// Calculate and return the CRC-64 hash code for the given data.
//
//	-> data	  -- The byte array to generate the hash code for
//	-> length -- The size in bytes of the data array
// <-		  -- The CRC-64 hash code for the data array
uint64_t HashUtils::CalculateCRC64( const uint8_t* const data, const size_t length )
{
	uint64_t crc = CRC64InitialValue;
	const uint8_t* current = data;
	size_t count = length;

	while( count-- )
	{
		crc = ( CRC64Table[ ( crc ^ *current++ ) & 0xFFL ] ^ ( crc >> 8 ) );
	}

	return ( crc ^ CRC64InitialValue );
}


// HashUtils::CalculateStringHash( const std::string_view& string )
//
// Calculate and return a hash value of the given string. I use CRC64 hashes
// for strings, but that should be considered an implementation detail -
// I may switch to a DJB hash function, for example. DJB hashes are quicker,
// and they have a nice uniform distribution, but there is an increased chance
// of collision vs a CRC64 hash. (This has been observed in a large string
// test database.)
//
//	-> string -- A string to hash
// <-		  -- The hash code for this string
uint64_t HashUtils::CalculateStringHash( const std::string_view& string )
{
	return CalculateCRC64( reinterpret_cast< const uint8_t* >( string.data() ), string.length() );
}


// HashUtils::CalculateLowercaseStringHash( const std::string_view& string )
//
// Calculate and return a case-insensitive hash value of the given string.
// I use CRC64 hashes for strings, but that should be considered an
// implementation detail. This function assumes that the string is limited
// to the ASCII character set. Case-insensitivity is achieved by downcasing
// every letter; downcasing is done by the simple 'A' + 32 = 'a' property,
// which results in the limitation to just ASCII characters. std::tolower()
// is avoided as it does a locale check, making it much slower than 'A' + 32.
//
//	-> string -- A string to hash
// <-		  -- The case-insensitive hash code for this string
uint64_t HashUtils::CalculateLowercaseStringHash( const std::string_view& string )
{
	uint64_t crc = CRC64InitialValue;
	const char* current = string.data();
	size_t count = string.length();

	while( count-- )
	{
		// !!! NOTE: We aren't using std::tolower() here because we don't
		// !!! want the overhead of its locale checks. This means we're
		// !!! assuming our strings are limited to the ASCII character set.
		char c = *current++;
		if( c >= 'A' && c <= 'Z' )
			c += 32; // downcase this capital letter

		crc = ( CRC64Table[ ( crc ^ c ) & 0xFFL ] ^ ( crc >> 8 ) );
	}

	return ( crc ^ CRC64InitialValue );
}

} // namespace ee

#if defined( GENERATE_CRC_TABLES )

#include <stdio.h>
#include <string.h>

// This code is derived from details published in the paper "A Painless Guide
// to CRC Error Detection Algorithms" by Ross N. Williams and code posted to
// http://www.ross.net/crc/download/crc_v3.txt.

namespace eeCRCTableGenerator
{
	template< typename size >
	struct CRCTableGenerator
	{
		typedef size SizeType;

		uint32_t		mWidth;
		SizeType		mPolynomial;
		SizeType		mMask;
		bool			mReflected;

		static SizeType GetBitMask( const uint32_t value )
		{
			return ( SizeType( 1 ) << value );
		}

		static SizeType Reflect( const SizeType value, const uint32_t bits )
		{
			size result	= value;
			size t		= value;

			for( uint32_t i = 0; i < bits; ++i )
			{
				if( t & SizeType( 1 ) )
					result |= GetBitMask ( ( bits - 1 ) - i );
				else
					result &= ~GetBitMask ( ( bits - 1 ) - i );

				t >>= 1;
			}

			return result;
		}

		SizeType GetWidthMask( void )
		{
			// Trickery used to prevent overflow...
			return SizeType( ( ( ( 1 << ( SizeType( mWidth - 1 ) ) ) - 1 ) << 1 ) | 1 );
		}

		SizeType GetTableEntry( const size index )
		{
			SizeType topbit = GetBitMask( mWidth - 1 );
			SizeType inbyte = SizeType( index );

			if( mReflected )
				inbyte = Reflect( inbyte, 8 );

			SizeType r = inbyte << ( mWidth - 8 );

			for( int i = 0; i < 8; ++i )
			{
				if( r & topbit )
					r = ( ( r << 1 ) ^ mPolynomial ) & mMask;
				else
					r <<= 1;
			}

			if( mReflected )
				r = Reflect( r, mWidth );

			return ( r & GetWidthMask() );
		}
	};

	typedef CRCTableGenerator< uint16_t > CRCTableGenerator16;
	typedef CRCTableGenerator< uint32_t > CRCTableGenerator32;
	typedef CRCTableGenerator< uint64_t > CRCTableGenerator64;

	template< typename SizeType >
	void GenerateCrcTable( CRCTableGenerator< SizeType >& generator, const char* const filename )
	{
		FILE* file;
		if( fopen_s( &file, filename, "w" ) != 0 )
			return;

		uint32_t bitcount = 8 * sizeof( SizeType );
		char typeString[ 64 ];

		switch( bitcount )
		{
		case 16: strcpy_s( typeString, "uint16_t" ); break;
		case 32: strcpy_s( typeString, "uint32_t" ); break;
		case 64: strcpy_s( typeString, "uint64_t" ); break;
		}

		fprintf( file, "// Elevation Engine\n" );
		fprintf( file, "//\n" );
		fprintf( file, "// Copyright (c) 2025 Azimuth Studios\n\n" );

		fprintf( file, "#pragma once\n\n" );
		fprintf( file, "//\n// Generated CRC%d data table; see HashUtils.cpp\n", bitcount );
		fprintf( file, "//\n\n" );

		fprintf( file, "namespace HashUtils {\n\n" );

		fprintf( file, "static const %s CRC%dTable[] =\n", typeString, bitcount );
		fprintf( file, "{\n" );

		SizeType index = 0;
		while( index < 256 )
		{
			SizeType tableWidth = ( bitcount == 16 ) ? 16 : ( ( bitcount == 32 ) ? 8 : 4 );

			for( SizeType i = 0; i < tableWidth; ++i )
			{
				SizeType value = generator.GetTableEntry( index + i );

				switch( bitcount )
				{
				case 16: fprintf( file, "\t0x%04lX,", value );    break;
				case 32: fprintf( file, "\t0x%08lX,", value );    break;
				case 64: fprintf( file, "\t0x%016I64X,", value ); break;
				}
			}

			fprintf( file, "\n" );

			index += tableWidth;
		}

		fprintf( file, "};\n\n" );

		fprintf( file, "} // namespace HashUtils\n" );

		fclose( file );
	}

	void GenerateCrc16Table( const char* const filename )
	{
		CRCTableGenerator16 generator = {
			.mWidth = 16,
			.mPolynomial = 0x8408,
			.mMask = 0xFFFF,
			.mReflected = true
		};

		GenerateCrcTable< uint16_t >( generator, filename );
	}

	void GenerateCrc32Table( const char* const filename )
	{
		CRCTableGenerator32 generator = {
			.mWidth = 32,
			.mPolynomial = 0x04C11DB7,
			.mMask = 0xFFFFFFFF,
			.mReflected = true
		};

		GenerateCrcTable< uint32_t >( generator, filename );
	}

	void GenerateCrc64Table( const char* const filename )
	{
		CRCTableGenerator64 generator = {
			.mWidth = 64,
			.mPolynomial = 0x0060034000F0D50Bull,
			.mMask = 0xFFFFFFFFFFFFFFFFull,
			.mReflected = true
		};

		GenerateCrcTable< uint64_t >( generator, filename );
	}

} // namespace eeCRCTableGenerator

#endif // #if defined( GENERATE_CRC_TABLES )
