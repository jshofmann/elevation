// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

namespace ee
{
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
