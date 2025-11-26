// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#include "pch.h"

#include <memory>

#include "TGAWriter.h"
#include "TGASupport.h"

#include <ee/core/Debug.h>
#include <ee/io/FileOutputStream.h>

using namespace ee;

bool TGAWriter::Write( const uint8_t* pixels, uint16_t width, uint16_t height, uint8_t bytesPerPixel, const char* filename )
{
	if( ( pixels == NULL ) || ( filename == NULL ) )
	{
		return false;
	}

	std::unique_ptr< FileOutputStream > stream = MakeFileOutputStream( filename );
	if( !stream->Open() )
	{
		eeDebug( "TGAWriter::write: Could not open '%s' for writing\n", filename );
		return false;
	}

	// ImageDescriptor: 0x20 defines a top-left origin,
	// 0x08 defines an 8-bit alpha channel
	uint8_t imageDescriptor = 0x20 | ( bytesPerPixel == 4 ? 0x8 : 0x0 );

	stream->WriteUInt8( 0 );					// TGAHeader::IDLength
	stream->WriteUInt8( 0 );					// TGAHeader::ColormapType
	stream->WriteUInt8( TGAHeader::IMAGE_TYPE_UNCOMPRESSED_RGB );	// TGAHeader::ImageType
	stream->WriteUInt16( 0 );					// TGAHeader::ColormapOrigin
	stream->WriteUInt16( 0 );					// TGAHeader::ColormapLength
	stream->WriteUInt8( 0 );					// TGAHeader::ColormapEntrySize
	stream->WriteUInt16( 0 );					// TGAHeader::XOrigin
	stream->WriteUInt16( 0 );					// TGAHeader::YOrigin
	stream->WriteUInt16( width );				// TGAHeader::ImageWidth
	stream->WriteUInt16( height );				// TGAHeader::ImageHeight
	stream->WriteUInt8( bytesPerPixel * 8 );	// TGAHeader::BitsPerPixel
	stream->WriteUInt8( imageDescriptor );		// TGAHeader::ImageDescriptor

	int size = width * height * bytesPerPixel;
	stream->Write( pixels, size );

	eeDebug( "TGAWriter::write: Wrote %s\n", filename );

	return true;
}
