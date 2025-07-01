// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios

#pragma once

#define STRINGIFY_IMPL( x ) # x
#define STRINGIFY( x ) STRINGIFY_IMPL( x )

#define CONCAT_2_IMPL( first, second ) first ## second
#define CONCAT_2( first, second ) CONCAT_2_NO_EXPANSION( first, second )

#define CONCAT_3_IMPL( first, second, third ) first ## second ## third
#define CONCAT_3( first, second, third ) CONCAT_3_NO_EXPANSION( first, second, third )

#define MEM_KB( s ) ( uintptr_t( s ) << 10 )
#define MEM_MB( s ) ( MEM_KB( s ) << 10 )

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
#define dimof( x ) ( sizeof( x ) / sizeof( x[0] ) )

} // namespace ee
