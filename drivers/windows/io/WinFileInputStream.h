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

		virtual bool MarkSupported( void ) const override final { return true; }
		// Mark() stores the current location within the stream;
		// Reset() will seek to the marked location and clear the marker.
		virtual void Mark( void ) override final;
		virtual void Reset( void ) override final;

		virtual bool CanSeek( void ) const override final { return true; }

		virtual bool Seek( size_t offset, SeekOrigin origin = SeekOrigin::kFromCurrent ) override final;

		// Known as 'ftell' in the POSIX API
		virtual size_t GetCurrentOffset( void ) const override final;

		// Note: This is implemented using ReadFile() so bytesToRead can't
		// be larger than what a DWORD can hold (ie UINT32_MAX). An assert
		// will fire if bytesToRead is larger than what ReadFile() can support.
		virtual FileResult Read( void* buffer, size_t bytesToRead, size_t* bytesRead ) override final;

	private:
		static constexpr size_t kInvalidMarkIndex = ~0ull;

		File	mFile;
		HANDLE	mHandle = INVALID_HANDLE_VALUE;
		size_t	mMarkIndex = kInvalidMarkIndex;

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
