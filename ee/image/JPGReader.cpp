// Elevation Engine
//
// Copyright (c) 2026 Azimuth Studios

#include "pch.h"

#include <turbojpeg.h>

#include "image/JPGReader.h"

using namespace ee;

// Assumes that jpgFile contains the contents of a .jpg format file.
// This function will not copy the data in the span given to it.
bool JPGReader::DecompressImage( std::span< const std::byte > jpgFile, Image& image )
{
	tjhandle handle = tj3Init( TJINIT_DECOMPRESS );
	if( handle == nullptr )
	{
		eeDebug( "JPGReader::DecompressImage: tj3Init( TJINIT_DECOMPRESS ) failed with error %d: %s\n", tj3GetErrorCode( handle ), tj3GetErrorStr( handle ) );
		return false;
	}

	const unsigned char* jpgData = reinterpret_cast< const unsigned char* >( jpgFile.data() );

	if( tj3DecompressHeader( handle, jpgData, jpgFile.size() ) != 0 )
	{
		eeDebug( "JPGReader::DecompressImage: tjDecompressHeader() failed with error %d: %s\n", tj3GetErrorCode( handle ), tj3GetErrorStr( handle ) );
		return false;
	}

	int width = tj3Get( handle, TJPARAM_JPEGWIDTH );
	int height = tj3Get( handle, TJPARAM_JPEGHEIGHT );
	
	// Because we're decompressing to TJPF_RGBA below there's 4 bytes per pixel
	if( !image.Allocate( width, height, GpuFormat::kR8G8B8A8_UNORM ) )
	{
		eeDebug( "JPGReader::DecompressImage: Image::Allocate() failed\n" );
		return false;
	}

//	if( tjDecompress2( handle, jpgData, jpgFile.size(), image.GetPixels(), width, 0, height, TJPF_RGBA, TJFLAG_FASTDCT ) != 0 )
	if( tj3Decompress8( handle, jpgData, jpgFile.size(), image.GetPixels(), 0, TJPF_RGBA ) != 0 )
	{
		eeDebug( "JPGReader::DecompressImage: tjDecompress2() failed with error %d: %s\n", tj3GetErrorCode( handle ), tj3GetErrorStr( handle ) );
		return false;
	}

	tjDestroy( handle );

	return true;
}
