// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#include "pch.h"

#include "WinFileOutputStream.h"

using namespace ee;

void WinFileOutputStream::Release( void )
{
}

size_t WinFileOutputStream::GetAvailable( void )
{
	return 0;
}

bool WinFileOutputStream::Seek( size_t offset, SeekOrigin origin )
{
	return false;
}

size_t WinFileOutputStream::GetCurrentOffset( void )
{
	return 0;
}

FileResult WinFileOutputStream::Write( const void* buffer, size_t length )
{
	return FileResult::kOther;
}

void WinFileOutputStream::Flush( void )
{
}
