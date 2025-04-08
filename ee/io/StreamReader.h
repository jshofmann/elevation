// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

#include "Reader.h"

namespace ee
{
	class InputStream;

	class StreamReader : public Reader
	{
	public:
		StreamReader( InputStream& stream );

		// Reader interface implementation

		virtual void Close( void );
		virtual void Mark( void );
		virtual void Reset( void );
		virtual bool Ready( void );

		virtual size_t ReadChars( std::string::value_type* str, size_t length );

	private:
		InputStream* mInputStream = nullptr;

	}; // class StreamReader

} // namespace ee
