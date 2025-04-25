// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#include "pch.h"

#include "VideoPlayer.h"

using namespace ee;

VideoPlayer::VideoPlayer()
	: mWidth( 0 )
	, mHeight( 0 )
{
}

VideoPlayer::~VideoPlayer()
{
}

bool VideoPlayer::Initialize( std::unique_ptr< ee::Device > device, std::unique_ptr< ee::Display > display )
{
	mDevice = std::move( device );
	mDisplay = std::move( display );

	return true;
}

void VideoPlayer::Shutdown( void )
{
	mDevice->Release();
}
