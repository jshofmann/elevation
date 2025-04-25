// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

#include <memory>

#include <ee/core/PlatformTypes.h>

namespace ee
{
	class Display;

	// The Device concept maps to a device in a target driver-
	// e.g. a ID3D12Device, VkDevice, or WGPUDevice
	class Device
	{
	public:
		virtual bool Initialize( void ) = 0;
		virtual void Release( void ) = 0;

		virtual bool IsInitialized( void ) const = 0;

		// For the drivers that need this,
		// a mechanism to recover from a lost device error
		virtual bool Recover( void ) { return true; }

		// MakeDisplay() will create a Display appropriate for this Device
		virtual std::unique_ptr< Display > MakeDisplay( void ) const = 0;

	}; // class Device

	// MakeDevice() will create a device appropriate for the requested Driver.
	// If Driver::kNative is used then it will pick the default driver for
	// the host operating system this program is running on.
	std::unique_ptr< Device > MakeDevice( Driver driver = Driver::kNative );

} // namespace ee
