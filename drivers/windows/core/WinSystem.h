// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

#include <ee/core/System.h>

namespace ee
{
	// Windows adds a few functions to the System namespace
	namespace System
	{
		// Note: The Driver should only be assigned once on application launch.
		// Note also that no validation is done to check if the assigned driver
		// is a valid driver for Platform::kWindows.
		void SetDriver( Driver driver );

		bool IsWin64( void );

		// This is roughly the same as "how much memory are we using"
		uint32_t GetWorkingSetSize( void );

	} // namespace System

} // namespace ee
