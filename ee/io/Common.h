// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

namespace ee
{
	// ftell(), etc are documented to return -1 as their failure values;
	// cast to size_t to silence a signed/unsigned mismatch build warning
	constexpr size_t kInvalidOffset = size_t( -1 );

	enum class FileResult
	{
		kSuccess = 0,
		kNotFound,
		kAlreadyExists,
		kAccessDenied,
		kInvalidArgument,
		kOther
	};

	enum class SeekOrigin
	{
		kFromStart,
		kFromCurrent,
		kFromEnd
	};

} // namespace ee
