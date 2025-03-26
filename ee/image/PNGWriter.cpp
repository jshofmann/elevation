// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#include "pch.h"

#include <stdio.h>

#include "PNGWriter.h"

#include <ee/utility/HashUtils.h>
#include <ee/core/Debug.h>

using namespace ee;

// Note: This is a very simple and limited PNG writer. It will encode a single
// uncompressed image into an uncompressed deflate block. Don't use it for
// something you intend to embed in a web page, etc - it's just meant for us to
// be able to paste screenshots and other generated images into Slack chats.

bool PNGWriter::Write( const uint8_t* pixels, uint16_t width, uint16_t height, uint8_t bytesPerPixel, const char* filename )
{
	if( ( pixels == NULL ) || ( filename == NULL ) )
	{
		return false;
	}

	FILE* file = fopen( filename, "wb" );
	if( file == NULL )
	{
		eeDebug( "fopen failed" );
		return false;
	}

	// The PNG datastream structure is documented at https://www.w3.org/TR/png-3/#5Introduction

	// All PNG datastreams start with these magic numbers. See https://www.w3.org/TR/PNG-Rationale.html
	// section 12.11 "PNG file signature" for why these numbers were chosen.
	// In ASCII: \211 P N G \r \n \032 \n
	uint8_t signature[8] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
	fwrite( signature, sizeof( signature ), 1, file );

	// The signature is followed by an IHDR chunk
	uint32_t ihdrSize = 13; // 13 bytes for an IHDR chunk
	fwrite( &ihdrSize, sizeof( ihdrSize ), 1, file );

	uint8_t ihdr[4] = { 'I', 'H', 'D', 'R' };
	fwrite( &ihdr, sizeof( ihdr ), 1, file );

	// Format of a IHDR block:
	// uint32_t width
	// uint32_t height
	// uint8_t bits per sample (1, 2, 4, 8, 16)
	// uint8_t color type:
	//   0 greyscale
	//   2 RGB color
	//   3 indexed color (ie colors from a palette)
	//   4 greyscale + alpha
	//   5 RGB color + alpha
	// uint8_t compression method (0 = deflate is the only valid value)
	// uint8_t filter method (0 = adaptive filtering is the only valid value)
	// uint8_t interlace method (0 = no interlacing 1 = Adam7 interlacing)
	typedef struct PNGHeader
	{
		uint32_t width;
		uint32_t height;
		uint8_t bitDepth;
		uint8_t colorType;
		uint8_t compressionMethod;
		uint8_t filterMethod;
		uint8_t interlaceMethod;
	} PNGHeader;

	struct PNGHeader header;

	header.width = width;
	header.height = height;
	header.bitDepth = 8; // we can assume RGB8 or RGBA8 format pixels
	// bytesPerPixel will be 3 (RGB8) or 4 (RGBA8)
	header.colorType = bytesPerPixel == 3 ? 2 : 6;
	header.compressionMethod = 0;
	header.filterMethod = 0;
	header.interlaceMethod = 0;

	fwrite( &header, 13, 1, file );

	uint32_t crc = ee::HashUtils::CalculateCRC32( reinterpret_cast< const uint8_t* >( &header ), 13 );
	fwrite( &crc, sizeof( crc ), 1, file );

	// The IHDR is followed by an IDAT block containing the pixel data
	uint32_t zlen = width * height * bytesPerPixel;

	fwrite( &zlen, sizeof( zlen ), 1, file ); // chunk size

	uint8_t idat[4] = { 'I', 'D', 'A', 'T' };
	fwrite( &idat, sizeof( idat ), 1, file );

	// build uncompressed deflate block here

	// Write the zlib header; the terminology used here comes from RFC 1950
	// (https://www.ietf.org/rfc/rfc1950.txt)

	uint8_t cmf = 0x78; // Compression method Deflate, 32K window size
	fwrite( &cmf, sizeof( cmf ), 1, file );

	uint8_t flg = 0x5e; // FLEVEL = 1
	fwrite( &flg, sizeof( flg ), 1, file );

	// Write the deflate blocks here, one per row of the image
	// (since the size limit of a deflate block is 64K bytes,
	//  we can't write the entire image to a single block unless
	//  it's a 128x128 image or less. We're just going to assume
	//  that our input image has a dimension > 128 pixels.)
	// The deflate block format is defined by RFC 1951 defined at
	// https://www.ietf.org/rfc/rfc1951.txt

	

	// write image data here

	// write image data CRC here

	// The final chunk in a PNG file is the IEND chunk
	uint32_t zero = 0;
	fwrite( &zero, sizeof( zero ), 1, file ); // chunk size

	uint8_t iend[4] = { 'I', 'E', 'N', 'D' };
	fwrite( &iend, sizeof( iend ), 1, file );

	fwrite( &zero, sizeof( zero ), 1, file ); // IEND CRC

	fclose( file );

	return true;
}
