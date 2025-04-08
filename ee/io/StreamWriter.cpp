// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#include "pch.h"

#include <ee/io/OutputStream.h>

#include "StreamWriter.h"

using namespace ee;

StreamWriter::StreamWriter( OutputStream& stream )
{
	mOutputStream = &stream;
}

void StreamWriter::Close( void )
{
	if( mOutputStream != nullptr )
	{
		mOutputStream->Flush();
		mOutputStream->Close();
	}
}

void StreamWriter::Flush( void )
{
	if( mOutputStream != nullptr )
	{
		mOutputStream->Flush();
	}
}

void StreamWriter::WriteBytes( const uint8_t* bytes, const size_t count )
{
	if( mOutputStream != nullptr )
	{
		mOutputStream->Write( bytes, count );
	}
}
