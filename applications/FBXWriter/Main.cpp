// FBXWriter application - part of Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#include "pch.h"

#include <ufbx/ufbx.h>

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

// Because this object represents the running application (ie the .exe file)
// a Meyer singleton is an appropriate pattern to use here
Application& Application::GetInstance( void )
{
	static FBXWriterApplication application;
	return application;
}

static void printUsage( void )
{
		eeDebug( "FBXWriterApplication: Usage: FBXWriter.exe <input .fbx file>\n" );
}

int FBXWriterApplication::Main( int argCount, const char* args[] )
{
	if( argCount == 0 || args == nullptr )
	{
		printUsage();
		return -1;
	}

	// Usage: FBXWriter <input .fbx file>
	// this will read in the given file and write it out to test.fbx

	// On Windows we aren't guaranteed that args[ 0 ] is the exe name; see
	// https://learn.microsoft.com/en-us/cpp/cpp/main-function-command-line-arsg
	// "Note: By convention, argv[0] is the filename of the program. However,
	// on Windows it's possible to spawn a process by using CreateProcess.
	// If you use both the first and second arguments (lpApplicationName and
	// lpCommandLine), argv[0] may not be the executable name."
	//
	// So, scan each member of args looking for a '.fbx' extension
	// to identify the member containing the input filename.
	const char* filename = nullptr;
	for( int i = 0; i < argCount; ++i )
	{
		if( strstr( args[ i ], ".fbx" ) != 0 )
		{
			filename = args[ i ];
			break;
		}
	}

	if( filename == nullptr )
	{
		printUsage();
		return -1;
	}

	// Use https://github.com/ufbx/ufbx to read in the input .fbx file
	ufbx_load_opts opts = { 0 }; // Optional, pass NULL for defaults
	ufbx_error error; // Optional, pass NULL if you don't care about errors
	ufbx_scene* scene = ufbx_load_file( filename, &opts, &error );
	if( scene == nullptr )
	{
		eeDebug( "FBXWriterApplication: Failed to load %s: %s\n", filename, error.description.data );
		return -1;
	}

	for( size_t i = 0; i < scene->nodes.count; i++ )
	{
		ufbx_node* node = scene->nodes.data[ i ];
		if( node->is_root )
			continue;

		eeDebug( "Object: %s\n", node->name.data );
		if( node->mesh != nullptr )
		{
			eeDebug( "-> mesh with %zu faces\n", node->mesh->faces.count );
		}

	} // for( size_t i = 0; i < scene->nodes.count; i++ )

	ufbx_free_scene( scene );

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
