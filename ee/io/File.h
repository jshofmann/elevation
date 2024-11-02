// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

#include <string_view>

#include <ee/io/Common.h>
#include <ee/io/FileStatus.h>

namespace ee
{
	class File
	{
	public:
		File() = delete;
		File( const File& file ) = delete;
		File( const File&& file ) = delete;
		File( const std::string_view& filename );

		inline const FileStatus& GetFileStatus( void ) const;

	private:
		FileStatus mFileStatus;

	}; // class File

	inline const FileStatus& File::GetFileStatus( void ) const
	{
		return mFileStatus;
	}

} // namespace ee
