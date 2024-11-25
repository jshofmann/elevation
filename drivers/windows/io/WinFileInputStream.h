// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

#include <ee/io/Common.h>
#include <ee/io/File.h>
#include <ee/io/InputStream.h>

namespace ee
{
	class WinFileInputStream : public InputStream
	{
	public:
		WinFileInputStream() = delete;
		WinFileInputStream( const WinFileInputStream& other ) = delete;
		WinFileInputStream( const WinFileInputStream&& other ) = delete;
		WinFileInputStream( const char* filename );
		WinFileInputStream( const File& file );
		~WinFileInputStream();

		bool Open( void );

		// InputStream interface implementation

		// Close() closes the file opened by Open().
		virtual void Close( void ) override final;

		// Return true if the stream is usable - i.e for files the file exists
		// (or could be created) and can be read (or written) to, for memory
		// the memory has been assigned or allocated.
		virtual bool Available( void ) const override final;

		// Returns the size in bytes of the stream, if such a concept is well
		// defined. For files, this is the size of the file in bytes.
		virtual size_t GetSize( void ) const override final;

		// Returns true if this is a stream that supports seeking.
		virtual bool CanSeek( void ) override final { return true; }

		// Note: we're using uint32_t, not size_t, for our offset and length
		// parameters because the Windows SDK uses DWORD (ie uint32_t) values
		// for those parameters in SetFilePointer() and ReadFile().
		virtual bool Seek( uint32_t offset, SeekOrigin origin = SeekOrigin::kFromCurrent ) override final;

		// Known as 'ftell' in the POSIX API
		virtual size_t GetCurrentOffset( void ) override final;

		virtual FileResult Read( void* buffer, uint32_t bytesToRead, uint32_t* bytesRead ) override final;

	private:
		File	mFile;
		HANDLE	mHandle;

	}; // class WinFileInputStream

	inline bool WinFileInputStream::Available( void ) const
	{
		return ( mHandle != INVALID_HANDLE_VALUE );
	}

	inline size_t WinFileInputStream::GetSize( void ) const
	{
		return mFile.GetFileStatus().GetSize();
	}

} // namespace ee
