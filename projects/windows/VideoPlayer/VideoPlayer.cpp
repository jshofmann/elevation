// Copyright (C) 2025 Azimuth Studios

#include "pch.h"

#include "VideoPlayer.h"

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

	return true;
}
