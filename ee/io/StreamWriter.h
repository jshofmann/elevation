// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

#include <ee/io/Writer.h>

namespace ee
{
	class OutputStream;

	class StreamWriter : public Writer
	{
	public:
		StreamWriter( OutputStream& stream );

		// Writer interface implementation

		virtual void Close( void ) override;
		virtual void Flush( void ) override;

		virtual void WriteBytes( const uint8_t* bytes, const size_t count ) override;

	private:
		OutputStream* mOutputStream = nullptr;

	}; // class StreamWriter

} // namespace ee
