// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

namespace ee
{
	namespace WinDebug
	{
		// Return a string representation of the given Windows message token
		// of the variety passed as the uMsg parameter of a Window Procedure.
		// Will return the token name for the commonly-seen WM_ tokens and a
		// hex-formatted representation of the value of all other tokens.
		const char* GetMessageString( UINT message );

	} // namespace WinDebug

} // namespace ee
