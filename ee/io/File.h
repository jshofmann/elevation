// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

#include <string>
#include <string_view>

#include <ee/io/Common.h>
#include <ee/io/FileStatus.h>

namespace ee
{
	class File
	{
	public:
		File() = delete;
		File( const std::string_view& filename );

		inline const char* GetFilename( void ) const noexcept;

		inline FileStatus& GetFileStatus( void ) noexcept;
		inline const FileStatus& GetFileStatus( void ) const noexcept;

	private:
		std::string	mFilename; // Filename as given during initialization
		FileStatus	mFileStatus;

	}; // class File

	inline const char* File::GetFilename( void ) const noexcept
	{
		return mFilename.c_str();
	}

	inline FileStatus& File::GetFileStatus( void ) noexcept
	{
		return mFileStatus;
	}

	inline const FileStatus& File::GetFileStatus( void ) const noexcept
	{
		return mFileStatus;
	}

} // namespace ee
