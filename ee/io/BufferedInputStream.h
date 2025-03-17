// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

#include <ee/io/Common.h>
#include <ee/io/InputStream.h>

namespace ee
{
	class BufferedInputStream : public InputStream
	{
	public:
		BufferedInputStream() = delete;
		BufferedInputStream( const BufferedInputStream& other ) = delete;
		BufferedInputStream( const BufferedInputStream&& other ) = delete;

		// Use this constructor if buffer has been allocated outside of
		// BufferedInputStream - stack-allocated memory, for example
		BufferedInputStream( InputStream* stream, const uint8_t* buffer, size_t bufferSizeInBytes );

		// This constructor will allocate the buffer from heap memory
		BufferedInputStream( InputStream* stream, size_t bufferSizeInBytes );
		~BufferedInputStream();

		// InputStream interface implementation

		virtual void Close( void ) override final;

		// Return true if the stream is usable - i.e for files the file exists
		// (or could be created) and can be read (or written) to, for memory
		// the memory has been assigned or allocated.
		virtual bool Available( void ) const override final;

		// Returns the size in bytes of the stream, if such a concept is well
		// defined. For buffers, this is the size of the buffer in bytes.
		virtual size_t GetSize( void ) const override final;

		// Returns true if this is a stream that supports seeking.
		virtual bool CanSeek( void ) override final { return true; }

		virtual bool Seek( size_t offset, SeekOrigin origin = SeekOrigin::kFromCurrent ) override final;

		// Known as 'ftell' in the POSIX API
		virtual size_t GetCurrentOffset( void ) const override final;

		virtual FileResult Read( void* buffer, uint32_t bytesToRead, uint32_t* bytesRead ) override final;

	private:
		InputStream* mSourceStream;
		size_t mSourceOffset = 0; // in bytes
		size_t mSourceLength = 0; // in bytes
		uint8_t* mBuffer = nullptr;
		size_t mBufferOffset = 0; // # of bytes used in mBuffer
		size_t mBufferSize = 0;
		bool mBufferOwned = false;

	}; // class BufferedInputStream

	inline bool BufferedInputStream::Available( void ) const
	{
		return ( mBuffer != nullptr );
	}

	inline size_t BufferedInputStream::GetSize( void ) const
	{
		return mBufferSize;
	}

	inline size_t BufferedInputStream::GetCurrentOffset( void ) const
	{
		return mSourceOffset;
	}

} // namespace ee
