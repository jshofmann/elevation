// Elevation Engine
//
// Copyright (c) 2025 Azimuth Studios

#pragma once

#include <cmath>

namespace ee
{
	class vec3
	{
	public:
		// A union of the different ways to address the tuple components
		union
		{
			struct
			{
				float x, y, z;
			};

			// Alternate naming scheme
			struct
			{
				float r, g, b;
			};

			// Alternate naming scheme
			struct
			{
				float u, v, w;
			};

			// Array format
			float array[ 3 ];
		};

		inline vec3() {}
		inline vec3( const float* values ); // values is assumed to have 3 elements
		inline vec3( float x, float y, float z );

		inline vec3& operator = ( const vec3& s );

		// values is assumed to have 3 elements
		inline void Set( const float* values );
		inline void Set( float x, float y, float z );
		inline void Set( const vec3& vec );

		inline float& operator [] ( int index );
		inline const float& operator [] ( int index ) const;

		// scalar operators+++++++++++++++++++++++++++++++++++++++++++++++++++++

		inline vec3 operator + ( float s ) const;
		inline vec3 operator - ( float s ) const;
		inline vec3 operator * ( float s ) const;
		inline vec3 operator / ( float s ) const;

		inline vec3& operator += ( float s );
		inline vec3& operator -= ( float s );
		inline vec3& operator *= ( float s );
		inline vec3& operator /= ( float s );

		// vector operators+++++++++++++++++++++++++++++++++++++++++++++++++++++

		inline vec3 operator + ( const vec3& vec ) const;
		inline vec3 operator - ( const vec3& vec ) const;
		inline vec3 operator * ( const vec3& vec ) const;
		inline vec3 operator / ( const vec3& vec ) const;

		inline vec3& operator += ( const vec3& vec );
		inline vec3& operator -= ( const vec3& vec );
		inline vec3& operator *= ( const vec3& vec );
		inline vec3& operator /= ( const vec3& vec );

		// vector operations++++++++++++++++++++++++++++++++++++++++++++++++++++

		inline vec3& Normalize( void );
		inline vec3 GetNormalized( void ) const;

		inline float Dot( const vec3& vec ) const;
		inline void Cross( const vec3& vec, vec3& vout ) const;

		// Reflects this vector off a plane normal
		inline vec3& Reflect( const vec3& normal );

		inline float Magnitude( void ) const;
		inline float MagnitudeSquared( void ) const;

		inline float Length( void ) const;
		inline float LengthSquared( void ) const;

		inline float DistanceTo( const vec3& vec ) const;
		inline float DistanceToSquared( const vec3& vec ) const;

		// Returns true if the vector is essentially zero
		// (within a custom tolerance you can specify)
		inline bool IsZero( float tolerance = 0.0001f ) const;

	}; // class vec3

//******************************************************************************
//******************************************************************************

	inline vec3::vec3( const float* values )
	{
		// values is assumed to have 3 elements
		x = values[ 0 ]; y = values[ 1 ]; z = values[ 2 ];
	}

	inline vec3::vec3( float _x, float _y, float _z )
	{
		x = _x; y = _y; z = _z;
	}

	inline vec3& vec3::operator = ( const vec3& s )
	{
		x = s.x; y = s.y; z = s.z;
		return *this;
	}

	inline void vec3::Set( const float* values )
	{
		// values is assumed to have 3 elements
		x = values[ 0 ]; y = values[ 1 ]; z = values[ 2 ];
	}

	inline void vec3::Set( float _x, float _y, float _z )
	{
		x = _x; y = _y; z = _z;
	}

	inline void vec3::Set( const vec3& vec )
	{
		x = vec.x; y = vec.y; z = vec.z;
	}

	inline float& vec3::operator [] ( int index )
	{
		return array[ index ];
	}

	inline const float& vec3::operator [] ( int index ) const
	{
		return array[ index ];
	}

// scalar operators+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	inline vec3 vec3::operator + ( float s ) const
	{
		return vec3( x + s, y + s, z + s );
	}

	inline vec3 vec3::operator - ( float s ) const
	{
		return vec3( x - s, y - s, z - s );
	}

	inline vec3 vec3::operator * ( float s ) const
	{
		return vec3( x * s, y * s, z * s );
	}

	inline vec3 vec3::operator / ( float s ) const
	{
		float inv = float( 1 ) / s;
		return vec3( x * inv, y * inv, z * inv );
	}

	inline vec3& vec3::operator += ( float s )
	{
		x += s; y += s; z += s;
		return *this;
	}

	inline vec3& vec3::operator -= ( float s )
	{
		x -= s; y -= s; z -= s;
		return *this;
	}

	inline vec3& vec3::operator *= ( float s )
	{
		x *= s; y *= s; z *= s;
		return *this;
	}

	inline vec3& vec3::operator /= ( float s )
	{
		float inv = float( 1 ) / s;
		x *= inv; y *= inv; z *= inv;
		return *this;
	}

	inline vec3 operator + ( float s, const vec3& vec )
	{
		return vec + s;
	}

	inline vec3 operator - ( float s, const vec3& vec )
	{
		return vec - s;
	}

