// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

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
		static constexpr char Separator = System::GetPlatform() == Platform::kWindows ? '\\' : '/';

		explicit FilePath();
		explicit FilePath( std::string_view path );

		FilePath( const FilePath& ) = default;
		FilePath& operator = ( const FilePath& ) = default;

		FilePath( FilePath&& ) noexcept	= default;
		FilePath& operator = ( FilePath&& ) noexcept = default;

		inline const char* c_str() const noexcept;

		inline operator std::string_view() const noexcept;

		void Clear( void ) noexcept;

		[[nodiscard]] inline bool operator == ( const FilePath& rhs ) const noexcept;
		[[nodiscard]] inline bool operator != ( const FilePath& rhs ) const noexcept;
		[[nodiscard]] inline bool operator < ( const FilePath& rhs ) const noexcept;
		[[nodiscard]] inline bool operator <= ( const FilePath& rhs ) const noexcept;
		[[nodiscard]] inline bool operator > ( const FilePath& rhs ) const noexcept;
		[[nodiscard]] inline bool operator >= ( const FilePath& rhs ) const noexcept;

	private:
		std::string mPath;

	}; // class FilePath

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
