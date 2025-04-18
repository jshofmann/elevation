// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#include "pch.h"

#include <stdarg.h>
#include <stdio.h>

// Note: These functions are declared in ee/core/Debug.h and defined here,
// as they have driver-specific implementations
#include <ee/core/Debug.h>

#include "WinDebug.h"

namespace ee
{
	static const uint32_t BufferSize = MEM_KB( 2 );

	// Debugf( const char* format, ... )
	//
	// On Windows, wrap OutputDebugString() in a printf-style interface.
	//
	//	-> format -- A printf-style format string
	//	-> ...	  -- Any arguments required by format
	void Debugf( const char* format, ... )
	{
		va_list args;

		va_start( args, format );

		char buffer[ BufferSize ];
		vsnprintf( buffer, BufferSize, format, args );

		OutputDebugString( buffer );

		va_end( args );
	}


	// VDebugf( const char* format, va_list args )
	//
	// On Windows, wrap OutputDebugString() in a vprintf-style interface.
	//
	//	-> format -- A printf-style format string
	//	-> args	  -- Any arguments required by format
	void VDebugf( const char* format, va_list args )
	{
		char buffer[ BufferSize ];
		vsnprintf( buffer, BufferSize, format, args );

		OutputDebugString( buffer );
	}


	// WinDebug::GetMessageString( UINT message )
	//
	// Return a string representation of the given Windows message token
	// of the variety passed as the uMsg parameter of a Window Procedure.
	// Will return the token name for the commonly-seen WM_ tokens and a
	// hex-formatted representation of the value of all other tokens.
	//
	//  -> message -- The uMsg parameter as passed to a WndProc implementation
	// <-          -- A string representation of that uMsg value
	const char* WinDebug::GetMessageString( UINT message )
	{
		switch( message )
		{
		case WM_NULL:							return "WM_NULL";
		case WM_CREATE:							return "WM_CREATE";
		case WM_DESTROY:						return "WM_DESTROY";
		case WM_MOVE:							return "WM_MOVE";
		case WM_SIZE:							return "WM_SIZE";

		case WM_ACTIVATE:						return "WM_ACTIVATE";

		case WM_SETFOCUS:						return "WM_SETFOCUS";
		case WM_KILLFOCUS:						return "WM_KILLFOCUS";
		case WM_ENABLE:							return "WM_ENABLE";
		case WM_SETREDRAW:						return "WM_SETREDRAW";
		case WM_SETTEXT:						return "WM_SETTEXT";
		case WM_GETTEXT:						return "WM_GETTEXT";
		case WM_GETTEXTLENGTH:					return "WM_GETTEXTLENGTH";
		case WM_PAINT:							return "WM_PAINT";
		case WM_CLOSE:							return "WM_CLOSE";
		case WM_QUERYENDSESSION:				return "WM_QUERYENDSESSION";
		case WM_QUERYOPEN:						return "WM_QUERYOPEN";
		case WM_ENDSESSION:						return "WM_ENDSESSION";
		case WM_QUIT:							return "WM_QUIT";
		case WM_ERASEBKGND:						return "WM_ERASEBKGND";
		case WM_SYSCOLORCHANGE:					return "WM_SYSCOLORCHANGE";
		case WM_SHOWWINDOW:						return "WM_SHOWWINDOW";
		case WM_WININICHANGE:					return "WM_WININICHANGE";

		case WM_DEVMODECHANGE:					return "WM_DEVMODECHANGE";
		case WM_ACTIVATEAPP:					return "WM_ACTIVATEAPP";
		case WM_FONTCHANGE:						return "WM_FONTCHANGE";
		case WM_TIMECHANGE:						return "WM_TIMECHANGE";
		case WM_CANCELMODE:						return "WM_CANCELMODE";
		case WM_SETCURSOR:						return "WM_SETCURSOR";
		case WM_MOUSEACTIVATE:					return "WM_MOUSEACTIVATE";
		case WM_CHILDACTIVATE:					return "WM_CHILDACTIVATE";
		case WM_QUEUESYNC:						return "WM_QUEUESYNC";

		case WM_GETMINMAXINFO:					return "WM_GETMINMAXINFO";

		case WM_PAINTICON:						return "WM_PAINTICON";
		case WM_ICONERASEBKGND:					return "WM_ICONERASEBKGND";
		case WM_NEXTDLGCTL:						return "WM_NEXTDLGCTL";
		case WM_SPOOLERSTATUS:					return "WM_SPOOLERSTATUS";
		case WM_DRAWITEM:						return "WM_DRAWITEM";
		case WM_MEASUREITEM:					return "WM_MEASUREITEM";
		case WM_DELETEITEM:						return "WM_DELETEITEM";
		case WM_VKEYTOITEM:						return "WM_VKEYTOITEM";
		case WM_CHARTOITEM:						return "WM_CHARTOITEM";
		case WM_SETFONT:						return "WM_SETFONT";
		case WM_GETFONT:						return "WM_GETFONT";
		case WM_SETHOTKEY:						return "WM_SETHOTKEY";
		case WM_GETHOTKEY:						return "WM_GETHOTKEY";
		case WM_QUERYDRAGICON:					return "WM_QUERYDRAGICON";
		case WM_COMPAREITEM:					return "WM_COMPAREITEM";
		case WM_GETOBJECT:						return "";

		case WM_COMPACTING:						return "WM_COMPACTING";
		case WM_COMMNOTIFY:						return "WM_COMMNOTIFY";
		case WM_WINDOWPOSCHANGING:				return "WM_WINDOWPOSCHANGING";
		case WM_WINDOWPOSCHANGED:				return "WM_WINDOWPOSCHANGED";

		case WM_POWER:							return "WM_POWER";

		case WM_COPYDATA:						return "WM_COPYDATA";
		case WM_CANCELJOURNAL:					return "WM_CANCELJOURNAL";

		case WM_NOTIFY:							return "WM_NOTIFY";
		case WM_INPUTLANGCHANGEREQUEST:			return "WM_INPUTLANGCHANGEREQUEST";
		case WM_INPUTLANGCHANGE:				return "WM_INPUTLANGCHANGE";
		case WM_TCARD:							return "WM_TCARD";
		case WM_HELP:							return "WM_HELP";
		case WM_USERCHANGED:					return "WM_USERCHANGED";
		case WM_NOTIFYFORMAT:					return "WM_NOTIFYFORMAT";
		case WM_CONTEXTMENU:					return "WM_CONTEXTMENU";
		case WM_STYLECHANGING:					return "WM_STYLECHANGING";
		case WM_STYLECHANGED:					return "WM_STYLECHANGED";
		case WM_DISPLAYCHANGE:					return "WM_DISPLAYCHANGE";
		case WM_GETICON:						return "WM_GETICON";
		case WM_SETICON:						return "WM_SETICON";

		case WM_NCCREATE:						return "WM_NCCREATE";
		case WM_NCDESTROY:						return "WM_NCDESTROY";
		case WM_NCCALCSIZE:						return "WM_NCCALCSIZE";
		case WM_NCHITTEST:						return "WM_NCHITTEST";
		case WM_NCPAINT:						return "WM_NCPAINT";
		case WM_NCACTIVATE:						return "WM_NCACTIVATE";
		case WM_GETDLGCODE:						return "WM_GETDLGCODE";

		case WM_NCMOUSEMOVE:					return "WM_NCMOUSEMOVE";
		case WM_NCLBUTTONDOWN:					return "WM_NCLBUTTONDOWN";
		case WM_NCLBUTTONUP:					return "WM_NCLBUTTONUP";
		case WM_NCLBUTTONDBLCLK:				return "WM_NCLBUTTONDBLCLK";
		case WM_NCRBUTTONDOWN:					return "WM_NCRBUTTONDOWN";
		case WM_NCRBUTTONUP:					return "WM_NCRBUTTONUP";
		case WM_NCRBUTTONDBLCLK:				return "WM_NCRBUTTONDBLCLK";
		case WM_NCMBUTTONDOWN:					return "WM_NCMBUTTONDOWN";
		case WM_NCMBUTTONUP:					return "WM_NCMBUTTONUP";
		case WM_NCMBUTTONDBLCLK:				return "WM_NCMBUTTONDBLCLK";

		case WM_NCXBUTTONDOWN:					return "WM_NCXBUTTONDOWN";
		case WM_NCXBUTTONUP:					return "WM_NCXBUTTONUP";
		case WM_NCXBUTTONDBLCLK:				return "WM_NCXBUTTONDBLCLK";

		case WM_INPUT_DEVICE_CHANGE:			return "WM_INPUT_DEVICE_CHANGE";
		case WM_INPUT:							return "WM_INPUT";

		case WM_KEYDOWN:						return "WM_KEYDOWN";
		case WM_KEYUP:							return "WM_KEYUP";
		case WM_CHAR:							return "WM_CHAR";
		case WM_DEADCHAR:						return "WM_DEADCHAR";
		case WM_SYSKEYDOWN:						return "WM_SYSKEYDOWN";
		case WM_SYSKEYUP:						return "WM_SYSKEYUP";
		case WM_SYSCHAR:						return "WM_SYSCHAR";
		case WM_SYSDEADCHAR:					return "WM_SYSDEADCHAR";

		case WM_IME_STARTCOMPOSITION:			return "WM_IME_STARTCOMPOSITION";
		case WM_IME_ENDCOMPOSITION:				return "WM_IME_ENDCOMPOSITION";
		case WM_IME_COMPOSITION:				return "WM_IME_COMPOSITION";

		case WM_INITDIALOG:						return "WM_INITDIALOG";
		case WM_COMMAND:						return "WM_COMMAND";
		case WM_SYSCOMMAND:						return "WM_SYSCOMMAND";
		case WM_TIMER:							return "WM_TIMER";
		case WM_HSCROLL:						return "WM_HSCROLL";
		case WM_VSCROLL:						return "WM_VSCROLL";
		case WM_INITMENU:						return "WM_INITMENU";
		case WM_INITMENUPOPUP:					return "WM_INITMENUPOPUP";

		case WM_GESTURE:						return "WM_GESTURE";
		case WM_GESTURENOTIFY:					return "WM_GESTURENOTIFY";

		case WM_MENUSELECT:						return "WM_MENUSELECT";
		case WM_MENUCHAR:						return "WM_MENUCHAR";
		case WM_ENTERIDLE:						return "WM_ENTERIDLE";
		case WM_MENURBUTTONUP:					return "WM_MENURBUTTONUP";
		case WM_MENUDRAG:						return "WM_MENUDRAG";
		case WM_MENUGETOBJECT:					return "WM_MENUGETOBJECT";
		case WM_UNINITMENUPOPUP:				return "WM_UNINITMENUPOPUP";
		case WM_MENUCOMMAND:					return "WM_MENUCOMMAND";

		case WM_CHANGEUISTATE:					return "WM_CHANGEUISTATE";
		case WM_UPDATEUISTATE:					return "WM_UPDATEUISTATE";
		case WM_QUERYUISTATE:					return "WM_QUERYUISTATE";

		case WM_CTLCOLORMSGBOX:					return "WM_CTLCOLORMSGBOX";
		case WM_CTLCOLOREDIT:					return "WM_CTLCOLOREDIT";
		case WM_CTLCOLORLISTBOX:				return "WM_CTLCOLORLISTBOX";
		case WM_CTLCOLORBTN:					return "WM_CTLCOLORBTN";
		case WM_CTLCOLORDLG:					return "WM_CTLCOLORDLG";
		case WM_CTLCOLORSCROLLBAR:				return "WM_CTLCOLORSCROLLBAR";
		case WM_CTLCOLORSTATIC:					return "WM_CTLCOLORSTATIC";

		case WM_MOUSEMOVE:						return "WM_MOUSEMOVE";
		case WM_LBUTTONDOWN:					return "WM_LBUTTONDOWN";
		case WM_LBUTTONUP:						return "WM_LBUTTONUP";
		case WM_LBUTTONDBLCLK:					return "WM_LBUTTONDBLCLK";
		case WM_RBUTTONDOWN:					return "WM_RBUTTONDOWN";
		case WM_RBUTTONUP:						return "WM_RBUTTONUP";
		case WM_RBUTTONDBLCLK:					return "WM_RBUTTONDBLCLK";
		case WM_MBUTTONDOWN:					return "WM_MBUTTONDOWN";
		case WM_MBUTTONUP:						return "WM_MBUTTONUP";
		case WM_MBUTTONDBLCLK:					return "WM_MBUTTONDBLCLK";
		case WM_MOUSEWHEEL:						return "WM_MOUSEWHEEL";

		case WM_XBUTTONDOWN:					return "WM_XBUTTONDOWN";
		case WM_XBUTTONUP:						return "WM_XBUTTONUP";
		case WM_XBUTTONDBLCLK:					return "WM_XBUTTONDBLCLK";

		case WM_PARENTNOTIFY:					return "WM_PARENTNOTIFY";
		case WM_ENTERMENULOOP:					return "WM_ENTERMENULOOP";
		case WM_EXITMENULOOP:					return "WM_EXITMENULOOP";

		case WM_NEXTMENU:						return "WM_NEXTMENU";
		case WM_SIZING:							return "WM_SIZING";
		case WM_CAPTURECHANGED:					return "WM_CAPTURECHANGED";
		case WM_MOVING:							return "WM_MOVING";

		case WM_POWERBROADCAST:					return "WM_POWERBROADCAST";

		case WM_IME_SETCONTEXT:					return "WM_IME_SETCONTEXT";
		case WM_IME_NOTIFY:						return "WM_IME_NOTIFY";
		case WM_IME_CONTROL:					return "WM_IME_CONTROL";
		case WM_IME_COMPOSITIONFULL:			return "WM_IME_COMPOSITIONFULL";
		case WM_IME_SELECT:						return "WM_IME_SELECT";
		case WM_IME_CHAR:						return "WM_IME_CHAR";
		case WM_IME_REQUEST:					return "WM_IME_REQUEST";
		case WM_IME_KEYDOWN:					return "WM_IME_KEYDOWN";
		case WM_IME_KEYUP:						return "WM_IME_KEYUP";

		case WM_DWMCOMPOSITIONCHANGED:			return "WM_DWMCOMPOSITIONCHANGED";
		case WM_DWMNCRENDERINGCHANGED:			return "WM_DWMNCRENDERINGCHANGED";
		case WM_DWMCOLORIZATIONCOLORCHANGED:	return "WM_DWMCOLORIZATIONCOLORCHANGED";
		case WM_DWMWINDOWMAXIMIZEDCHANGE:		return "WM_DWMWINDOWMAXIMIZEDCHANGE";

		} // switch( message )

		static char buffer[ 8 ];
		snprintf( buffer, 8, "0x%04x", message );
		return buffer;
	}

} // namespace ee
