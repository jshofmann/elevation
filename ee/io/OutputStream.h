// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

#include <ee/io/Common.h>

namespace ee
{
	// There are implementations of OutputStream based on what's being written to-
	// memory, a file, a socket, etc.
	class OutputStream
	{
	public:
		// Note: Do not attempt to use the stream after Close is called!
		// For files, sockets, etc this will close the stream destination.
		virtual void Close( void ) = 0;

		// Return true if the stream is usable - i.e for files the file exists
		// (or could be created) and can be read (or written) to, for memory
		// the memory has been assigned or allocated.
		virtual bool Available( void ) const = 0;

		// Returns true if this is a stream that supports seeking.
		virtual bool CanSeek( void ) = 0;

		// Note: we're using uint32_t, not size_t, for our offset and length
		// parameters because the Windows SDK uses DWORD (ie uint32_t) values
		// for those parameters in SetFilePointer() and WriteFile().
		virtual bool Seek( uint32_t offset, SeekOrigin origin = SeekOrigin::kFromCurrent ) = 0;

		// Known as 'ftell' in the POSIX API
		virtual size_t GetCurrentOffset( void ) = 0;

		// Returns the number of bytes written
		virtual uint32_t Write( const void* buffer, uint32_t length ) = 0;
		virtual void Flush( void ) {}

		// Implement operator << for each primitive type we support
		template< class T >
		OutputStream& operator << ( const T& data )
		{
			WriteOutputStream( *this, data );
			return *this;
		}

		// Or you can use a more old-school interface...
		inline void WriteBool( bool value );
		inline void WriteChar( char value );
		inline void WriteUInt8( uint8_t value );
		inline void WriteUInt16( uint16_t value );
		inline void WriteUInt32( uint32_t value );
		inline void WriteUInt64( uint64_t value );
		inline void WriteInt8( int8_t value );
		inline void WriteInt16( int16_t value );
		inline void WriteInt32( int32_t value );
		inline void WriteInt64( int64_t value );
		inline void WriteFloat( float value );
		inline void WriteDouble( double value );
		// !!! string support coming soon....

		inline void WriteBool( const bool* value, int count );
		inline void WriteChar( const char* value, int count );
		inline void WriteUInt8( const uint8_t* value, int count );
		inline void WriteUInt16( const uint16_t* value, int count );
		inline void WriteUInt32( const uint32_t* value, int count );
		inline void WriteUInt64( const uint64_t* value, int count );
		inline void WriteInt8( const int8_t* value, int count );
		inline void WriteInt16( const int16_t* value, int count );
		inline void WriteInt32( const int32_t* value, int count );
		inline void WriteInt64( const int64_t* value, int count );
		inline void WriteFloat( const float* value, int count );
		inline void WriteDouble( const double* value, int count );

	protected:
		// a virtual destructor is required in order to allow
		// derived implementations to override it
		virtual ~OutputStream() {}

	}; // class OutputStream

	// Helper for using OutputStream with unique_ptr
	class OutputStreamDeleter
	{
	public:
		void operator () ( OutputStream* stream ) const
		{
			if( stream != nullptr )
			{
				stream->Close();
			}
		}
	};

#define ImplementOutputStreamWrite(type)								\
	inline void WriteOutputStream( OutputStream& stream, const type& data ) \
	{																	\
		stream.Write( &data, sizeof( data ) );							\
	}

	ImplementOutputStreamWrite( bool );
	ImplementOutputStreamWrite( char );
	ImplementOutputStreamWrite( int8_t );
	ImplementOutputStreamWrite( int16_t );
	ImplementOutputStreamWrite( int32_t );
	ImplementOutputStreamWrite( int64_t );
	ImplementOutputStreamWrite( uint8_t );
	ImplementOutputStreamWrite( uint16_t );
	ImplementOutputStreamWrite( uint32_t );
	ImplementOutputStreamWrite( uint64_t );
	ImplementOutputStreamWrite( float );
	ImplementOutputStreamWrite( double );

#undef ImplementOutputStreamWrite

	inline void OutputStream::WriteBool( bool value )
	{
		WriteUInt8( value ? 1 : 0 );
	}

	inline void OutputStream::WriteChar( char value )
	{
		Write( &value, sizeof( char ) );
	}

	inline void OutputStream::WriteUInt8( uint8_t value )
	{
		Write( &value, sizeof( uint8_t ) );
	}

	inline void OutputStream::WriteUInt16( uint16_t value )
	{
		Write( &value, sizeof( uint16_t ) );
	}

	inline void OutputStream::WriteUInt32( uint32_t value )
	{
		Write( &value, sizeof( uint32_t ) );
	}

	inline void OutputStream::WriteUInt64( uint64_t value )
	{
		Write( &value, sizeof( uint64_t ) );
	}

	inline void OutputStream::WriteInt8( int8_t value )
	{
		Write( &value, sizeof( int8_t ) );
	}

	inline void OutputStream::WriteInt16( int16_t value )
	{
		Write( &value, sizeof( int16_t ) );
	}

	inline void OutputStream::WriteInt32( int32_t value )
	{
		Write( &value, sizeof( int32_t ) );
	}

	inline void OutputStream::WriteInt64( int64_t value )
	{
		Write( &value, sizeof( int64_t ) );
	}

	inline void OutputStream::WriteFloat( float value )
	{
		Write( &value, sizeof( float ) );
	}

	inline void OutputStream::WriteDouble( double value )
	{
		Write( &value, sizeof( double ) );
	}

	inline void OutputStream::WriteBool( const bool* value, int count )
	{
		Write( value, sizeof( bool ) * count );
	}

	inline void OutputStream::WriteChar( const char* value, int count )
	{
		Write( value, sizeof( char ) * count );
	}

	inline void OutputStream::WriteUInt8( const uint8_t* value, int count )
	{
		Write( value, sizeof( uint8_t ) * count );
	}

	inline void OutputStream::WriteUInt16( const uint16_t* value, int count )
	{
		Write( value, sizeof( uint16_t ) * count );
	}

	inline void OutputStream::WriteUInt32( const uint32_t* value, int count )
	{
		Write( value, sizeof( uint32_t ) * count );
	}

	inline void OutputStream::WriteUInt64( const uint64_t* value, int count )
	{
		Write( value, sizeof( uint64_t ) * count );
	}

	inline void OutputStream::WriteInt8( const int8_t* value, int count )
	{
		Write( value, sizeof( int8_t ) * count );
	}

	inline void OutputStream::WriteInt16( const int16_t* value, int count )
	{
		Write( value, sizeof( int16_t ) * count );
	}

	inline void OutputStream::WriteInt32( const int32_t* value, int count )
	{
		Write( value, sizeof( int32_t ) * count );
	}

	inline void OutputStream::WriteInt64( const int64_t* value, int count )
	{
		Write( value, sizeof( int64_t ) * count );
	}

	inline void OutputStream::WriteFloat( const float* value, int count )
	{
		Write( value, sizeof( float ) * count );
	}

	inline void OutputStream::WriteDouble( const double* value, int count )
	{
		Write( value, sizeof( double ) * count );
	}

} // namespace ee
