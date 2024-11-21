// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

namespace ee
{
	class File;
	class FileStatus;

	namespace WinFileUtils
	{
		bool GetFileAttributes( const File& file, FileStatus& status );

	} // namespace WinFileUtils

} // namespace ee
