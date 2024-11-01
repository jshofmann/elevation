// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#include "pch.h"

#include "GlobalSettings.h"

using namespace ee;

GlobalSettings::GlobalSettings()
	: mDisplayMode( 0 )			// Graphics::kDisplayFullscreen
	, mScreenWidth( 1920 )
	, mScreenHeight( 1080 )
	, mSceneWidth( 1920 )
	, mSsceneHeight( 1080 )
	, mPaused( false )
{
	setFlag( kDoLighting );
}
