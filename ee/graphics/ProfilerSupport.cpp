// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#include "pch.h"

#include "ProfilerSupport.h"

using namespace ee;

#if defined( BUILD_PROFILE )

#if defined( BUILD_PC )
#include <tchar.h>
#include <pix.h>
#endif

#if defined( BUILD_RAD_TELEMETRY )
#include <rad_tm.h>
#endif

#if defined( BUILD_SUPERLUMINAL )
#include <superluminal/API/include/superluminal/PerformanceAPI.h>
extern PerformanceAPI_Functions gSuperluminal; // defined in Profiler.cpp
#endif

using namespace ee;

// color should be in the format 0xffRRGGBB
void ee::PushGraphicsEvent( const char* name, const wchar_t* widename, uint32_t color )
{
#if defined( BUILD_PC )

	switch( System::GetDriver() )
	{
	case Driver::kVulkan:
	{
//		vulGraphicsInstance.BeginDebugEvent( name, color );
	}
	break;

	case Driver::kDirectX12:
	{
		// Hook GraphicsEventImpl up to both the GPU and CPU profilers
//		dx12GraphicsInstance.PushGpuMarker( widename );
		PIXBeginEvent( color, widename );
	}
	break;

	} // switch( System::GetDriver() )

#elif defined( BUILD_LINUX )

//		vulGraphicsInstance.BeginDebugEvent( name, color );

#endif // #elif defined( BUILD_LINUX )

#if defined( BUILD_SUPERLUMINAL )

	if( gSuperluminal.BeginEvent != NULL )
	{
		gSuperluminal.BeginEvent( name, NULL, color );
	}

#endif // #if defined( BUILD_SUPERLUMINAL )

#if defined( BUILD_RAD_TELEMETRY )

	tmEnter( 0, TMZF_NONE, name );

#endif // #if defined( BUILD_RAD_TELEMETRY )
}

// color should be in the format 0xffRRGGBB
void ee::PushGraphicsNamedEvent( uint32_t color, const char* format, va_list args )
{
	char name[ 64 ];

#if defined( BUILD_PC )

	// D3DPERF_BeginEvent requires a wide string; our arguments are
	// const char* so we need to execute printf() before converting
	vsnprintf_s( name, sizeof( name ), _TRUNCATE, format, args );

	// Convert to wide string for D3DPERF_BeginEvent()
	wchar_t widename[ 64 ];
	size_t convertedCount;
	mbstowcs_s( &convertedCount, widename, 64, name, _TRUNCATE );

	switch( System::GetDriver() )
	{
	case Driver::kVulkan:
	{
//		vulGraphicsInstance.BeginDebugEvent( name, color );
	}
	break;

	case Driver::kDirectX12:
	{
//		dx12GraphicsInstance.PushGpuMarker( widename );
		PIXBeginEvent( color, widename );
	}
	break;

	} // switch( System::GetDriver() )

#elif defined( BUILD_LINUX )

//		vulGraphicsInstance.BeginDebugEvent( name, color );

#endif // #elif defined( BUILD_LINUX )

#if defined( BUILD_SUPERLUMINAL )

	if( gSuperluminal.BeginEvent != NULL )
	{
		gSuperluminal.BeginEvent( name, NULL, color );
	}

#endif // #if defined( BUILD_SUPERLUMINAL )

#if defined( BUILD_RAD_TELEMETRY )

	tmEnter( 0, TMZF_NONE, name );

#endif // #if defined( BUILD_RAD_TELEMETRY )
}

void ee::PopGraphicsEvent( void )
{
#if defined( BUILD_PC )

	switch( System::GetDriver() )
	{
	case Driver::kVulkan:
	{
//		vulGraphicsInstance.EndDebugEvent();
	}

	case Driver::kDirectX12:
	{
//		dx12GraphicsInstance.PopGpuMarker();
		PIXEndEvent();
	}
	break;

	} // switch( System::GetDriver() )

#elif defined( BUILD_LINUX )

//		vulGraphicsInstance.EndDebugEvent();

#endif // #elif defined( BUILD_LINUX )

#if defined( BUILD_SUPERLUMINAL )

	if( gSuperluminal.EndEvent != NULL )
	{
		gSuperluminal.EndEvent();
	}

#endif // #if defined( BUILD_SUPERLUMINAL )

#if defined( BUILD_RAD_TELEMETRY )

	tmLeave( 0 );

#endif // #if defined( BUILD_RAD_TELEMETRY )
}

#endif // #if defined( BUILD_PROFILE )
