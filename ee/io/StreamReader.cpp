// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#include "pch.h"

#include "StreamReader.h"

#include <ee/io/InputStream.h>

using namespace ee;

StreamReader::StreamReader( InputStream& stream )
{
	mInputStream = &stream;
}

void StreamReader::Close( void )
{
	if( mInputStream != nullptr )
	{
		mInputStream->Close();
	}
}

void StreamReader::Mark( void )
{
	if( mInputStream != nullptr )
	{
		mInputStream->Mark();
	}
}

void StreamReader::Reset( void )
{
	if( mInputStream != nullptr )
	{
		mInputStream->Reset();
	}
}

bool StreamReader::Ready( void )
{
	if( mInputStream != nullptr )
	{
		return mInputStream->Available() != 0;
	}
	else
	{
		return false;
	}
}

size_t StreamReader::ReadChars( std::string::value_type* str, size_t length )
{
	if( mInputStream != nullptr )
	{
		size_t bytesRead;
		FileResult result = mInputStream->Read( str, length, &bytesRead );
		if( result != FileResult::kSuccess )
			return 0;

		return bytesRead;
	}
	else
	{
		return 0;
	}
}
