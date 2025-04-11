// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

#include <ee/core/PlatformDetection.h>
#include <ee/io/File.h>

#if defined( BUILD_PC )
#include <drivers/windows/io/WinFileOutputStream.h>
#else // assume the POSIX interface is supported
#include <drivers/posix/io/PosixFileOutputStream.h>
#endif

namespace ee
{
#if defined( BUILD_PC )
	using FileOutputStream = WinFileOutputStream;
#else // assume the POSIX interface is supported
	using FileOutputStream = PosixFileOutputStream;
#endif

	// Use these functions to instantiate the appropriate OutputStream
	// for the System you're running on. They'll be implemented in a driver.
	std::unique_ptr< FileOutputStream > MakeFileOutputStream( const char* filename );

	std::unique_ptr< FileOutputStream > MakeFileOutputStream( std::shared_ptr< File > file );

} // namespace ee
