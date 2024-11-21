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
	namespace WinFileUtils
	{
		bool GetFileAttributes( const File& file, FileStatus& status )
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
				eeDebug( "WinFileUtils::GetFileAttributes: GetFullPathName( %s ) failed with error 0x08x\n", file.GetFilename(), GetLastError() );
				return false;
			}

			status.SetAbsolutePath( buffer );

			WIN32_FILE_ATTRIBUTE_DATA attributes;
			if( GetFileAttributesEx( status.GetAbsolutePath().c_str(), GetFileExInfoStandard, &attributes ) == FALSE )
			{
#if 0
				// Report anything that's not a simple 'file not found' error
				DWORD error = GetLastError();
				if( ( error != ERROR_FILE_NOT_FOUND ) && ( error != ERROR_PATH_NOT_FOUND ) )
				{
					eeDebug( "WinFileUtils::GetFileAttributes: GetFileAttributesEx( %s ) returned error 0x%08x", fa.AbsolutePath.c_str(), GetLastError() );
				}

				fa.Flags.mFlags = 0;
				fa.Modified = 0;
				fa.Size = 0;
			}
			else
			{
				fa.Flags.setFlag( FileAttributes::Flags::EXISTS, true );
				fa.Flags.setFlag( FileAttributes::Flags::DIRECTORY, ( attributes.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) != 0 );
				fa.Flags.setFlag( FileAttributes::Flags::READONLY, ( attributes.dwFileAttributes & FILE_ATTRIBUTE_READONLY ) != 0 );
				fa.Flags.setFlag( FileAttributes::Flags::HIDDEN, ( attributes.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN ) != 0 );

				FILETIME localWrite;
				if( FileTimeToLocalFileTime( &attributes.ftLastWriteTime, &localWrite ) )
				{
					fa.Modified = FileTimeToUnixTime( &localWrite );
				}
				else
				{
					eeDebug( "WinFileUtils::GetFileAttributes: FileTimeToLocalFileTime( %s ) returned error 0x%08x", fa.AbsolutePath.c_str(), GetLastError() );
					fa.Modified = FileTimeToUnixTime( &attributes.ftLastWriteTime );
				}

				fa.Size = ( UInt64( attributes.nFileSizeHigh ) << 32 ) | attributes.nFileSizeLow;
#endif
			}

			return true;
		}

	} // namespace WinFileUtils


} // namespace ee
