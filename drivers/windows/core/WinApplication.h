// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

#include <ee/core/Application.h>

#include <drivers/windows/core/WinWindow.h>

namespace ee
{
	class WinApplication : public Application
	{
	public:
		// Application interface implementation

		virtual void Exit( void ) override;

		virtual bool Update( void ) override;

		// WinApplication member functions

		HINSTANCE GetHInstance( void ) const;
		void	  SetHInstance( HINSTANCE hinstance );

		HWND GetParentWindow( void ) const;
		void SetParentWindow( HWND hwnd );

		HACCEL GetAccelTable( void ) const;
		void   SetAccelTable( HACCEL accel );

		WinWindow&		 GetApplicationWindow( void );
		const WinWindow& GetApplicationWindow( void ) const;

		// WinApplication class functions

		static WinApplication& GetInstance( void );

	protected:
		HINSTANCE mHInstance	= NULL;
		HWND	  mParentWindow = NULL;
		HACCEL	  mAccelTable	= NULL;
		bool	  mRunning		= false;

		WinWindow mApplicationWindow;

	}; // class WinApplication

	inline HINSTANCE WinApplication::GetHInstance( void ) const
	{
		return mHInstance;
	}

	inline HWND WinApplication::GetParentWindow( void ) const
	{
		return mParentWindow;
	}

	inline HACCEL WinApplication::GetAccelTable( void ) const
	{
		return mAccelTable;
	}

	inline WinWindow& WinApplication::GetApplicationWindow( void )
	{
		return mApplicationWindow;
	}

	inline const WinWindow& WinApplication::GetApplicationWindow( void ) const
	{
		return mApplicationWindow;
	}

} // namespace ee
