// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

#include <string_view>

#include <ee/io/Common.h>
#include <ee/io/File.h>
#include <ee/io/InputStream.h>

namespace ee
{
	class WinFileInputStream : public InputStream
	{
	public:
		WinFileInputStream() = delete;
		WinFileInputStream( const std::string_view& filename );
		WinFileInputStream( const File& file );

		// InputStream interface implementation

		virtual void Release( void ) override final;

		// Returns the size in bytes of the stream, if such a concept is well
		// defined. For files, this is the size of the file in bytes.
		virtual size_t GetAvailable( void ) override final;

		// Returns true if this is a stream that supports seeking.
		virtual bool CanSeek( void ) override final { return true; }

		virtual bool Seek( size_t offset, SeekOrigin origin = SeekOrigin::kFromCurrent ) override final;

		virtual size_t GetCurrentOffset( void ) override final;

		virtual FileResult Read( void* buffer, size_t length ) override final;

	}; // class FileInputStream

} // namespace ee
