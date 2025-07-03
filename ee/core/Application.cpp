// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#include "pch.h"

#include "Application.h"

using namespace ee;

int Application::Main( int argCount, const char* args[] )
{
	mRunning = OnStart();

	while( mRunning )
	{
		if( !Update() )
		{
			break;
		}

	} // while( mRunning )

	OnStop();

	Shutdown();

	return 0;
}

void Application::Exit( void )
{
	mRunning = false;
}
