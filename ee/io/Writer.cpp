// Copyright (C) 2025 Azimuth Studios

#include "pch.h"

#include <stdarg.h>

#include "Writer.h"

using namespace ee;

static void WriteFormatV( Writer& writer, const char* format, va_list args )
{
	int length;
	{
		va_list argscopy;
		va_copy( argscopy, args );

#if defined( COMPILER_MSVC )
		length = ::_vscprintf( format, argscopy );
#else
		length = ::vsnprintf( nullptr, 0, format, argscopy );
#endif

		va_end( argscopy );
	}

	// if the string successfully formatted
	if( length > 0 )
	{
		va_list argscopy;
		va_copy( argscopy, args );

		std::string buffer;
		buffer.resize( length );

#if defined( COMPILER_MSVC )
		::vsnprintf_s( buffer.data(), length, length, format, argscopy );
#else
		::vsnprintf( buffer.data(), length, format, argscopy );
#endif

		va_end( argscopy );

		writer.WriteBytes( reinterpret_cast< const uint8_t* >( buffer.c_str() ), buffer.length() );
	}
}

void Writer::WriteFormat( const char* format, ... )
{
	va_list args;
	va_start( args, format );

	WriteFormatV( *this, format, args );

	va_end( args );
}

void Writer::WriteFormatLn( const char* format, ... )
{
	va_list args;
	va_start( args, format );

	WriteFormatV( *this, format, args );
	EndLine();

	va_end( args );
}

