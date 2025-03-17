// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#include "pch.h"

#include <memory>

#include <ee/io/FileInputStream.h>

#include <drivers/windows/core/WinCheck.h>
#include <drivers/windows/core/WinUtil.h>

#include "BufferedInputStream.h"

using namespace ee;

BufferedInputStream::~BufferedInputStream()
{
	Close();

	if( mBufferOwned )
	{
		delete[] mBuffer;
		mBuffer = nullptr;
	}
}

void BufferedInputStream::Close( void )
{
	mSourceStream->Close();
	mSourceOffset = 0;
	mSourceLength = 0;
	mBufferOffset = 0;
}

bool BufferedInputStream::Seek( size_t offset, SeekOrigin origin )
{
	if( !Available() )
		return false;

	size_t requestedPosition;

	if( origin == SeekOrigin::kFromStart )
	{
		requestedPosition = offset;
	}
	else if( origin == SeekOrigin::kFromCurrent )
	{
		requestedPosition = GetCurrentOffset() + offset;
	}
	else // origin == SeekOrigin::kFromEnd
	{
		requestedPosition = mSourceLength - offset;
	}

	if( mBufferOffset < mBufferSize )
	{
		
	}

	return true;
}

FileResult BufferedInputStream::Read( void* buffer, uint32_t bytesToRead, uint32_t* bytesRead )
{
	return FileResult::kOther;
}
