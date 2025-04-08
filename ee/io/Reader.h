// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

#include <string>

namespace ee
{
	class Reader
	{
	public:
		virtual void Close( void ) = 0;

		virtual void Mark( void ) {}
		virtual void Reset( void ) {}

		virtual bool Ready( void ) = 0;

		virtual	size_t ReadChars( std::string::value_type* str, size_t length ) = 0;

		virtual size_t Skip( size_t charCount );

		virtual std::string::value_type ReadChar( void );
		virtual std::string::value_type PeekChar( void );

		inline size_t Read( std::string::value_type* str, size_t length );
		inline size_t Read( std::string::value_type* str, size_t start, size_t length );

		size_t SkipWhitespace( void );

		std::string ReadLn( void );
		std::string ReadString( void );

		static std::string::value_type sErrorMarker;

	}; // class Reader

	/*
	Reader inline implementation
	*/
	size_t Reader::Read( std::string::value_type* str, size_t length )
	{
		return ReadChars( str, length );
	}

	size_t Reader::Read( std::string::value_type* str, size_t start, size_t length )
	{
		return ReadChars( str + start, length );
	}

} // namespace ee
