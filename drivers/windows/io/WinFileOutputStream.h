// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

#include <string_view>

#include <ee/io/Common.h>
#include <ee/io/File.h>
#include <ee/io/OutputStream.h>

namespace ee
{
	class WinFileOutputStream : public OutputStream
	{
	public:
		WinFileOutputStream() = delete;
		WinFileOutputStream( const std::string_view& filename );
		WinFileOutputStream( const File& file );

		// OutputStream interface implementation

		virtual void Release( void ) override final;

		// Returns the size in bytes of the stream, if such a concept is well
		// defined. For files, this is the size of the file in bytes.
		virtual size_t GetAvailable( void ) override final;

		// Returns true if this is a stream that supports seeking.
		virtual bool CanSeek( void ) override final { return true; }

		virtual bool Seek( size_t offset, SeekOrigin origin = SeekOrigin::kFromCurrent ) override final;

		virtual size_t GetCurrentOffset( void ) override final;

		virtual FileResult Write( const void* buffer, size_t length ) override final;
		virtual void Flush( void ) override final;

	}; // class FileOutputStream

} // namespace ee
