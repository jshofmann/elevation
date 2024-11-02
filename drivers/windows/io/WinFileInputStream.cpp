// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#include "pch.h"

#include "WinFileInputStream.h"

using namespace ee;

void WinFileInputStream::Release( void )
{
}

size_t WinFileInputStream::GetAvailable( void )
{
	return 0;
}

bool WinFileInputStream::Seek( size_t offset, SeekOrigin origin )
{
	return false;
}

size_t WinFileInputStream::GetCurrentOffset( void )
{
	return 0;
}

FileResult WinFileInputStream::Read( void* buffer, size_t length )
{
	return FileResult::kOther;
}
