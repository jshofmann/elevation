// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

#include <wrl/client.h>
#include <dxgi1_6.h>

#include <ee/graphics/Display.h>
#include <drivers/dx12/dx12ColorBuffer.h>

namespace ee
{
	class dx12Display : public Display
	{
	public:
		bool Initialize( uint32_t width, uint32_t height, DXGI_FORMAT format, HWND hwnd, Device* device );
		void Release( void );

		virtual void GetResolution( uint32_t& width, uint32_t& height ) override final; // in pixels

		virtual uint32_t GetMonitorRefreshRate( void ) const override final; // in hertz

		virtual bool IsHDRCapable( void ) const override final;
		virtual void EnableHDR( bool useHDR ) override final;

		virtual void Resize( uint32_t width, uint32_t height ) override final;

		virtual void Present( void ) override final;

	private:
		enum
		{
			kSwapChainBufferCount = 3
		};

		Microsoft::WRL::ComPtr< IDXGISwapChain1 > mSwapChain;
		dx12ColorBuffer mFrameBuffers[ kSwapChainBufferCount ];

		bool mHDROutputEnabled = false;

	}; // class dx12Display

	inline void dx12Display::GetResolution( uint32_t& width, uint32_t& height )
	{
		width = 0;
		height = 0;
	}

	inline uint32_t dx12Display::GetMonitorRefreshRate( void ) const
	{
		return 0;
	}

	inline bool dx12Display::IsHDRCapable( void ) const
	{
		return false;
	}

} // namespace ee
