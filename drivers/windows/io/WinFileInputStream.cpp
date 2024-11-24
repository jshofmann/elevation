// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#include "pch.h"

#include <memory>

#include <ee/io/FileInputStream.h>

#include "WinFileInputStream.h"
#include "WinFileUtils.h"

using namespace ee;

namespace ee
{
	std::unique_ptr<FileInputStream> MakeFileInputStream( const char* filename )
	{
		return std::make_unique< WinFileInputStream >( filename );
	}

	std::unique_ptr<FileInputStream> MakeFileInputStream( const File& file )
	{
		return std::make_unique< WinFileInputStream >( file );
	}

} // namespace ee

WinFileInputStream::WinFileInputStream( const char* filename )
	: mFile( filename )
	, mHandle( INVALID_HANDLE_VALUE )
{
	// Note: If this fails, we're not going to abort construction here
	WinFileUtils::GetFileAttributes( mFile, mFile.GetFileStatus() );
}

WinFileInputStream::WinFileInputStream( const File& file )
	: mFile( file )
	, mHandle( INVALID_HANDLE_VALUE )
{
	// Note: If this fails, we're not going to abort construction here
	WinFileUtils::GetFileAttributes( mFile, mFile.GetFileStatus() );
}

WinFileInputStream::~WinFileInputStream()
{
	Close();
}

bool WinFileInputStream::Open( void )
{
	mHandle = CreateFile( mFile.GetFilename(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL );

	return ( mHandle != INVALID_HANDLE_VALUE );
}

void WinFileInputStream::Close( void )
{
}

bool WinFileInputStream::Available( void ) const
{
	return false;
}

size_t WinFileInputStream::GetSize( void )
{
	return 0;
}

bool WinFileInputStream::Seek( uint32_t offset, SeekOrigin origin )
{
	return false;
}

size_t WinFileInputStream::GetCurrentOffset( void )
{
	return 0;
}

FileResult WinFileInputStream::Read( void* buffer, uint32_t length )
{
	return FileResult::kOther;
}
