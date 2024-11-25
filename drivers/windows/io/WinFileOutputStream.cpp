// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#include "pch.h"

#include <memory>

#include <ee/io/FileOutputStream.h>

#include <drivers/windows/core/WinCheck.h>
#include <drivers/windows/core/WinUtil.h>

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

	if( mHandle == INVALID_HANDLE_VALUE )
	{
		DWORD error = GetLastError();
		eeUnusedVariable( error );

		eeDebug( "WinFileOutputStream::Open: CreateFile( %s ) returned error %d: %s\n", mFile.GetFilename(), error, WinUtil::GetErrorString( error ).c_str() );
	}

	return ( mHandle != INVALID_HANDLE_VALUE );
}

void WinFileOutputStream::Close( void )
{
	if( mHandle != INVALID_HANDLE_VALUE )
	{
		eeCheckBool( CloseHandle( mHandle ) );
		mHandle = INVALID_HANDLE_VALUE;
	}
}

bool WinFileOutputStream::Seek( uint32_t offset, SeekOrigin origin )
{
	if( !Available() )
		return false;

	DWORD result = SetFilePointer( mHandle, offset, NULL, WinFileUtils::GetMoveMethod( origin ) );

	return ( result != INVALID_SET_FILE_POINTER );
}

size_t WinFileOutputStream::GetCurrentOffset( void )
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

uint32_t WinFileOutputStream::Write( const void* buffer, uint32_t length )
{
	if( !Available() )
		return 0;

	DWORD bytesWritten;
	eeCheckBool( WriteFile( mHandle, buffer, length, &bytesWritten, NULL ) );

	return bytesWritten;
}

void WinFileOutputStream::Flush( void )
{
	if( Available() )
	{
		eeCheckBool( FlushFileBuffers( mHandle ) );
	}
}
