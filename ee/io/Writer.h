// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

#include <cstdarg>
#include <string>
#include <string_view>

namespace ee
{
	class Writer
	{
	public:
		virtual void Close( void ) = 0;
		virtual void Flush( void ) = 0;

		virtual void WriteBytes( const uint8_t* bytes, const size_t count ) = 0;

		inline void Write( const std::string_view& view );
		inline void Write( char value );
		inline void Write( bool value );
		inline void Write( float value );
		inline void Write( double value );
		inline void Write( int8_t value );
		inline void Write( int16_t value );
		inline void Write( int32_t value );
		inline void Write( int64_t value );
		inline void Write( uint8_t value );
		inline void Write( uint16_t value );
		inline void Write( uint32_t value );
		inline void Write( uint64_t value );

		//void WriteFormat( const char* format, ... );

		inline void WriteLn( const std::string_view& view );
		inline void WriteLn( char value );
		inline void WriteLn( bool value );
		inline void WriteLn( float value );
		inline void WriteLn( double value );
		inline void WriteLn( int8_t value );
		inline void WriteLn( int16_t value );
		inline void WriteLn( int32_t value );
		inline void WriteLn( int64_t value );
		inline void WriteLn( uint8_t value );
		inline void WriteLn( uint16_t value );
		inline void WriteLn( uint32_t value );
		inline void WriteLn( uint64_t value );

		//void WriteFormatLn( const char* format, ... );

		inline void EndLine( void );

	}; // class Writer

	inline void Writer::Write( const std::string_view& view )
	{
		WriteBytes( reinterpret_cast< const uint8_t* >( view.data() ), view.size() );
	}

	inline void Writer::Write( char value )
	{
		WriteBytes( reinterpret_cast< const uint8_t* >( &value ), 1 );
	}

	inline void Writer::Write( bool value )
	{
		Write( std::to_string( value ) );
	}

	inline void Writer::Write( double value )
	{
		Write( std::to_string( value ) );
	}

	inline void Writer::Write( int32_t value )
	{
		Write( std::to_string( value ) );
	}

	inline void Writer::Write( int64_t value )
	{
		Write( std::to_string( value ) );
		Write( "i64" );
	}

	inline void Writer::Write( uint32_t value )
	{
		Write( std::to_string( value ) );
	}

	inline void Writer::Write( uint64_t value )
	{
		Write( std::to_string( value ) );
		Write( "u64" );
	}

	inline void Writer::Write( uint8_t value )
	{
		Write( std::to_string( value ) );
	}

	inline void Writer::WriteLn( const std::string_view& view )
	{
		Write( view );
		EndLine();
	}

	inline void Writer::WriteLn( bool value )
	{
		Write( value );
		EndLine();
	}

	inline void Writer::WriteLn( double value )
	{
		Write( value );
		EndLine();
	}

	inline void Writer::WriteLn( int32_t value )
	{
		Write( value );
		EndLine();
	}

	inline void Writer::WriteLn( int64_t value )
	{
		Write( value );
		EndLine();
	}


	inline void Writer::WriteLn( uint32_t value )
	{
		Write( value );
		EndLine();
	}

	inline void Writer::WriteLn( uint64_t value )
	{
		Write( value );
		EndLine();
	}

	inline void Writer::EndLine( void )
	{
		Write( "\r\n" );
	}
}
