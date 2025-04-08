// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

#include <ee/io/Common.h>
#include <ee/io/File.h>
#include <ee/io/OutputStream.h>

namespace ee
{
	class WinFileOutputStream : public OutputStream
	{
	public:
		WinFileOutputStream() = delete;
		WinFileOutputStream( const WinFileOutputStream& other ) = delete;
		WinFileOutputStream( const WinFileOutputStream&& other ) = delete;
		WinFileOutputStream( const char* filename );
		WinFileOutputStream( const File& file );
		~WinFileOutputStream();

		bool Open( void );

		// OutputStream interface implementation

		// Close() closes the file opened by Open().
		virtual void Close( void ) override final;

		// Return true if the stream is usable - i.e for files the file exists
		// (or could be created) and can be read (or written) to, for memory
		// the memory has been assigned or allocated.
		virtual bool Available( void ) const override final;

		// Returns true if this is a stream that supports seeking.
		virtual bool CanSeek( void ) override final { return true; }

		virtual bool Seek( size_t offset, SeekOrigin origin = SeekOrigin::kFromCurrent ) override final;

		// Known as 'ftell' in the POSIX API
		virtual size_t GetCurrentOffset( void ) override final;

		// Returns the number of bytes written
		virtual uint32_t Write( const void* buffer, size_t length ) override final;
		virtual void Flush( void ) override final;

	private:
		File	mFile;
		HANDLE	mHandle;

	}; // class WinFileOutputStream

	inline bool WinFileOutputStream::Available( void ) const
	{
		return ( mHandle != INVALID_HANDLE_VALUE );
	}

} // namespace ee
