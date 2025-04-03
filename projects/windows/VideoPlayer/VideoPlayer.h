// Copyright (C) 2025 Azimuth Studios

#pragma once

#include <stdint.h>

class VideoPlayer
{
public:
	VideoPlayer();
	~VideoPlayer();

	bool Initialize( uint16_t width, uint16_t height );

	inline void GetDimensions( uint16_t& width, uint16_t& height ) const;

private:
	uint16_t				mWidth, mHeight; // in pixels

}; // class VideoPlayer

inline void VideoPlayer::GetDimensions( uint16_t& width, uint16_t& height ) const
{
	width = mWidth;
	height = mHeight;
}