	inline vec3 operator * ( float s, const vec3& vec )
	{
		return vec * s;
	}

	inline vec3 operator / ( float s, const vec3& vec )
	{
		float inv = float( 1 ) / s;
		return vec * inv;
	}

// vector operators+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	inline vec3 vec3::operator + ( const vec3& vec ) const
	{
		return vec3( x, y, z ) += vec;
	}

	inline vec3 vec3::operator - ( const vec3& vec ) const
	{
		return vec3( x, y, z ) -= vec;
	}

	inline vec3 vec3::operator * ( const vec3& vec ) const
	{
		return vec3( x, y, z ) *= vec;
	}

	inline vec3 vec3::operator / ( const vec3& vec ) const
	{
		return vec3( x, y, z ) /= vec;
	}

	inline vec3& vec3::operator += ( const vec3& vec )
	{
		x += vec.x; y += vec.y; z += vec.z;
		return *this;
	}

	inline vec3& vec3::operator -= ( const vec3& vec )
	{
		x -= vec.x; y -= vec.y; z -= vec.z;
		return *this;
	}

	inline vec3& vec3::operator *= ( const vec3& vec )
	{
		x *= vec.x; y *= vec.y; z *= vec.z;
		return *this;
	}

	inline vec3& vec3::operator /= ( const vec3& vec )
	{
		x /= vec.x; y /= vec.y; z /= vec.z;
		return *this;
	}

	inline vec3& operator + ( vec3& vec )
	{
		return vec;
	}

	inline vec3 operator - ( const vec3& vec )
	{
		return vec3( -vec.x, -vec.y, -vec.z );
	}

	inline bool operator == ( const vec3& v1, const vec3& v2 )
	{
		return ( v1.x == v2.x && v1.y == v2.y && v1.z == v2.z );
	}

	inline bool operator != ( const vec3& v1, const vec3& v2 )
	{
		return ( v1.x != v2.x || v1.y != v2.y || v1.z != v2.z );
	}

	inline bool operator < ( const vec3& v1, const vec3& v2 )
	{
		return ( v1.x < v2.x && v1.y < v2.y && v1.z < v2.z );
	}

	inline bool operator <= ( const vec3& v1, const vec3& v2 )
	{
		return ( v1.x <= v2.x && v1.y <= v2.y && v1.z <= v2.z );
	}

	inline bool operator > ( const vec3& v1, const vec3& v2 )
	{
		return ( v1.x > v2.x && v1.y > v2.y && v1.z > v2.z );
	}

	inline bool operator >= ( const vec3& v1, const vec3& v2 )
	{
		return ( v1.x >= v2.x && v1.y >= v2.y && v1.z >= v2.z );
	}

	inline vec3 Lerp( const vec3& a, const vec3& b, float x )
	{
		return a + x * ( b - a );
	}

	inline vec3 Cross( const vec3& v1, const vec3& v2 )
	{
		vec3 cp;
		v1.Cross( v2, cp );
		return cp;
	}

	inline float Dot( const vec3& v1, const vec3& v2 )
	{
		return v1.Dot( v2 );
	}

// vector operations++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

	inline vec3& vec3::Normalize( void )
	{
		float mag = Magnitude();
		if( mag != float( 0 ) )
		{
			float invMag = float( 1 ) / mag;
			*this *= invMag;
		}

		return *this;
	}

	inline vec3 vec3::GetNormalized( void ) const
	{
		return vec3( x, y, z ).Normalize();
	}

	inline vec3& vec3::Reflect( const vec3& normal )
	{
		// Implements vector reflection where the incident vector
		// I = I - ( 2 * Dot( N, I ) * N )
		float scalar = normal.Dot( *this );

		// so Dot( N, L ) * N creates a vector in the direction of N
		*this -= 2 * scalar * normal;

		return *this;
	}

	inline float vec3::Dot( const vec3& vec ) const
	{
		return ( x * vec.x + y * vec.y + z * vec.z );
	}

	inline void vec3::Cross( const vec3& vec, vec3& vout ) const
	{
		vout.Set( y * vec.z - z * vec.y,
				  z * vec.x - x * vec.z,
				  x * vec.y - y * vec.x );
	}

	inline float vec3::Magnitude( void ) const
	{
		return sqrtf( x * x + y * y + z * z );
	}

	inline float vec3::MagnitudeSquared( void ) const
	{
		return ( x * x + y * y + z * z );
	}

	inline float vec3::Length( void ) const
	{
		return sqrtf( x * x + y * y + z * z );
	}

	inline float vec3::LengthSquared( void ) const
	{
		return ( x * x + y * y + z * z );
	}

	inline float vec3::DistanceTo( const vec3& vec ) const
	{
		vec3 diff = *this - vec;
		return diff.Length();
	}

	inline float vec3::DistanceToSquared( const vec3& vec ) const
	{
		vec3 diff = *this - vec;
		return diff.LengthSquared();
	}

	inline bool vec3::IsZero( float tolerance ) const
	{
		return fabs( x ) <= tolerance && fabs( y ) <= tolerance && fabs( z ) <= tolerance;
	}

} // namespace ee
