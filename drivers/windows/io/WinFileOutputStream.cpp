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

	std::unique_ptr< FileOutputStream > MakeFileOutputStream( std::shared_ptr< File > file )
	{
		return std::make_unique< WinFileOutputStream >( file );
	}

} // namespace ee

WinFileOutputStream::WinFileOutputStream( const char* filename )
{
	mFile = std::make_shared< File >( filename );
	// Note: If this fails, we're not going to abort construction here
	WinFileUtils::BuildFileStatus( *mFile, mFile->GetStatus() );
}

WinFileOutputStream::WinFileOutputStream( std::shared_ptr< File > file )
	: mFile( file )
{
	// Note: If this fails, we're not going to abort construction here
	WinFileUtils::BuildFileStatus( *mFile, mFile->GetStatus() );
}

WinFileOutputStream::~WinFileOutputStream()
{
	Close();
}

bool WinFileOutputStream::Open( void )
{
	mHandle = CreateFile( mFile->GetFilename(), GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

	if( mHandle == INVALID_HANDLE_VALUE )
	{
		DWORD error = GetLastError();
		eeUnusedVariable( error );

		eeDebug( "WinFileOutputStream::Open: CreateFile( %s ) returned error %d: %s\n", mFile->GetFilename(), error, WinUtil::GetErrorString( error ).c_str() );
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

bool WinFileOutputStream::Seek( size_t offset, SeekOrigin origin )
{
	if( !Valid() )
		return false;

	return eeCheckBool( SetFilePointerEx( mHandle, WinUtil::ToLARGE_INTEGER( offset ), NULL, WinFileUtils::GetMoveMethod( origin ) ) );
}

size_t WinFileOutputStream::GetCurrentOffset( void )
{
	if( !Valid() )
		return -1;

	// The Windows SDK equivalent of ftell() is SetFilePointerEx()'s lpNewFilePointer out parameter
	LARGE_INTEGER pointer;
	if( !eeCheckBool( SetFilePointerEx( mHandle, { 0, 0 }, &pointer, FILE_CURRENT ) ) )
	{
		return -1;
	}

	return WinUtil::ToSize( pointer );
}

uint32_t WinFileOutputStream::Write( const void* buffer, size_t length )
{
	if( !Valid() )
		return 0;

	eeAssert( length < 0xffffffff, "WriteFile can only write up to UINT32_MAX bytes; %lld bytes were requested", length );
	if( length > 0xffffffff )
		return 0;

	DWORD bytesWritten;
	eeCheckBool( WriteFile( mHandle, buffer, DWORD( length ), &bytesWritten, NULL ) );

	return bytesWritten;
}

void WinFileOutputStream::Flush( void )
{
	if( Valid() )
	{
		eeCheckBool( FlushFileBuffers( mHandle ) );
	}
}
