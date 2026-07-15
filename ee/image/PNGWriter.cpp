// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#include "pch.h"

#include <stdio.h>

#include "PNGWriter.h"

#include <ee/utility/HashUtils.h>

using namespace ee;

// Note: This is a very simple and limited PNG writer. It will encode a single
// uncompressed image into an uncompressed deflate block. Don't use it for
// something you intend to embed in a web page, etc - it's just meant for us to
// be able to paste screenshots and other generated images into Slack chats.
// Done because Legal won't let us use the STB library :/

// multibyte PNG fields are stored in network (ie big-endian) order
static constexpr uint32_t ToBigEndian( const uint32_t value )
{
	return ( value << 24 )
		| ( ( value << 8 ) & 0x00FF0000 )
		| ( ( value >> 8 ) & 0x0000FF00 )
		| ( value >> 24 );
}

// Incremental Adler-32 update as defined by RFC 1950
// (https://www.ietf.org/rfc/rfc1950.txt) sections 2.3 and 9.
// 's1'/'s2' should start at 1 and 0 respectively;
// the final checksum is ( s2 << 16 ) | s1.
static void UpdateAdler32( uint32_t& s1, uint32_t& s2, const uint8_t* data, size_t length )
{
	constexpr uint32_t kAdlerMod = 65521; // largest prime smaller than 65536

	while( length-- )
	{
		s1 = ( s1 + *data++ ) % kAdlerMod;
		s2 = ( s2 + s1 ) % kAdlerMod;
	}
}

