// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#include "pch.h"

#include <vector>
#include <cctype>

#include "Reader.h"

using namespace ee;

// Note: -1 is 0x7f, which is the DEL character in the ASCII character set;
// it's unlikely to be encountered in input data, and to change it risks
// introducing subtle regressions into the code base.
std::string::value_type Reader::sErrorMarker = -1;

//***********************************************************************
//***********************************************************************

std::string::value_type Reader::ReadChar( void )
{
	std::string::value_type c = 0;
	if( Read( &c, 1 ) == 0 )
		return Reader::sErrorMarker;
	else
		return c;
}

std::string::value_type Reader::PeekChar( void )
{
	Mark();
	std::string::value_type c = ReadChar();
	Reset();
	return c;
}

size_t Reader::Skip( size_t charCount )
{
	size_t skipCount = charCount;
	while( skipCount-- )
	{
		if( ReadChar() == Reader::sErrorMarker )
		{
			skipCount++;
			break;
		}
	}

	return charCount - skipCount;
}

size_t Reader::SkipWhitespace( void )
{
	Mark();

	size_t ws = 0;
	char c = ReadChar();
	while( std::isspace( c ) )
	{
		ws++;
		c = ReadChar();
	}

	Reset();
	Skip( ws );

	return ws;
}

std::string Reader::ReadLn( void )
{
	std::vector< std::string::value_type > chars;

	std::string::value_type c = ReadChar();
	while( ( c == '\r' ) && ( c != Reader::sErrorMarker ) )
	{
		c = ReadChar();
	}

	while( ( c != '\n' ) && ( c != Reader::sErrorMarker ) )
	{
		if( c != '\r' )
		{
			chars.push_back( c );
		}
		c = ReadChar();
	}

	if( c != Reader::sErrorMarker || !chars.empty() )
	{
		// Ensure that the string is 0-terminated and return it
		chars.push_back( 0 );
		return std::string( &chars[ 0 ] );
	}

	// else return an empty string as an error value
	return std::string( "" );
}

std::string Reader::ReadString( void )
{
	SkipWhitespace();

	std::vector< std::string::value_type > chars;

	std::string::value_type c = ReadChar();
	while( ( c == '\r' ) && ( c != Reader::sErrorMarker ) )
	{
		c = ReadChar();
	}

	while( !std::isspace( c ) && ( c != Reader::sErrorMarker ) )
	{
		chars.push_back( c );
		c = ReadChar();
	}

	if( c != Reader::sErrorMarker || !chars.empty() )

	{
		// Ensure that the string is 0-terminated and return it
		chars.push_back( 0 );
		return std::string( &chars[ 0 ] );
	}

	// else return an empty string as an error value
	return std::string( "" );
}
