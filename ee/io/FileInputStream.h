// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

#include <ee/core/PlatformDetection.h>
#include <ee/io/File.h>

#if defined( BUILD_PC )
#include <drivers/windows/io/WinFileInputStream.h>
#else // assume the POSIX interface is supported
#include <drivers/posix/io/PosixFileInputStream.h>
#endif

namespace ee
{
#if defined( BUILD_PC )
	using FileInputStream = WinFileInputStream;
#else // assume the POSIX interface is supported
	using FileInputStream = PosixFileInputStream;
#endif

	// Use these functions to instantiate the appropriate InputStream
	// for the System you're running on. They'll be implemented in a driver.
	std::unique_ptr< FileInputStream > MakeFileInputStream( const char* filename );

	std::unique_ptr< FileInputStream > MakeFileInputStream( std::shared_ptr< File > file );

} // namespace ee
