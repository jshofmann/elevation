// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

#include <ee/graphics/GpuFormat.h>

namespace ee
{
	class Window;
	class Device;

	// The Display controls the output to a display device (e.g. monitor);
	// it manages the swap chain and output configuration. On Windows, this
	// maps to DXGI's responsibilities; in Vulkan, VkSwapChain and its
	// Win32 extensions.
	class Display
	{
	public:
		// Fix 'delete of an abstract class that has a non-virtual destructor
		// results in undefined behavior' build warning
		virtual ~Display() {}

		virtual bool Initialize( GpuFormat format, Window* window, Device* device ) = 0;
		virtual void Release( void ) = 0;

		virtual void GetResolution( uint32_t& width, uint32_t& height ) = 0; // in pixels

		virtual uint32_t GetMonitorRefreshRate( void ) const = 0; // in hertz

		virtual bool IsHDRCapable( void ) const = 0;
		virtual void EnableHDR( bool useHDR ) = 0;

		virtual void Resize( uint32_t width, uint32_t height ) = 0;

		virtual void Present( void ) = 0;

	}; // class Display

} // namespace ee
