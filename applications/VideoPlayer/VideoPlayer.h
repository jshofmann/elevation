// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

#include <stdint.h>
#include <memory>

#include <ee/graphics/Device.h>
#include <ee/graphics/Display.h>

class VideoPlayer
{
public:
	VideoPlayer();
	~VideoPlayer();

	bool Initialize( std::unique_ptr< ee::Device > device, std::unique_ptr< ee::Display > display );
	void Shutdown( void );

	inline void GetDimensions( uint16_t& width, uint16_t& height ) const;

private:
	uint16_t mWidth, mHeight; // in pixels

	std::unique_ptr< ee::Device > mDevice;
	std::unique_ptr< ee::Display > mDisplay;

}; // class VideoPlayer

inline void VideoPlayer::GetDimensions( uint16_t& width, uint16_t& height ) const
{
	width  = mWidth;
	height = mHeight;
}
