// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

#include "Flags.h"

namespace ee
{
	class GlobalSettings
	{
	public:
		int			mDisplayMode;		// use Graphics::DisplayMode enum values

		// screenWidth/Height is the display's dimensions; sceneWidth/Height
		// are the dimensions the game is rendered to. These are frequently
		// identical unless the final render target is being upscaled.

		uint16_t	mScreenWidth;		// or window dimensions in windowed display
		uint16_t	mScreenHeight;

		uint16_t	mSceneWidth;			// dimensions of the render target
		uint16_t	mSceneHeight;		// == screen dimensions usually

		bool		mPaused;

		typedef enum
		{
			kDebuggerAttached,
			kDoLighting,
			kWireframe,
			kFlagCount
		}
		Flags;

		GlobalSettings();

		inline bool GetFlag( uint32_t flag ) const
		{
			return mFlags.GetFlag( 1ULL << flag );
		}

		inline void SetFlag( uint32_t flag, bool value )
		{
			return mFlags.SetFlag( 1ULL << flag, value );
		}

		inline void SetFlag( uint32_t flag )
		{
			SetFlag( flag, true );
		}

		inline void ClearFlag( uint32_t flag )
		{
			return mFlags.ClearFlag( 1ULL << flag );
		}

		inline void ToggleFlag( uint32_t flag )
		{
			return mFlags.ToggleFlag( 1ULL << flag );
		}

	private:
		Flags32 mFlags;

	}; // class GlobalSettings

} // namespace ee
