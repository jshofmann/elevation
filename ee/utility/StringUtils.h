// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

#include <string>
#include <algorithm>

namespace ee
{
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
