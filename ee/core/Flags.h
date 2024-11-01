#pragma once

#include "PlatformTypes.h"

namespace ee
{
	template< class FlagType >
	class TFlags
	{
	public:
		inline TFlags()
			: mFlags( 0 )
		{
		}

		inline TFlags( FlagType f )
			: mFlags( f )
		{
		}

		inline void Set( FlagType f )
		{
			mFlags = f;
		}

		inline void SetFlag( FlagType flag, bool value )
		{
			if( value )
			{
				mFlags |= flag;
			}
			else
			{
				mFlags &= ~flag;
			}
		}

		inline void SetFlag( FlagType flag )
		{
			mFlags |= flag;
		}

		inline void SetFlags( FlagType flagSet )
		{
			mFlags |= flagSet;
		}

		inline void ToggleFlag( FlagType flag )
		{
			mFlags ^= flag;
		}

		inline bool GetFlag( FlagType flag ) const
		{
			return ( ( mFlags & flag ) != 0 );
		}

		inline bool GetFlags( FlagType flag ) const
		{
			return ( ( mFlags & flag ) == flag );
		}

		inline void ClearFlag( FlagType flag )
		{
			mFlags &= ~flag;
		}

		inline void ClearFlags( FlagType flagSet )
		{
			mFlags &= ~flagSet;
		}

		inline bool HasFlag( FlagType flagSet ) const
		{
			return ( ( mFlags & flagSet ) != 0 );
		}

		bool operator == ( TFlags< FlagType > other )
		{
			return other.mFlags == mFlags;
		}

		bool operator != ( TFlags< FlagType > other )
		{
			return other.mFlags != mFlags;
		}

		FlagType mFlags;

	}; // class TFlags

	typedef TFlags< uint64_t >	Flags64;
	typedef TFlags< uint32_t >	Flags32;
	typedef TFlags< uint16_t >	Flags16;
	typedef TFlags< uint8_t >	Flags8;
	typedef Flags32				Flags;

} // namespace ee
