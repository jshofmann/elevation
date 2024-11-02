// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

#include <ee/core/PlatformDetection.h>

#if defined( PLATFORM_WINDOWS )

using FileInputStream = WinFileInputStream;

#else // assume the POSIX interface is supported

using FileInputStream = PosixFileInputStream;

#endif
