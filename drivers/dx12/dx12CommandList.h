// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

#include <ee/graphics/Enums.h>
#include <ee/graphics/CommandList.h>

namespace ee
{
	class dx12CommandList : public CommandList
	{
	public:
		dx12CommandList() = delete;
		dx12CommandList( CommandListType type );
		virtual ~dx12CommandList();

		bool Initialize( Device* device );
		void Release( void );

		CommandListType GetType( void ) const
		{
			return mType;
		}

		ID3D12CommandList* GetD3D12CommandList( void ) const
		{
			return mCommandList;
		}

	private:
		CommandListType mType = CommandListType::kGraphics;
		ID3D12CommandList* mCommandList = nullptr;

	}; // class dx12CommandList

} // namespace ee
