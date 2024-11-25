// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#include "pch.h"

#include <memory>

#include <ee/io/FileInputStream.h>

#include <drivers/windows/core/WinCheck.h>
#include <drivers/windows/core/WinUtil.h>

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

	if( mHandle == INVALID_HANDLE_VALUE )
	{
		DWORD error = GetLastError();
		eeUnusedVariable( error );

		eeDebug( "WinFileInputStream::Open: CreateFile( %s ) returned error %d: %s\n", mFile.GetFilename(), error, WinUtil::GetErrorString( error ).c_str() );
	}

	return ( mHandle != INVALID_HANDLE_VALUE );
}

void WinFileInputStream::Close( void )
{
	if( mHandle != INVALID_HANDLE_VALUE )
	{
		eeCheckBool( CloseHandle( mHandle ) );
		mHandle = INVALID_HANDLE_VALUE;
	}
}

bool WinFileInputStream::Seek( uint32_t offset, SeekOrigin origin )
{
	if( !Available() )
		return false;

	DWORD result = SetFilePointer( mHandle, offset, NULL, WinFileUtils::GetMoveMethod( origin ) );

	return ( result != INVALID_SET_FILE_POINTER );
}

size_t WinFileInputStream::GetCurrentOffset( void )
{
	if( !Available() )
		return -1;

	// The Windows SDK equivalent of ftell() is SetFilePointer()'s result
	LONG offsetHigh;
	DWORD offset = SetFilePointer( mHandle, 0, &offsetHigh, FILE_CURRENT );

	if( offset == INVALID_SET_FILE_POINTER )
		return -1;

	return size_t( size_t( offsetHigh ) << 32 | offset );
}

FileResult WinFileInputStream::Read( void* buffer, uint32_t bytesToRead, uint32_t* bytesRead )
{
	if( !Available() )
		return FileResult::kNotFound;

	BOOL success = eeCheckBool( ReadFile( mHandle, buffer, bytesToRead, LPDWORD( bytesRead ), NULL ) );

	return success ? FileResult::kSuccess : FileResult::kOther;
}
