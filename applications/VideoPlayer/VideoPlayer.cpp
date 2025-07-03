// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#include "pch.h"

#include "VideoPlayer.h"

using namespace ee;

bool VideoPlayer::Initialize( std::unique_ptr< ee::Device > device, std::unique_ptr< ee::Display > display )
{
	mDevice = std::move( device );
	mDisplay = std::move( display );

	return true;
}

void VideoPlayer::Shutdown( void )
{
	if( mDisplay )
	{
		mDisplay->Release();
		mDisplay.reset();
	}

	if( mDevice )
	{
		mDevice->Release();
		mDevice.reset();
	}
}
