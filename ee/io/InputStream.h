// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

#include <ee/io/Common.h>

namespace ee
{
	// There are implementations of InputStream based on what's being read from-
	// memory, a file, a socket, etc.
	class InputStream
	{
	public:
		// Not all InputStreams have the concept of Opening the stream,
		// so implementation is optional
		virtual bool Open( void ) { return true; }

		// Note: Do not attempt to use the stream after Close is called!
		// For files, sockets, etc this will close the stream source.
		virtual void Close( void ) = 0;

		// Return true if the stream is usable - i.e for files the file exists
		// (or could be created) and can be read (or written) to, for memory
		// the memory has been assigned or allocated.
		virtual bool Valid( void ) const = 0;

		// Returns the number of bytes available to read. For files, this will
		// be size of the file minus the number of bytes already read
		virtual size_t Available( void ) const = 0;

		// Returns the size in bytes of the stream, if such a concept is well
		// defined. For files, this is the size of the file in bytes;
		// for memory, the number of bytes allocated or assigned.
		virtual size_t GetSize( void ) const = 0;

		// Return true if this is a stream that supports the concept of a marker
		// defining a location within the stream (e.g. a byte offset from the start)
		virtual bool MarkSupported( void ) const { return false; }
		// If marking is supported, Mark() stores the current location within the stream;
		// Reset() will seek to the marked location and clear the marker.
		virtual void Mark( void ) {}
		virtual void Reset( void ) {}

		// Returns true if this is a stream that supports seeking.
		virtual bool CanSeek( void ) const = 0;

		virtual bool Seek( size_t offset, SeekOrigin origin = SeekOrigin::kFromCurrent ) = 0;

		// Known as 'ftell' in the POSIX API
		virtual size_t GetCurrentOffset( void ) const = 0;

		virtual FileResult Read( void* buffer, size_t bytesToRead, size_t* bytesRead = nullptr ) = 0;

		// Implement operator >> each primitive type we support
		template< class T >
		InputStream& operator >> ( T& data )
		{
			InputStreamRead( *this, data );
			return *this;
		}

		// Or you can use a more old-school interface...
		inline uint8_t ReadByte( void );
		inline bool ReadBool( void );
		inline char ReadChar( void );
		inline uint8_t ReadUInt8( void );
		inline uint16_t ReadUInt16( void );
		inline uint32_t ReadUInt32( void );
		inline uint64_t ReadUInt64( void );
		inline int8_t ReadInt8( void );
		inline int16_t ReadInt16( void );
		inline int32_t ReadInt32( void );
		inline int64_t ReadInt64( void );
		inline float ReadFloat( void );
		inline double ReadDouble( void );
		// !!! string support coming soon....

		inline void ReadBool( bool* value, int count );
		inline void ReadChar( char* value, int count );
		inline void ReadUInt8( uint8_t* value, int count );
		inline void ReadUInt16( uint16_t* value, int count );
		inline void ReadUInt32( uint32_t* value, int count );
		inline void ReadUInt64( uint64_t* value, int count );
		inline void ReadInt8( int8_t* value, int count );
		inline void ReadInt16( int16_t* value, int count );
		inline void ReadInt32( int32_t* value, int count );
		inline void ReadInt64( int64_t* value, int count );
		inline void ReadFloat( float* value, int count );
		inline void ReadDouble( double* value, int count );

	protected:
		// a virtual destructor is required in order to allow
		// derived implementations to override it
		virtual ~InputStream() {}

	}; // class InputStream

	// Helper for using InputStream with unique_ptr
	class InputStreamDeleter
	{
	public:
		void operator () ( InputStream* stream ) const
		{
			if( stream != nullptr )
			{
				stream->Close();
			}
		}
	};

#define ImplementInputStreamRead(type) \
	inline void InputStreamRead( InputStream& stream, type& data ) \
	{ \
		stream.Read( &data, sizeof( data ) ); \
	}

