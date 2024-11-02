// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

#include <ee/io/FileStatus.h>
#include <ee/io/FileResult.h>

namespace ee
{
	namespace FileSystem
	{
		[[nodiscard]] bool Exists( FilePathView path, FileResult& error );

	} // namespace FileSystem

} // namespace ee
