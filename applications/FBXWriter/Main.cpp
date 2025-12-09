// FBXWriter application - part of Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#include "pch.h"

#include <cstdint>

#include <ee/core/Debug.h>
#include <ee/core/Application.h>

#include "FBXWriter.h"

using namespace ee;

class FBXWriterApplication : public Application
{
public:
	// Application interface implementation

	virtual const char* GetName( void ) const override final
	{
		return "FBXWriter";
	}

	virtual int Main( int argCount, const char* args[] ) override final;

	// FBXWriterApplication member functions


}; // class FBXWriterApplication

// Because this object represents the running application (ie .exe file)
// a Meyer singleton is an appropriate pattern to use here
Application& Application::GetInstance( void )
{
	static FBXWriterApplication application;
	return application;
}


int FBXWriterApplication::Main( int argCount, const char* args[] )
{
	return 0;
}

int main( int argc, const char* argv[] )
{
	// Trigger the instantiation of the FBXWriterApplication object
	FBXWriterApplication& application = static_cast< FBXWriterApplication& >( Application::GetInstance() );

	if( application.Initialize() )
	{
		return application.Main( argc,argv );
	}
}
