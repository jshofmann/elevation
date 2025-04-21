// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

#include <wrl/client.h>

#include <d3d12.h>
#include <dxcapi.h>
#include <dxgi1_6.h>

#include <ee/core/Application.h>
#include <ee/graphics/Device.h>

namespace ee
{
	class dx12Device : public Device
	{
	public:
		static constexpr uint32_t kInvalidGPUIndex = ~0u;

		// IDevice interface implementation

		virtual bool Initialize( void );
		virtual void Release( void );

		virtual bool IsInitialized( void ) const;

		// dx12Device implementation

		// For lack of a better place to store this....
		IDxcUtils* GetDxcUtils( void ) const;

	private:
		bool CreateDevice( void );

		// Note: Use of the debug layer is not gated by BUILD_RETAIL
		// because we want the option of enabling DRED in retail builds
		HRESULT EnableDebugLayer( void );

		HRESULT CreateDeviceFactory( void );

		HRESULT VerifyMinimumFeatureLevel( IDXGIAdapter1* adapter, D3D_FEATURE_LEVEL level ) const;

#if !defined( BUILD_RETAIL )

		bool LoadPIXDLL( void );

#endif // #if !defined( BUILD_RETAIL )

		Microsoft::WRL::ComPtr< IDXGIFactory6 > mDXGIFactory;
		Microsoft::WRL::ComPtr< IDXGIAdapter1 > mAdapter;

		Microsoft::WRL::ComPtr< ID3D12Debug > mD3D12Debug;

		Microsoft::WRL::ComPtr< ID3D12Device > mDevice;
		ThreadID							   mDeviceThreadID = kInvalidThreadID;

		bool	mAllowPIXAttach = true;
		HMODULE mPIXCapturerModule = NULL;

		D3D_FEATURE_LEVEL mFeatureLevel = D3D_FEATURE_LEVEL_12_0;

		// For lack of a better place to store this....
		Microsoft::WRL::ComPtr< IDxcUtils > mDxcUtils;

	}; // class dx12Device

	inline bool dx12Device::IsInitialized( void ) const
	{
		return mDevice != NULL;
	}

	inline IDxcUtils* dx12Device::GetDxcUtils( void ) const
	{
		return mDxcUtils.Get();
	}

} // namespace ee