bool PNGWriter::Write( const uint8_t* pixels,
					   uint16_t width,
					   uint16_t height,
					   uint8_t bytesPerPixel,
					   const char* filename,
					   ImageOrigin origin )
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

	// All PNG datastreams start with these magic numbers.
	// See https://www.w3.org/TR/PNG-Rationale.html section 12.11
	// "PNG file signature" for why these numbers were chosen.
	// In ASCII: \211 P N G \r \n \032 \n
	static constexpr uint8_t signature[ 8 ] = { 0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A };
	fwrite( signature, sizeof( signature ), 1, file );

	// The signature is followed by an IHDR chunk
	static constexpr uint32_t ihdrSize = ToBigEndian( 13 ); // 13 bytes for an IHDR chunk
	fwrite( &ihdrSize, sizeof( ihdrSize ), 1, file );

	static constexpr uint8_t ihdr[ 4 ] = { 'I', 'H', 'D', 'R' };
	fwrite( &ihdr, sizeof( ihdr ), 1, file );

	// Format of a IHDR chunk:
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

	// width/height are not uint8_t and have to be put into network byte order
	header.width = ToBigEndian( width );
	header.height = ToBigEndian( height );
	header.bitDepth = 8; // we can assume RGB8 or RGBA8 format pixels
	// bytesPerPixel will be 3 (RGB8) or 4 (RGBA8)
	header.colorType = bytesPerPixel == 3 ? 2 : 6;
	header.compressionMethod = 0;
	header.filterMethod = 0;
	header.interlaceMethod = 0;

	fwrite( &header, 13, 1, file );

	// The CRC should include the chunk type and chunk data but not the chunk size
	uint32_t hdrCRC = ee::HashUtils::BeginCRC32();
	hdrCRC = ee::HashUtils::UpdateCRC32( hdrCRC, ihdr, sizeof( ihdr ) );
	hdrCRC = ee::HashUtils::UpdateCRC32( hdrCRC, reinterpret_cast< const uint8_t* >( &header ), 13 );

	hdrCRC = ToBigEndian( ee::HashUtils::EndCRC32( hdrCRC ) );
	fwrite( &hdrCRC, sizeof( hdrCRC ), 1, file );

	// The IHDR is followed by an IDAT chunk containing the pixel data

	// We're using filter method 0 (no filtering). The IDAT block then requires
	// a filter-type byte prepended to each scan line, which is 0 (no filtering)
	size_t stride = (size_t) width * bytesPerPixel;
	size_t rawCount = ( stride + 1 ) * height; // includes the filter byte

	// Calculate the number of 64KiB blocks necessary to hold the image data
	size_t chunkCount = ( rawCount + 65534 ) / 65535;

	// There must be at least one IDAT chunk
	if( chunkCount == 0 )
		chunkCount = 1;

	// zlen is the size of the IDAT chunk's data field: the 2-byte zlib header,
	// one 5-byte stored-block header per deflate block, the raw scanline data
	// itself, and the trailing 4-byte Adler-32 checksum
	uint32_t zlen = ToBigEndian( uint32_t( 2 + chunkCount * 5 + rawCount + 4 ) );
	fwrite( &zlen, sizeof( zlen ), 1, file );

	static constexpr uint8_t idat[ 4 ] = { 'I', 'D', 'A', 'T' };
	fwrite( &idat, sizeof( idat ), 1, file );

	// The IDAT chunk's CRC covers the chunk type and every byte of chunk data
	// that follows (zlib header + deflate blocks + Adler-32) so it's
	// accumulated incrementally
	uint32_t idatCRC = ee::HashUtils::BeginCRC32();
	idatCRC = ee::HashUtils::UpdateCRC32( idatCRC, idat, sizeof( idat ) );

	// Write the zlib header; the terminology used here comes from RFC 1950
	// (https://www.ietf.org/rfc/rfc1950.txt) section 2.2.

	static constexpr uint8_t zlibHeader[ 2 ] = {
		0x78, // CMF: compression method Deflate, 32K window size
		0x5e, // FLG: FLEVEL = 1, no preset dictionary, FCHECK makes CMF/FLG a multiple of 31
	};

	fwrite( zlibHeader, sizeof( zlibHeader ), 1, file );

	idatCRC = ee::HashUtils::UpdateCRC32( idatCRC, zlibHeader, sizeof( zlibHeader ) );

	// Adler-32 checksum of the uncompressed data, computed incrementally
	// RFC 1950 defines these inital values for S1 and S2
	uint32_t adlerS1 = 1;
	uint32_t adlerS2 = 0;

	// Write the deflate blocks. Deflate's 'stored' (uncompressed) block type
	// is limited to 65535 bytes of data per block so a image larger than that
	// needs to be split across several blocks. A block boundary doesn't need
	// to line up with a scanline boundary: the inflated byte stream is just
	// one contiguous run of (filter-type byte + stride bytes) * height,
	// and PNG re-splits that back into scanlines - the block split below is
	// only about respecting the 65535-byte limit. The stored-block format is
	// defined by RFC 1951 (https://www.ietf.org/rfc/rfc1951.txt) section 3.2.4.
	size_t bytesLeft = rawCount;
	size_t pixelIndex = 0;   // next unwritten byte in 'pixels'
	size_t rowBytesLeft = 0; // pixel bytes left before the next filter-type byte is due
	size_t currentRow = 0;   // next scanline to be emitted, in PNG (top-down) order
	do
	{
		uint16_t blockSize = uint16_t( ( bytesLeft > 65535 ) ? 65535 : bytesLeft );

		// BFINAL/BTYPE byte: BFINAL (bit 0) is set on the last block,
		// BTYPE (bits 1-2) is always 0 (ie the data is uncompressed)
		uint8_t bfinal = ( bytesLeft == blockSize ) ? 1 : 0;
		fwrite( &bfinal, sizeof( bfinal ), 1, file );

		idatCRC = ee::HashUtils::UpdateCRC32( idatCRC, &bfinal, sizeof( bfinal ) );

		// LEN and its one's complement NLEN, both little-endian per RFC 1951.
		fwrite( &blockSize, sizeof( blockSize ), 1, file );

		idatCRC = ee::HashUtils::UpdateCRC32( idatCRC, reinterpret_cast< const uint8_t* >( &blockSize ), sizeof( blockSize ) );

		uint16_t nlen = (uint16_t) ~blockSize;
		fwrite( &nlen, sizeof( nlen ), 1, file );

		idatCRC = ee::HashUtils::UpdateCRC32( idatCRC, reinterpret_cast< const uint8_t* >( &nlen ), sizeof( nlen ) );

		uint16_t blockBytesLeft = blockSize;
		while( blockBytesLeft > 0 )
		{
			if( rowBytesLeft == 0 )
			{
				// Start of a new scanline: emit its filter-type byte
				const uint8_t filterType = 0;
				fwrite( &filterType, sizeof( filterType ), 1, file );

				// Update both hash values
				idatCRC = ee::HashUtils::UpdateCRC32( idatCRC, &filterType, sizeof( filterType ) );
				UpdateAdler32( adlerS1, adlerS2, &filterType, sizeof( filterType ) );

				rowBytesLeft = stride;
				--blockBytesLeft;
				--bytesLeft;

				// 'pixels' is always laid out row 0 first; for a
				// BottomLeft-origin source that's the last PNG scanline, so
				// walk the source rows in reverse as we emit them.
				size_t sourceRow = ( origin == ImageOrigin::kBottomLeft ) ? ( height - 1 - currentRow ) : currentRow;
				pixelIndex = sourceRow * stride;
				++currentRow;

				continue;

			} // if( rowBytesLeft == 0 )

			size_t bytesWritten = ( rowBytesLeft < blockBytesLeft ) ? rowBytesLeft : blockBytesLeft;
			fwrite( pixels + pixelIndex, bytesWritten, 1, file );

			idatCRC = ee::HashUtils::UpdateCRC32( idatCRC, pixels + pixelIndex, bytesWritten );

			UpdateAdler32( adlerS1, adlerS2, pixels + pixelIndex, bytesWritten );

			pixelIndex += bytesWritten;
			rowBytesLeft -= bytesWritten;
			blockBytesLeft -= uint16_t( bytesWritten );
			bytesLeft -= bytesWritten;

		} // while( blockBytesLeft > 0 )
	}
	while( bytesLeft > 0 );

	// The last four bytes of the IDAT block is its Adler-32 hash value
	uint32_t adlerCRC = ToBigEndian( ( adlerS2 << 16 ) | adlerS1 );
	fwrite( &adlerCRC, sizeof( adlerCRC ), 1, file );

	idatCRC = ee::HashUtils::UpdateCRC32( idatCRC, reinterpret_cast< const uint8_t* >( &adlerCRC ), sizeof( adlerCRC ) );

	// The IDAT block is complete, finalize calculating its CRC32 hash
	idatCRC = ToBigEndian( ee::HashUtils::EndCRC32( idatCRC ) );

	fwrite( &idatCRC, sizeof( idatCRC ), 1, file );

	// The final chunk in a PNG file is the IEND chunk which has no contents
	uint32_t iendSize = 0;
	fwrite( &iendSize, sizeof( iendSize ), 1, file );

	static constexpr uint8_t iend[ 4 ] = { 'I', 'E', 'N', 'D' };
	fwrite( &iend, sizeof( iend ), 1, file );

	// A precomputed CRC32 hash of the iend array
	static constexpr uint32_t iendCRC = ToBigEndian( 0xAE426082u );
	fwrite( &iendCRC, sizeof( iendCRC ), 1, file );

	fclose( file );

	return true;
}
