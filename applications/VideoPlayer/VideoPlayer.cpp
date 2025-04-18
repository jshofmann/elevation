// Copyright (C) 2025 Azimuth Studios

#include "pch.h"

#include "VideoPlayer.h"

#include <ee/graphics/Device.h>

using namespace ee;

VideoPlayer::VideoPlayer()
	: mWidth( 0 )
	, mHeight( 0 )
{
}

VideoPlayer::~VideoPlayer()
{
}

bool VideoPlayer::Initialize( uint16_t width, uint16_t height )
{
	mWidth = width;
	mHeight = height;

	mDevice = MakeDevice();
	if( mDevice == nullptr )
		return false;

	if( !mDevice->Initialize() )
		return false;

	return true;
}

void VideoPlayer::Shutdown( void )
{
	mDevice->Release();
}
