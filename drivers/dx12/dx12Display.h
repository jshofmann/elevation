// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

#include <ee/graphics/Display.h>

namespace ee
{
	class dx12Display : public Display
	{
	public:
		virtual bool Initialize( uint32_t width, uint32_t height, Device* device ) override final;
		virtual void Release( void ) override final;

		virtual bool IsInitialized( void ) const override final;

		virtual void GetResolution( uint32_t& width, uint32_t& height ) override final; // in pixels

		virtual uint32_t GetMonitorRefreshRate( void ) const override final; // in hertz

		virtual bool IsHDRCapable( void ) const override final;
		virtual void EnableHDR( bool useHDR ) override final;

		virtual void Resize( uint32_t width, uint32_t height ) override final;

		virtual void Present( void ) override final;

	private:

	}; // class dx12Display

	inline bool dx12Display::IsInitialized( void ) const
	{
		return false;
	}

	inline void dx12Display::GetResolution( uint32_t& width, uint32_t& height )
	{
		width = 0;
		height = 0;
	}

	uint32_t dx12Display::GetMonitorRefreshRate( void ) const
	{
		return 0;
	}

	bool dx12Display::IsHDRCapable( void ) const
	{
		return false;
	}

} // namespace ee
