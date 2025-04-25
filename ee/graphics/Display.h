// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

namespace ee
{
	class Device;

	// The Display controls the output to a display device (e.g. monitor);
	// it manages the swap chain and output configuration. On Windows, this
	// maps to DXGI's responsibilities; in Vulkan, VkSwapChain and its
	// Win32 extensions.
	class Display
	{
	public:
		// Setup/teardown is very driver-specific and not done here

		virtual void GetResolution( uint32_t& width, uint32_t& height ) = 0; // in pixels

		virtual uint32_t GetMonitorRefreshRate( void ) const = 0; // in hertz

		virtual bool IsHDRCapable( void ) const = 0;
		virtual void EnableHDR( bool useHDR ) = 0;

		virtual void Resize( uint32_t width, uint32_t height ) = 0;

		virtual void Present( void ) = 0;

	}; // class Display

} // namespace ee
