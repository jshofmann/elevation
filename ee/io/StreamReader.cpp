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
		return mInputStream->Available();
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
		size_t start = mInputStream->GetCurrentOffset();

		FileResult result = mInputStream->Read( str, length );
		if( result != FileResult::kSuccess )
			return 0;

		// Return the number of bytes read
		return mInputStream->GetCurrentOffset() - start;
	}
	else
	{
		return 0;
	}
}
