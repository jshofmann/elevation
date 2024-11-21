// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#include "pch.h"

#include <memory>

#include <ee/io/FileOutputStream.h>

#include "WinFileOutputStream.h"
#include "WinFileUtils.h"

using namespace ee;

namespace ee
{
	std::unique_ptr<FileOutputStream> MakeFileOutputStream( const char* filename )
	{
		return std::make_unique< WinFileOutputStream >( filename );
	}

	std::unique_ptr<FileOutputStream> MakeFileOutputStream( const File& file )
	{
		return std::make_unique< WinFileOutputStream >( file );
	}

} // namespace ee

WinFileOutputStream::WinFileOutputStream( const char* filename )
	: mFile( filename )
	, mHandle( INVALID_HANDLE_VALUE )
{
	// Note: If this fails, we're not going to abort construction here
	WinFileUtils::GetFileAttributes( mFile, mFile.GetFileStatus() );
}

WinFileOutputStream::WinFileOutputStream( const File& file )
	: mFile( file )
	, mHandle( INVALID_HANDLE_VALUE )
{
	// Note: If this fails, we're not going to abort construction here
	WinFileUtils::GetFileAttributes( mFile, mFile.GetFileStatus() );
}

WinFileOutputStream::~WinFileOutputStream()
{
	Close();
}

bool WinFileOutputStream::Open( void )
{
	mHandle = CreateFile( mFile.GetFilename(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );
#if 0
	if( mHandle != INVALID_HANDLE_VALUE )
	{
		LARGE_INTEGER size;
		GetFileSizeEx( mHandle, &size );
		mLength = size.QuadPart;
	}
	else
	{
		mLength = 0;
	}
#endif

	return ( mHandle != INVALID_HANDLE_VALUE );
}

void WinFileOutputStream::Close( void )
{
	if( mHandle != INVALID_HANDLE_VALUE )
	{
		CloseHandle( mHandle );
		mHandle = INVALID_HANDLE_VALUE;
	}
}

bool WinFileOutputStream::Available( void ) const
{
	return false;
}

bool WinFileOutputStream::Seek( size_t offset, SeekOrigin origin )
{
	return false;
}

size_t WinFileOutputStream::GetCurrentOffset( void )
{
	return 0;
}

FileResult WinFileOutputStream::Write( const void* buffer, size_t length )
{
	return FileResult::kOther;
}

void WinFileOutputStream::Flush( void )
{
}
