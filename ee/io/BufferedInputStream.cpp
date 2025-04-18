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

bool BufferedInputStream::Open( void )
{
	mSourceOffset = 0;
	mSourceLength = 0;
	mBufferOffset = 0;
	return mSourceStream->Open();
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
	if( !Valid() )
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

FileResult BufferedInputStream::Read( void* buffer, size_t bytesToRead, size_t* bytesRead )
{
	return FileResult::kOther;
}
