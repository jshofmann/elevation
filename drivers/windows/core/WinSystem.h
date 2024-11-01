// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

#include <ee/core/System.h>

namespace ee
{
    class WinSystem : public System
	{
	public:
		WinSystem();

		// Note: The Driver should only be assigned once on application launch
		static void SetDriver( Driver driver );

		// WinSystem member functions

		static bool IsWin64( void );

		// This is roughly the same as "how much memory are we using"
		static uint32_t GetWorkingSetSize( void );

	}; // class WinSystem

} // namespace ee
