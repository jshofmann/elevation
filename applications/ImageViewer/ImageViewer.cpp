// Elevation Engine
//
// Copyright (c) 2026 Azimuth Studios

#include "pch.h"

#include <span>

#include "ImageViewer.h"

#include <io/File.h>
#include <io/FileInputStream.h>
#include <utility/Defer.h>
#include <image/JPGReader.h>

using namespace ee;

bool ImageViewer::LoadImage( const char* filename )
{
	std::shared_ptr< File > jpgFile = std::make_shared< File >( filename );

	std::unique_ptr<FileInputStream> stream = MakeFileInputStream( jpgFile );
	if( !stream->Open() )
		return false;

	size_t size = stream->GetSize();

	std::byte* contents = new std::byte[ size ];
	auto cleanup = Defer( [ contents ]() { delete[] contents; } );

	if( stream->Read( contents, size ) != FileResult::kSuccess )
		return false;

	if( !JPGReader::DecompressImage( { contents, size }, mImage ) )
		return false;

	return true;
}
