// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

namespace ee
{
	class Config;

	/*
	*/
	class Application
	{
	public:
		// Application::GetInstance() is a "static virtual" function
		// that each application is expected to implement. This is
		// an instance of the Meyer singleton pattern.
		static Application& GetInstance( void );

		virtual const char* GetName( void ) const = 0;

		virtual int Main( int argCount, const char* args[] ) = 0;

		// Optional application-specific initialize and teardown hooks
		virtual bool Initialize( void ) { return true; }
		virtual void Shutdown( void ) {}

		// Applications can call this to signal that the application
		// should be shut down (e.g. when File->Quit is selected)
		virtual void Exit( void ) {}

		// It's expected that Application::Update() will be called once per
		// simulation frame - by default, one simulation frame == one render
		// frame, but some applications may decouple those loops
		virtual bool Update( void ) { return false; }

		// If your application supports use of config files,
		// implement this accessor. Users should assume that the Application
		// owns the Config object and manages its lifetime.
		virtual Config* GetConfig( void ) noexcept { return nullptr; }

	}; // class Application

} // namespace ee
