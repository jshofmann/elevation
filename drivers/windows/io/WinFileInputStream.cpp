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
{
	// Note: If this fails, we're not going to abort construction here
	WinFileUtils::GetFileAttributes( mFile, mFile.GetFileStatus() );
}

WinFileInputStream::WinFileInputStream( const File& file )
	: mFile( file )
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

void WinFileInputStream::Mark( void )
{
	if( mHandle != INVALID_HANDLE_VALUE )
	{
		mMarkIndex = GetCurrentOffset();
	}
}

void WinFileInputStream::Reset( void )
{
	if( mMarkIndex != kInvalidMarkIndex )
	{
		Seek( mMarkIndex, SeekOrigin::kFromStart );
		mMarkIndex = kInvalidMarkIndex;
	}
}

bool WinFileInputStream::Seek( size_t offset, SeekOrigin origin )
{
	if( !Available() )
		return false;

	return eeCheckBool( SetFilePointerEx( mHandle, WinUtil::ToLARGE_INTEGER( offset ), NULL, WinFileUtils::GetMoveMethod( origin ) ) );
}

size_t WinFileInputStream::GetCurrentOffset( void ) const
{
	if( !Available() )
		return -1;

	// The Windows SDK equivalent of ftell() is SetFilePointerEx()'s lpNewFilePointer out parameter
	LARGE_INTEGER pointer;
	if( !eeCheckBool( SetFilePointerEx( mHandle, { 0, 0 }, &pointer, FILE_CURRENT ) ) )
	{
		return -1;
	}

	return WinUtil::ToSize( pointer );
}

FileResult WinFileInputStream::Read( void* buffer, size_t bytesToRead, size_t* bytesRead )
{
	if( !Available() )
		return FileResult::kNotFound;

	eeAssert( bytesToRead < 0xffffffff, "ReadFile can read only up to UINT32_MAX bytes; %lld bytes were requested", bytesToRead );
	if( bytesToRead > 0xffffffff )
		return FileResult::kInvalidArgument;

	BOOL success = eeCheckBool( ReadFile( mHandle, buffer, static_cast< DWORD >( bytesToRead ), LPDWORD( bytesRead ), NULL ) );

	return success ? FileResult::kSuccess : FileResult::kOther;
}
