// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

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

		FileStatus() noexcept = default;

		explicit FileStatus( Type type ) noexcept
			: m_Type( type )
		{}

		FileStatus( const FileStatus& ) noexcept = default;
		FileStatus& operator = ( const FileStatus& ) noexcept = default;

		Type GetType( void ) const noexcept
		{
			return m_Type;
		}

		bool Exists( void ) const noexcept
		{
			return m_Type != Type::kNone && m_Type != Type::kNotFound;
		}

		bool IsDirectory( void ) const noexcept
		{
			return m_Type == Type::kDirectory;
		}

		bool IsFile( void ) const noexcept
		{
			return m_Type == Type::kFile;
		}

		bool IsSymlink( void ) const noexcept
		{
			return m_Type == Type::kSymlink;
		}

	private:
		Type m_Type = Type::kNone;

	}; // class FileStatus

} // namespace ee
