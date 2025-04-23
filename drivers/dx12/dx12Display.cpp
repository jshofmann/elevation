// Elevation Engine
//
// Copyright (C) 2025 Azimuth Studios

#include "pch.h"

#include <dxgi1_6.h>

#include "dx12Display.h"

#include <drivers/dx12/dx12Device.h>

using namespace ee;

bool dx12Display::Initialize( Device* device )
{
	// We can assume that device is a dx12Device here.
	// It is required to create a swap chain.
	if( device == nullptr )
	{
		eeDebug( "dx12Display::Initialize: A dx12Device is required\n" );
		return false;
	}

	if( !device->IsInitialized() )
	{
		eeDebug( "dx12Display::Initialize: The dx12Device must be initialized first\n" );
		return false;
	}

	IDXGIFactory6* dxgiFactory = static_cast< dx12Device* >( device )->GetDXGIFactory();
	if( dxgiFactory == nullptr )
	{
		eeDebug( "dx12Display::Initialize: The dx12Device is missing its IDXGIFactory\n" );
		return false;
	}

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.Width					= width;
	swapChainDesc.Height				= height;
	swapChainDesc.Format				= SwapChainFormat;
	swapChainDesc.BufferUsage			= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount			= SWAP_CHAIN_BUFFER_COUNT;
	swapChainDesc.SampleDesc.Count		= 1;
	swapChainDesc.SampleDesc.Quality	= 0;
	swapChainDesc.Scaling				= DXGI_SCALING_NONE;
	swapChainDesc.SwapEffect			= DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.AlphaMode				= DXGI_ALPHA_MODE_IGNORE;
	swapChainDesc.Flags					= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
	fsSwapChainDesc.Windowed = TRUE;

	ASSERT_SUCCEEDED( dxgiFactory->CreateSwapChainForHwnd( g_CommandManager.GetCommandQueue(), GameCore::g_hWnd,
														   &swapChainDesc, &fsSwapChainDesc, nullptr, &s_SwapChain1 ) );

#if CONDITIONALLY_ENABLE_HDR_OUTPUT
	{
		IDXGISwapChain4*	   swapChain = (IDXGISwapChain4*) s_SwapChain1;
		ComPtr< IDXGIOutput >  output;
		ComPtr< IDXGIOutput6 > output6;
		DXGI_OUTPUT_DESC1	   outputDesc;
		UINT				   colorSpaceSupport;

		// Query support for ST.2084 on the display and set the color space accordingly
		if( SUCCEEDED( swapChain->GetContainingOutput( &output ) ) && SUCCEEDED( output.As( &output6 ) ) &&
			SUCCEEDED( output6->GetDesc1( &outputDesc ) ) &&
			outputDesc.ColorSpace == DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020 &&
			SUCCEEDED(
				swapChain->CheckColorSpaceSupport( DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020, &colorSpaceSupport ) ) &&
			( colorSpaceSupport & DXGI_SWAP_CHAIN_COLOR_SPACE_SUPPORT_FLAG_PRESENT ) &&
			SUCCEEDED( swapChain->SetColorSpace1( DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020 ) ) )
		{
			g_bEnableHDROutput = true;
		}
	}
#endif // End CONDITIONALLY_ENABLE_HDR_OUTPUT

	for( uint32_t i = 0; i < SWAP_CHAIN_BUFFER_COUNT; ++i )
	{
		ComPtr< ID3D12Resource > DisplayPlane;
		ASSERT_SUCCEEDED( s_SwapChain1->GetBuffer( i, MY_IID_PPV_ARGS( &DisplayPlane ) ) );
		g_DisplayPlane[ i ].CreateFromSwapChain( L"Primary SwapChain Buffer", DisplayPlane.Detach() );
	}

	return true;
}

void dx12Display::Release( void )
{
}

void dx12Display::EnableHDR( bool useHDR )
{
}

void dx12Display::Resize( uint32_t width, uint32_t height )
{
}

void dx12Display::Present( void )
{
}
