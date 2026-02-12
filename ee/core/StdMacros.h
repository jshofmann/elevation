// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

#define STRINGIFY_IMPL( x ) # x
#define STRINGIFY( x ) STRINGIFY_IMPL( x )

#define CONCAT_2_IMPL( first, second ) first ## second
#define CONCAT_2( first, second ) CONCAT_2_IMPL( first, second )

#define CONCAT_3_IMPL( first, second, third ) first ## second ## third
#define CONCAT_3( first, second, third ) CONCAT_3_IMPL( first, second, third )

#define MEM_KB( s ) ( uintptr_t( s ) << 10 )
#define MEM_MB( s ) ( MEM_KB( s ) << 10 )

// Note: This works for enum values but not #define tokens; if you feed it
// a #defined token (e.g. DXGI_ERROR_INVALID_CALL) it will perform macro
// substitution first resulting in the string "((HRESULT)0x887A0001L)"
// instead of DXGI_ERROR_INVALID_CALL.
#define TOKEN_TO_STRING( t ) case ( t ): return STRINGIFY( t );

namespace ee
{

template< class T >
void SafeDelete( T** ppT )
{
    if( *ppT )
    {
        delete ( *ppT );
        *ppT = NULL;
    }
}

template< class T >
void SafeDeleteArray( T** ppT )
{
    if( *ppT )
    {
        delete[] ( *ppT );
        *ppT = NULL;
    }
}

template< class T >
void SafeRelease( T** ppT )
{
    if( *ppT )
    {
        ( *ppT )->Release();
        *ppT = NULL;
    }
}

// Return the element count from a static buffer
template< typename T, size_t N >
inline size_t constexpr dimof( T const(&)[ N ] )
{
    return N;
}

} // namespace ee
