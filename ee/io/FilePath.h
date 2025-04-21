// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

#include <string>
#include <string_view>

#include <ee/core/System.h>

namespace ee
{
	// FilePath is a string containing a filename. It can hold any component
	// of a filename - just a base name, base name + extension, partially
	// qualified path (i.e. directory + basename + extension), or a fully
	// qualified path. You can also use it to parse the components of a
	// path you give it into path, basename, and extension. Note that nothing
	// here will actually access anything in a file system, it's all
	// text manipulation.
	//
	// The components of a path:
	// Root: On POSIX this is '/'; on Windows this is the drive letter or
	//       the start of a UNC path (e.g. "\\server\share").
	// Parent: The part of the path up to but not including
	//         the directory or filename under consideration.
	// Base name: The name of a file minus any extension or parent directory.
	// Filename: The name of a file including any potential extension.
	//           It may be equal to the base name.
	// Extension: If the filename has a period in it ('.') the text after the period.
	//            Note that if the first character of a filename is a period,
	//            then that will not be considered as the start of an extension.
	//            Note also that a single dot '.' as part of a path is not the
	//            start of an extension but denotes the current working directory.
	//
	// All text here is expected to have UTF8 encoding.

	class FilePath
	{
	public:
		// The directory separator used by this platform. This class doesn't
		// care which slashes are used, but certain operations will always use
		// the preferred separator.
		static constexpr char Separator = System::GetPlatform() == Platform::kWindows ? '\\' : '/';

		explicit FilePath( const std::string_view& path );

		FilePath( const FilePath& ) = default;
		FilePath& operator = ( const FilePath& ) = default;

		FilePath( FilePath&& ) noexcept	= default;
		FilePath& operator = ( FilePath&& ) noexcept = default;

		// Clears the stored contents so the path is now empty.
		// Clear() preserves the capacity of the underlying string.
		void Clear( void ) noexcept;

		// ---------------------------------------------------------------------
		// String operations
		// ---------------------------------------------------------------------

		inline const char* c_str( void ) const noexcept;

		inline operator std::string_view( void ) const noexcept;

		inline size_t size( void ) const noexcept; 

		// ---------------------------------------------------------------------
		// Concatenation
		// ---------------------------------------------------------------------

		// Appends new elements to the path with a directory separator in-between.
		//
		// When appending a path that has a root (all absolute paths and some
		// less common relative path formats on Windows), the path is assigned
		// rather than being appended.
		//
		// Common Examples (preferred separator may differ by platform):
		//   ""     + ""   --> ""
		//   ""     + "a"  --> "a"
		//   "a"    + ""   --> "a/"
		//   "a/b"  + "c"  --> "a/b/c"
		//   "a/b/" + "c"  --> "a/b/c"
		//
		// Windows Examples:
		//   "C:\a" + "b"    --> "C:\a\b" (relative path with no root appends)
		//   "C:\a" + "C:\b" --> "C:\b"   (absolute path assigns)
		//   "C:a"  + "C:b"  --> "C:b"    (rooted path assigns)
		//   "a"    + "\b"   --> "\b"     (rooted path assigns)
		//
		// POSIX Examples:
		//   "a"  + "/b" --> "/b" (absolute path assigns)
		//   "/a" + "/b" --> "/b" (absolute path assigns)
		//
		// Note: this behavior differs slightly from std::filesystem
		// for the case of two relative paths with matching roots.
		FilePath& Append( const std::string_view& path );

		// Performs a simple string concatenation
		FilePath& Concat( const std::string_view& path );

		[[nodiscard]] inline bool operator == ( const FilePath& rhs ) const noexcept;
		[[nodiscard]] inline bool operator != ( const FilePath& rhs ) const noexcept;
		[[nodiscard]] inline bool operator < ( const FilePath& rhs ) const noexcept;
		[[nodiscard]] inline bool operator <= ( const FilePath& rhs ) const noexcept;
		[[nodiscard]] inline bool operator > ( const FilePath& rhs ) const noexcept;
		[[nodiscard]] inline bool operator >= ( const FilePath& rhs ) const noexcept;

	private:
		std::string_view GetPathRoot( const std::string_view& path ) const;

		std::string mPath;

	}; // class FilePath

	inline FilePath::FilePath( const std::string_view& path )
	{
		mPath.assign( path );
	}

	inline void FilePath::Clear( void ) noexcept
	{
		mPath.clear();
	}

	inline const char* FilePath::c_str() const noexcept
	{
		return mPath.c_str();
	}

	inline FilePath::operator std::string_view () const noexcept
	{
		return mPath;
	}

	inline size_t FilePath::size( void ) const noexcept
	{
		return mPath.size();
	}

	inline FilePath& FilePath::Concat( const std::string_view& path )
	{
		mPath.append( path );
		return *this;
	}

	bool FilePath::operator == ( const FilePath& rhs ) const noexcept
	{
		return mPath == rhs.mPath;
	}

	bool FilePath::operator != ( const FilePath& rhs ) const noexcept
	{
		return mPath != rhs.mPath;
	}

	bool FilePath::operator < ( const FilePath& rhs ) const noexcept
	{
		return mPath < rhs.mPath;
	}

	bool FilePath::operator <= ( const FilePath& rhs ) const noexcept
	{
		return mPath <= rhs.mPath;
	}

	bool FilePath::operator > ( const FilePath& rhs ) const noexcept
	{
		return mPath > rhs.mPath;
	}

	bool FilePath::operator >= ( const FilePath& rhs ) const noexcept
	{
		return mPath >= rhs.mPath;
	}

} // namespace ee
