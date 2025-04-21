// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#include "pch.h"

#include "FilePath.h"

using namespace ee;

FilePath& FilePath::Append( const std::string_view& path )
{
	std::string_view root = GetPathRoot( path );
	if( !root.empty() )
	{
		mPath.assign( path );
		return *this;
	}

	if( !mPath.empty() && !mPath.ends_with( '/' ) && !mPath.ends_with( '\\' ) )
	{
		// Ensure the two appends don't lead to two allocations
		mPath.reserve( mPath.size() + 1 + path.size() );

		// Always append the preferred separator. This makes the class harder
		// to test, but matches the behavior of std::filesystem::path and
		// more importantly ensures that users can build extended-length
		// file paths on Windows which requires using backslashes.
		mPath.append( 1, Separator );
	}

	mPath.append( path );

	return *this;
}

std::string_view FilePath::GetPathRoot( const std::string_view& path ) const
{
	// No path? No root.
	if( path.empty() )
	{
		return {};
	}

	if constexpr( System::GetPlatform() == Platform::kWindows )
	{
		// Windows paths are incredibly complicated and this behavior differs
		// from std::filesystem::path in Microsoft's STL. There is a lot of
		// inconsistency between different APIs and different parts of Windows
		// on what path types are supported.
		//
		// Windows supports multiple root directories, such as C:\ and
		// \\server\share, and each can have a different working directory.
		// With cmd.exe, it's not possible to browse up the tree beyond the
		// root. For example, 'dir C:\..' shows 'C:\' and 'dir \\server\share\..'
		// shows '\\server\share'. While UNC paths to network shares look like
		// you can browse up a directory, both the server and share together
		// form the volume. You can see this locally using 'dir \\localhost\C$\..'.
		//
		// Our behavior tries to match this and always provide the root of the
		// volume. If there is no volume, there is no root. This is the simplest
		// and most consistent behavior, but has some consequences. For example,
		// \\server is treated as having no root because it's a bad UNC path,
		// but \server has a root because that is a valid path. While redundant
		// separators are often ignored, there are places in Windows where they
		// are not allowed (including here) and therefore it isn't too uncommon
		// to see this. We try to match the Win32 APIs. If the path wouldn't
		// resolve correctly, then we consider it a bad path with no root.
		//
		// Drive Absolute:
		//
		// A drive letter followed by a colon and a separator (backslash)
		// is an absolute path.
		//
		//     "C:\Windows\explorer.exe"
		//     GetPathRoot() returns "C:\"
		//     std::filesystem::path::root_path() returns "C:\"
		//
		// Redundant separators are allowed such as "C:\\\Windows\\\explorer.exe".
		//
		// Drive letters must be A-Z or a-z. Most of Windows seems to enforce
		// this, as does the STL, but technically it doesn't appear to be
		// necessary. If the symbolic link was added to the \GLOBAL?? objects
		// list then it seems like the underlying Rtl functions will accept
		// them. However, I see no reason why we need to support this behavior.
		//
		// Drive Relative:
		//
		// A drive letter followed by a colon but without the separator is
		// a relative path to the current directory of that drive.
		//
		//     "C:explorer.exe"
		//     GetPathRoot() returns "C:"
		//     std::filesystem::path::root_path() returns "C:"
		//
		// In Windows, each root can have its own working directory. This is
		// generally only visible in the shell. A process has a current
		// directory and the Win32 API SetCurrentDirectory only allows changing
		// that single working directory. However, this functionality exists
		// in all processes and the behavior will differ depending on how the
		// process was launched.
		//
		// If the drive letter matches the drive letter of the current directory
		// for the process, it uses that. Otherwise, it looks for an environment
		// variable named "=C:" (replacing C with the drive letter).
		// So for example, "=C:=C:\Windows" may be set. If that environment
		// variable doesn't exist, it uses the root of the drive. You can view
		// this information using "!peb" in WinDbg under Environment and also
		// CurrentDirectory.
		//
		// When double-clicking an executable in Windows Explorer, only
		// CurrentDirectory is set. However, if you launch using the shell,
		// the process inherits that environment and may have the environment
		// variables set.
		//
		// Rooted:
		//
		// A path beginning with a separator is a path from the root
		// of the drive currently set as the working directory.
		//
		//     "\Windows\explorer.exe"
		//     GetPathRoot() returns "\"
		//     std::filesystem::path::root_path() returns "\"
		//
		// Redundant leading separators are not allowed, because that forms
		// a UNC path instead. However, redundant separators elsewhere are fine.
		//
		// This chooses "\" as the root, because no matter how many times you
		// go up the tree, you want to remain rooted to the drive and not
		// become relative to the working directory. Consider this:
		//
		//     C:\Users\me\Desktop>dir \..\Windows
		//
		// The path is rooted to C:, goes up a directory (still C:\),
		// then accesses C:\Windows. When performing a similar operation
		// with this API, you would expect the parent of a rooted path
		// to always remain rooted, such that when you append to it
		// the final path remains rooted.
		//
		// Relative:
		//
		// Paths relative to the current working directory have no root.
		//
		//     "abc/def.txt"
		//     GetPathRoot() returns ""
		//     std::filesystem::path::root_path() returns ""
		//
		// Redundant separators are allowed.
		//
		// UNC:
		//
		// A UNC path is used to access network resources. It begins with
		// two separators, then the server name, and the share name.
		// The server name and share name together make up the volume.
		//
		//     "\\MyServer\MyShare\abc\def.txt"
		//     GetPathRoot() returns "\\MyServer\MyShare"
		//     std::filesystem::path::root_path() returns "\\MyServer"
		//
		// Redundant leading slashes are not allowed.
		// Redundant slashes elsewhere are fine.
		//
		// Since the volume is represented by the server name and the share name
		// together, this is technically the root of the file system. You can't
		// go up a directory and browse all shares on the server,
		// that is not how it works.
		//
		// You can test this locally using "\\localhost\C$" to access the
		// C drive. In cmd, run "dir \\localhost\C$\.." and you can verify
		// it shows "C:\". This is the same behavior when accessing actual
		// network shares.
		//
		// It's also possible to produce UNC paths using device paths "\\.\"
		// and extended-length paths "\\?\UNC" and they behave the same way:
		// returning the root through the share name.
		//
		// Device:
		//
		// A device path begins with "\\.\" and provides direct access to the
		// DosDevices object manager directory. These are usually used to access
		// devices and pipes, but can be used for files as well even though MSDN
		// discourages it: "If you're working with Windows API functions, you
		// should use the "\\.\" prefix to access devices only and not files."
		//
		//     "\\.\C:\Windows\explorer.exe"
		//     GetPathRoot() returns "\\.\C:\"
		//     std::filesystem::path::root_path() returns "\\.\"
		//
		//     "\\.\UNC\MyServer\MyShare\abs\def.txt"
		//     GetPathRoot() returns "\\.\UNC\MyServer\MyShare"
		//     std::filesystem::path::root_path() returns "\\.\"
		//
		// Redundant leading slashes are not allowed.
		//
		// Windows behavior is a bit strange here. "\\.\C:" is not valid but
		// "\\.\C:\" is, although for UNC paths the trailing slash is not necessary.
		//
		// Extended-Length:
		//
		// An extended-length path begins with "\\?\" and has some special
		// properties that bypass some of the usual path transformations.
		// For example, dots "." and dot-dots ".." are not resolved and
		// can actually be part of a valid path. Additionally, backslashes must
		// be used. This form also allows use of paths that exceed MAX_PATH.
		//
		//     "\\?\C:\Windows\explorer.exe"
		//     GetPathRoot() returns "\\?\C:\"
		//     std::filesystem::path::root_path() returns "\\?\"
		//
		//     "\\?\UNC\MyServer\MyShare\abs\def.txt"
		//     GetPathRoot() returns "\\?\UNC\MyServer\MyShare"
		//     std::filesystem::path::root_path() returns "\\?\"
		//
		// Redundant leading slashes are not allowed.
		//
		// What About Bad Paths?
		//
		// C:\\ Yes
		// \\ No
		// \\? no
		// \\?\
		//
		// The path types below come from RTL_PATH_TYPE, although the public
		// facing names can differ. The best reference is actually not on MSDN:
		//
		// https://googleprojectzero.blogspot.com/2016/02/the-definitive-guide-on-win32-to-nt.html
		// https://learn.microsoft.com/en-us/windows/win32/fileio/naming-a-file
		// https://learn.microsoft.com/en-us/dotnet/standard/io/file-path-formats

		size_t len = path.size();
		if( len == 0 )
		{
			return {};
		}

		char first = path.front();

		// Handle drive absolute and drive relative paths.
		//
		// We enforce drive letters being ASCII letters, matching most Windows
		// behavior. Although technically adding a symbolic link to the
		// GLOBAL?? object table for a non-letter is possible, there's no known
		// benefit to support that bizarre case.
		if( len >= 2 && path[ 1 ] == ':' && std::isalpha( first ) )
		{
			if( len >= 3 )
			{
				char third = path[ 2 ];
				if( ( third == '\\' || third == '/' ) )
				{
					// Return "C:\"
					return path.substr( 0, 3 );
				}
			}

			// Return "C:"
			return path.substr( 0, 2 );
		}

		// Any paths not beginning with a separator are relative paths.
		if( first != '\\' && first != '/' )
		{
			return {};
		}

		// Handle special case of "\"
		if( len == 1 )
		{
			return path.substr( 0, 1 );
		}

		// Rooted path of the form "\Windows\explorer.exe" returns "\"
		char second = path[ 1 ];
		if( second != '\\' && second != '/' )
		{
			return path.substr( 0, 1 );
		}

		// Everything beyond this point is either a UNC path (\\server\share),
		// a device path (\\.\) or an extended-length path (\\?\).
		//
		// Shortest valid UNC path is 5: \\a\b
		// Shortest device path is 5(?): \\.\a
		// Shortest extended-length path is 5(?): \\?\a
		//
		// Actual shortest device and extended-length paths are not unknown,
		// but at the very least there needs to be one character after the
		// prefix. The exact limit isn't needed here, except to ensure it's
		// at least 3.
		if( len <= 3 )
		{
			return {};
		}

		// Handle UNC paths.
		//
		// Note: Three leading slashes is not a valid UNC path,
		// so we ignore that case and return no root.
		char third = path[ 2 ];
		if( third != '?' && third != '.' )
		{
			size_t serverEndPos = path.find_first_of( "\\/", 2 );
			if( serverEndPos == std::string_view::npos )
			{
				// This is a UNC path that doesn't contain a share name.
				// Both the server and shares must be present to form
				// the root of a volume, so this doesn't have a root.
				return {};
			}

			// Skip any redundant slashes. If this is npos, it means
			// the string ends with a separator: "\\server\"
			size_t shareStartPos = path.find_first_not_of( "\\/", serverEndPos + 1 );
			if( shareStartPos == std::string_view::npos )
			{
				return {};
			}

			// Return "\\server\share"
			size_t shareEndPos = path.find_first_of( "\\/", shareStartPos );
			return path.substr( 0, shareEndPos );
		}

		// The remaining are either device paths or extended-length paths.
		// There are a couple differences from UNC paths here:
		//
		// * Paths to the root of a drive letter end with a separator: \\?\C:\
		// * UNC paths have one extra component: \\?\UNC\server\share
		//
		// Windows handles these slightly differently. Device paths allow
		// some extra separators where extended-length paths bypass some
		// normal path processing and therefore don't allow them at all.
		// But is that a problem we need to fix? Not sure.

		// Parse what comes after \\?\ to find the volume or device name.
		//
		// Device paths allow extra slashes \\.\\\C:\ but extended-length paths
		// do not. We're allowing them here for simplicity.
		size_t volumeEndPos = path.find_first_not_of( "\\/", 3 );
		volumeEndPos		= path.find_first_of( "\\/", volumeEndPos );

		// Assuming all valid paths contain at least one separator.
		//
		// Valid UNC paths need at least two: UNC\server\share
		//
		// Device paths are a bit of an unknown. \\.\C: is not a valid path
		// but \\.\C:\ is, and similarly with the form \\.\Volume{GUID} which
		// needs a trailing separator to form a valid path. Hard to verify
		// with other devices though. Win32 APIs say COM1 exists and its full
		// path is \\.\COM1 but \\.\COM1 doesn't exist and neither does
		// \\.\COM1\ so I'm unsure.
		//
		// Extended-length paths are similar. \\?\C:\ requires the trailing
		// slash, so does \\?\Volume{GUID}\ so it seems easiest to just expect
		// one for now until we have a case where this fails.
		//
		if( volumeEndPos == std::string_view::npos )
		{
			// Assume all valid roots need to contain a backslash.
			return {};
		}

		std::string_view volume( path );
		volume = path.substr( 4, volumeEndPos != std::string_view::npos ? volumeEndPos - 4 : std::string_view::npos );

		if( volume.size() == 3 )
		{
			char u = std::toupper( volume[ 0 ] );
			char n = std::toupper( volume[ 1 ] );
			char c = std::toupper( volume[ 2 ] );

			if( u == 'U' && n == 'N' && c == 'C' )
			{
				size_t serverEndPos = path.find_first_of( "\\/", volumeEndPos + 1 );
				if( serverEndPos == std::string_view::npos )
				{
					// This is a UNC path that doesn't contain a share name.
					// Both the server and shares must be present to form
					// the root of a volume, so this doesn't have a root.
					return {};
				}

				// Skip any redundant slashes. If this is npos,
				// it means the string ends with a separator: "\\server\"
				size_t shareStartPos = path.find_first_not_of( "\\/", serverEndPos + 1 );
				if( shareStartPos == std::string_view::npos )
				{
					return {};
				}

				// Return "\\?\UNC\server\share"
				size_t shareEndPos = path.find_first_of( "\\/", shareStartPos );
				return path.substr( 0, shareEndPos );
			}
		}

		// Return everything up to the trailing slash: "\\?\foo\"
		return path.substr( 0, volumeEndPos + 1 );
	}
	else // Not Windows, assume POSIX semantics
	{
		// POSIX only has a single root directory and it's not named.
		// Absolute paths return "/" as the root path and relative paths
		// return "".
		//
		// The one possible exception is that two leading slashes "//" is
		// resolved in an implementation-defined manner, even though one and
		// three or more are both absolute paths. Given that, it may be
		// more correct to return "//" as the root, but until we encounter
		// an implementation that actually does this, it's hard to predict
		// the best implementation.
		if( !path.empty() )
		{
			char first = path.front();
			if( first == '/' || first == '\\' )
			{
				return path.substr( 0, 1 );
			}
		}

		return {};
	}
}
