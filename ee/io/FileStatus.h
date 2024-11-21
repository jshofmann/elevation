// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

#include <string>

namespace ee
{
	class FileStatus
	{
	public:
		enum class Type
		{
			kNone, // Type is unevaluated or an error occured during evaluation
			kFile,
			kDirectory,
			kSymlink,
			kNotFound,
			kUnknown // The file exists but its type could not be determined
		};

		enum Flags : uint8_t
		{
			kExists		= 0x01,
			kReadOnly	= 0x02,
			kHidden		= 0x04
		};

		FileStatus() noexcept = default;
		FileStatus( const FileStatus& ) noexcept = default;
		FileStatus& operator = ( const FileStatus& ) noexcept = default;

		const std::string& GetAbsolutePath( void ) const noexcept
		{
			return mAbsolutePath;
		}

		void SetAbsolutePath( const char* path ) noexcept
		{
			mAbsolutePath = path;
		}

		Type GetType( void ) const noexcept
		{
			return mType;
		}

		bool IsDirectory( void ) const noexcept
		{
			return mType == Type::kDirectory;
		}

		bool IsFile( void ) const noexcept
		{
			return mType == Type::kFile;
		}

		bool IsSymlink( void ) const noexcept
		{
 			return mType == Type::kSymlink;
		}

		size_t GetSize( void ) const noexcept
		{
			return mSize;
		}

		bool Exists( void ) const noexcept
		{
			return ( mFlags & Flags::kExists ) != 0 ? true : false;
		}

		bool IsReadOnly( void ) const noexcept
		{
			return ( mFlags & Flags::kReadOnly ) != 0 ? true : false;
		}

		bool IsHidden( void ) const noexcept
		{
			return ( mFlags & Flags::kHidden ) != 0 ? true : false;
		}

	private:
		std::string	mAbsolutePath;
		Type		mType = Type::kNone;
		size_t		mSize = 0;
		uint8_t		mFlags = 0;

	}; // class FileStatus

} // namespace ee
