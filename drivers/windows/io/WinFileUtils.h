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
		bool BuildFileStatus( const File& file, FileStatus& status );

		DWORD GetMoveMethod( SeekOrigin origin );

	} // namespace WinFileUtils

} // namespace ee