	ImplementInputStreamRead( bool );
	ImplementInputStreamRead( char );
	ImplementInputStreamRead( int8_t );
	ImplementInputStreamRead( int16_t );
	ImplementInputStreamRead( int32_t );
	ImplementInputStreamRead( int64_t );
	ImplementInputStreamRead( uint8_t );
	ImplementInputStreamRead( uint16_t );
	ImplementInputStreamRead( uint32_t );
	ImplementInputStreamRead( uint64_t );
	ImplementInputStreamRead( float );
	ImplementInputStreamRead( double );

#undef ImplementInputStreamRead

	inline uint8_t InputStream::ReadByte( void )
	{
		return ReadUInt8();
	}

	inline bool InputStream::ReadBool( void )
	{
		return ( ReadUInt8() != 0 );
	}

	inline char InputStream::ReadChar( void )
	{
		char temp;
		Read( &temp, sizeof( char ) );
		return temp;
	}

	inline uint8_t InputStream::ReadUInt8( void )
	{
		uint8_t temp = 0;
		Read( &temp, sizeof( uint8_t ) );
		return temp;
	}

	inline uint16_t InputStream::ReadUInt16( void )
	{
		uint16_t temp = 0;
		Read( &temp, sizeof( uint16_t ) );
		return temp;
	}

	inline uint32_t InputStream::ReadUInt32( void )
	{
		uint32_t temp = 0;
		Read( &temp, sizeof( uint32_t ) );
		return temp;
	}

	inline uint64_t InputStream::ReadUInt64( void )
	{
		uint64_t temp = 0;
		Read( &temp, sizeof( uint64_t ) );
		return temp;
	}

	inline int8_t InputStream::ReadInt8( void )
	{
		int8_t temp = 0;
		Read( &temp, sizeof( int8_t ) );
		return temp;
	}

	inline int16_t InputStream::ReadInt16( void )
	{
		int16_t temp = 0;
		Read( &temp, sizeof( int16_t ) );
		return temp;
	}

	inline int64_t InputStream::ReadInt64( void )
	{
		int64_t temp = 0;
		Read( &temp, sizeof( int64_t ) );
		return temp;
	}

	inline float InputStream::ReadFloat( void )
	{
		float temp = 0.0f;
		Read( &temp, sizeof( float ) );
		return temp;
	}

	inline int InputStream::ReadInt32( void )
	{
		int temp = 0;
		Read( &temp, sizeof( int ) );
		return temp;
	}

	inline double InputStream::ReadDouble( void )
	{
		double temp = 0.0;
		Read( &temp, sizeof( double ) );
		return temp;
	}

	inline void InputStream::ReadBool( bool* val, int count )
	{
		Read( val, sizeof( bool ) * count );
	}

	inline void InputStream::ReadChar( char* val, int count )
	{
		Read( val, sizeof( char ) * count );
	}

	inline void InputStream::ReadUInt8( uint8_t* val, int count )
	{
		Read( val, sizeof( uint8_t ) * count );
	}

	inline void InputStream::ReadUInt16( uint16_t* val, int count )
	{
		Read( val, sizeof( uint16_t ) * count );
	}

	inline void InputStream::ReadUInt32( uint32_t* val, int count )
	{
		Read( val, sizeof( uint32_t ) * count );
	}

	inline void InputStream::ReadUInt64( uint64_t* val, int count )
	{
		Read( val, sizeof( uint64_t ) * count );
	}

	inline void InputStream::ReadInt8( int8_t* val, int count )
	{
		Read( val, sizeof( int8_t ) * count );
	}

	inline void InputStream::ReadInt16( int16_t* val, int count )
	{
		Read( val, sizeof( int16_t ) * count );
	}

	inline void InputStream::ReadInt32( int32_t* val, int count )
	{
		Read( val, sizeof( int32_t ) * count );
	}

	inline void InputStream::ReadInt64( int64_t* val, int count )
	{
		Read( val, sizeof( int64_t ) * count );
	}

	inline void InputStream::ReadFloat( float* val, int count )
	{
		Read( val, sizeof( float ) * count );
	}

	inline void InputStream::ReadDouble( double* val, int count )
	{
		Read( val, sizeof( double ) * count );
	}

} // namespace ee
