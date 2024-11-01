// Elevation Engine
//
// Copyright (c) 2024 Azimuth Studios
// All Rights Reserved.
//
// Azimuth Studios, Logan, UT
// http://www.azimuthstudios.com

#pragma once

#define STRINGIFY_NO_EXPANSION( x ) # x
#define STRINGIFY( x ) STRINGIFY_NO_EXPANSION( x )
#define GLUE_2_NO_EXPANSION( first, second ) first ## second
#define GLUE_3_NO_EXPANSION( first, second, third ) first ## second ## third
#define GLUE_2( first, second ) GLUE_2_NO_EXPANSION( first, second )
#define GLUE_3( first, second, third ) GLUE_3_NO_EXPANSION( first, second, third )

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
