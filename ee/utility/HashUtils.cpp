// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#include "pch.h"

#include <ee/core/Debug.h>
#include <ee/utility/HashUtils.h>

namespace ee {

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
	uint16_t crc = 0xFFFF;

	for( size_t i = 0; i < length; ++i )
	{
		crc = ( crc >> 8 ) ^ HashUtils::CRC16Table[ ( crc ^ data[ i ] ) & 0xFF ];
	}

	return ( crc ^ 0xFFFF );
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
	uint32_t crc = 0xFFFFFFFF;

	for( size_t i = 0; i < length; ++i )
	{
		crc = ( crc >> 8 ) ^ HashUtils::CRC32Table[ ( crc ^ data[ i ] ) & 0xFF ];
	}

	return ( crc ^ 0xFFFFFFFF );
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
	uint64_t crc = 0xFFFFFFFFFFFFFFFF;

	for( size_t i = 0; i < length; ++i )
	{
		crc = ( crc >> 8 ) ^ HashUtils::CRC64Table[ ( crc ^ data[ i ] ) & 0xFF ];
	}

	return ( crc ^ 0xFFFFFFFFFFFFFFFF );
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

		fprintf( file, "#pragma once\n\n" );
		fprintf( file, "//\n// Generated CRC%d data table; see HashUtils.cpp\n", bitcount );
		fprintf( file, "//\n\n" );

		fprintf( file, "static const %s HashUtils::CRC%dTable[] =\n", typeString, bitcount );
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

		fprintf( file, "};\n" );

		fclose( file );
	}

	void GenerateCrc16Table( const char* const filename )
	{
		CRCTableGenerator16 generator = {
			16,
			0x8408,
			0xFFFF,
			true
		};

		GenerateCrcTable< uint16_t >( generator, filename );
	}

	void GenerateCrc32Table( const char* const filename )
	{
		CRCTableGenerator32 generator = {
			32,
			0x04C11DB7,
			0xFFFFFFFF,
			true
		};

		GenerateCrcTable< uint32_t >( generator, filename );
	}

	void GenerateCrc64Table( const char* const filename )
	{
		CRCTableGenerator64 generator = {
			64,
			0x0060034000F0D50B,
			0xFFFFFFFFFFFFFFFF,
			true
		};

		GenerateCrcTable< uint64_t >( generator, filename );
	}

} // namespace eeCRCTableGenerator

#endif // #if defined( GENERATE_CRC_TABLES )
