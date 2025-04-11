// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#include "pch.h"

#include <ee/io/File.h>
#include <ee/io/FileStatus.h>

#include "WinFileUtils.h"

using namespace ee;

namespace ee
{
	namespace
	{
		time_t FileTimeToUnixTime( LPFILETIME fileTime )
		{
			LONGLONG tmp = LONGLONG( fileTime->dwHighDateTime ) << 32;
			tmp = tmp + fileTime->dwLowDateTime - 116444736000000000;
			return time_t( tmp / 10000000L );
		}
	}

	namespace WinFileUtils
	{
		bool BuildFileStatus( const File& file, FileStatus& status )
		{
			char* filepart;
			char buffer[ 1024 ];

			// filepart will point to the filename component in buffer
			DWORD size = GetFullPathName( file.GetFilename(),
										  sizeof( buffer ),
										  buffer,
										  &filepart );
			if( size == 0 )
			{
				eeDebug( "WinFileUtils::BuildFileStatus: GetFullPathName( %s ) failed with error 0x08x\n", file.GetFilename(), GetLastError() );
				return false;
			}

			status.SetAbsolutePath( buffer );

			WIN32_FILE_ATTRIBUTE_DATA attributes;
			if( GetFileAttributesEx( status.GetAbsolutePath().c_str(), GetFileExInfoStandard, &attributes ) == FALSE )
			{
				// Report anything that's not a simple 'file not found' error
				DWORD error = GetLastError();
				if( ( error != ERROR_FILE_NOT_FOUND ) && ( error != ERROR_PATH_NOT_FOUND ) )
				{
					eeDebug( "WinFileUtils::BuildFileStatus: GetFileAttributesEx( %s ) returned error 0x%08x", status.GetAbsolutePath().c_str(), GetLastError() );
				}

				status.SetType( FileStatus::Type::kNone );
				status.SetFlags( 0 );
				status.SetLastModified( 0 );
				status.SetSize( 0 );
			}
			else
			{
				if( ( attributes.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) != 0 )
				{
					status.SetType( FileStatus::Type::kDirectory );
				}
				else
				{
					status.SetType( FileStatus::Type::kFile );
				}

				uint8_t flags = FileStatus::Flags::kExists;

				if( ( attributes.dwFileAttributes & FILE_ATTRIBUTE_READONLY ) != 0 )
				{
					flags |= FileStatus::Flags::kReadOnly;
				}

				if( ( attributes.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN ) != 0 )
				{
					flags |= FileStatus::Flags::kHidden;
				}

				FILETIME localWrite;
				if( FileTimeToLocalFileTime( &attributes.ftLastWriteTime, &localWrite ) )
				{
					status.SetLastModified( FileTimeToUnixTime( &localWrite ) );
				}
				else
				{
					eeDebug( "WinFileUtils::BuildFileStatus: FileTimeToLocalFileTime( %s ) returned error 0x%08x", status.GetAbsolutePath().c_str(), GetLastError() );
					status.SetLastModified( FileTimeToUnixTime( &attributes.ftLastWriteTime ) );
				}

				status.SetSize( ( size_t( attributes.nFileSizeHigh ) << 32 ) | attributes.nFileSizeLow );
			}

			return true;
		}

		DWORD GetMoveMethod( SeekOrigin origin )
		{
			switch( origin )
			{
			case SeekOrigin::kFromStart:	return FILE_BEGIN;
			case SeekOrigin::kFromCurrent:	return FILE_CURRENT;
			case SeekOrigin::kFromEnd:		return FILE_END;
			}

			return FILE_CURRENT;
		}

	} // namespace WinFileUtils

} // namespace ee
