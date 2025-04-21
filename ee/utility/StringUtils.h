// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

#include <string>
#include <algorithm>
#include <charconv>

namespace ee
{
	// Wrap std::from_chars in a more strtoul-style interface,
	// returning 0 if the string_view is not convertable to uint32_t
	inline uint32_t ToUInt( const std::string_view& value )
	{
		unsigned int result;
		auto [ptr, ec] = std::from_chars( value.data(), value.data() + value.size(), result );
		return  ec == std::errc() ? result : 0;
	}

	// Wrap std::from_chars in a more strtof-style interface,
	// returning 0 if the string_view is not convertable to uint32_t
	inline float ToFloat( const std::string_view& value )
	{
		float result;
		auto [ptr, ec] = std::from_chars( value.data(), value.data() + value.size(), result );
		return  ec == std::errc() ? result : 0;
	}

	// Remove any whitespace from the start of a string
	inline void TrimLeadingWhitespace( std::string& str )
	{
		str.erase( str.begin(), std::find_if(
					   str.begin(), str.end(), []( unsigned char c )
					   {
						   return !std::isspace( c );
					   }
				  ) );
	}

	// Remove any whitespace from the end of a string
	inline void TrimTrailingWhitespace( std::string& str )
	{
		str.erase( std::find_if( str.rbegin(), str.rend(), []( unsigned char c )
					{
						return !std::isspace( c );
					}
				 ).base(), str.end() );
	}

	// Trim any whitespace from both ends of the string
	inline void TrimWhitespace( std::string& str )
	{
		TrimLeadingWhitespace( str );
		TrimTrailingWhitespace( str );
	}

} // namespace ee
