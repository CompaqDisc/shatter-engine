// Copyright � 2017, Christiaan Bakker, All rights reserved.
#pragma once

#include <glm/glm.hpp>

/*
namespace SIMDMath
{
	typedef __m128 vec3;
	typedef __m128 vec4;

	inline vec3 ToVec3( glm::vec3 vec )
	{
		return _mm_setr_ps( vec.x, vec.y, vec.z, 0.0f );
	}

	inline vec4 ToVec4( glm::vec4 vec )
	{
		return _mm_setr_ps( vec.x, vec.y, vec.z, vec.w );
	}

	inline vec3 Subtract( vec3 p, vec3 v )
	{
		return _mm_sub_ps( p, v );
	}

	inline float Length( vec3 v )
	{
		return _mm_cvtss_f32( _mm_sqrt_ss( _mm_dp_ps( v, v, 0x71 ) ) );
	}

	inline float Distance( vec3 p, vec3 v )
	{
		return Length( Subtract( p, v ) );
	}

	inline vec3 Dot( vec3 v, vec3 w )
	{
		return _mm_dp_ps( v, w, 0x71 );
	}
}*/

class Vector2D
{
public:
	union { float X, R, S; };
	union { float Y, G, T; };

	Vector2D() = default;
	Vector2D( float X, float Y )
	{
		this->X = X;
		this->Y = Y;
	}

	inline float* Base()
	{
		return &X;
	}

	inline float& operator[]( size_t Index )
	{
		return ( &X )[Index];
	}

	inline const float& operator[]( size_t Index ) const
	{
		return ( &X )[Index];
	}

	inline Vector2D operator=( const Vector2D& Vector )
	{
		this->X = Vector.X;
		this->Y = Vector.Y;
		return *this;
	}

	inline Vector2D operator+( const Vector2D& Vector ) const
	{
		return Vector2D(
			X + Vector.X,
			Y + Vector.Y
		);
	};

	inline Vector2D operator-( const Vector2D& Vector ) const
	{
		return Vector2D(
			X - Vector.X,
			Y - Vector.Y
		);
	};

	inline Vector2D operator*( const Vector2D& Vector ) const
	{
		return Vector2D(
			X * Vector.X,
			Y * Vector.Y
		);
	};

	inline Vector2D operator+( const float& Scalar ) const
	{
		return Vector2D(
			X + Scalar,
			Y + Scalar
		);
	};

	inline Vector2D operator-( const float& Scalar ) const
	{
		return Vector2D(
			X - Scalar,
			Y - Scalar
		);
	};

	inline Vector2D operator*( const float& Scalar ) const
	{
		return Vector2D(
			X * Scalar,
			Y * Scalar
		);
	};

	inline Vector2D operator+=( const Vector2D& Vector )
	{
		X += Vector.X;
		Y += Vector.Y;
		return *this;
	};

	inline Vector2D operator-=( const Vector2D& Vector )
	{
		X -= Vector.X;
		Y -= Vector.Y;
		return *this;
	};

	inline Vector2D operator*=( const Vector2D& Vector )
	{
		X *= Vector.X;
		Y *= Vector.Y;
		return *this;
	};

	inline Vector2D operator+=( const float& Scalar )
	{
		X += Scalar;
		Y += Scalar;
		return *this;
	};

	inline Vector2D operator-=( const float& Scalar )
	{
		X -= Scalar;
		Y -= Scalar;
		return *this;
	};

	inline Vector2D operator*=( const float& Scalar )
	{
		X *= Scalar;
		Y *= Scalar;
		return *this;
	};

	inline float Dot( const Vector2D& Vector ) const
	{
		return X * Vector.X + Y * Vector.Y;
	}
};

class Vector3D
{
public:
	union { float X, R, S; };
	union { float Y, G, T; };
	union { float Z, B, P; };

	Vector3D() = default;
	Vector3D( float X, float Y, float Z )
	{
		this->X = X;
		this->Y = Y;
		this->Z = Z;
	}

	inline const float* Base() const
	{
		return &X;
	}

	inline float& operator[]( size_t Index )
	{
		return ( &X )[Index];
	}

	inline const float& operator[]( size_t Index ) const
	{
		return ( &X )[Index];
	}

	inline Vector3D operator=( const Vector3D& Vector )
	{
		this->X = Vector.X;
		this->Y = Vector.Y;
		this->Z = Vector.Z;
		return *this;
	}

	inline Vector3D operator+( const Vector3D& Vector ) const
	{
		return Vector3D(
			X + Vector.X,
			Y + Vector.Y,
			Z + Vector.Z
		);
	};

	inline Vector3D operator-( const Vector3D& Vector ) const
	{
		return Vector3D(
			X - Vector.X,
			Y - Vector.Y,
			Z - Vector.Z
		);
	};

	inline Vector3D operator*( const Vector3D& Vector ) const
	{
		return Vector3D(
			X * Vector.X,
			Y * Vector.Y,
			Z * Vector.Z
		);
	};

	inline Vector3D operator/( const Vector3D& Vector ) const
	{
		return Vector3D(
			X / Vector.X,
			Y / Vector.Y,
			Z / Vector.Z
		);
	};

	inline Vector3D operator+( const float& Scalar ) const
	{
		return Vector3D(
			X + Scalar,
			Y + Scalar,
			Z + Scalar
		);
	};

	inline Vector3D operator-( const float& Scalar ) const
	{
		return Vector3D( 
			X - Scalar, 
			Y - Scalar, 
			Z - Scalar
		);
	};

	inline Vector3D operator*( const float& Scalar ) const
	{
		return Vector3D(
			X * Scalar,
			Y * Scalar,
			Z * Scalar
		);
	};

	friend inline Vector3D operator*( const float& Scalar, const Vector3D& Vector )
	{
		return Vector * Scalar;
	};

	inline Vector3D operator/( const float& Scalar ) const
	{
		return Vector3D(
			X / Scalar,
			Y / Scalar,
			Z / Scalar
		);
	};

	friend inline Vector3D operator/( const float& Scalar, const Vector3D& Vector )
	{
		return Vector3D(
			Scalar / Vector.X,
			Scalar / Vector.Y,
			Scalar / Vector.Z
		);
	};

	inline Vector3D operator+=( const Vector3D& Vector )
	{
		X += Vector.X;
		Y += Vector.Y;
		Z += Vector.Z;
		return *this;
	};

	inline Vector3D operator-=( const Vector3D& Vector )
	{
		X -= Vector.X;
		Y -= Vector.Y;
		Z -= Vector.Z;
		return *this;
	};

	inline Vector3D operator*=( const Vector3D& Vector )
	{
		X *= Vector.X;
		Y *= Vector.Y;
		Z *= Vector.Z;
		return *this;
	};

	inline Vector3D operator/=( const Vector3D& Vector )
	{
		X /= Vector.X;
		Y /= Vector.Y;
		Z /= Vector.Z;
		return *this;
	};

	inline Vector3D operator+=( const float& Scalar )
	{
		X += Scalar;
		Y += Scalar;
		Z += Scalar;
		return *this;
	};

	inline Vector3D operator-=( const float& Scalar )
	{
		X -= Scalar;
		Y -= Scalar;
		Z -= Scalar;
		return *this;
	};

	inline Vector3D operator*=( const float& Scalar )
	{
		X *= Scalar;
		Y *= Scalar;
		Z *= Scalar;
		return *this;
	};

	inline Vector3D operator/=( const float& Scalar )
	{
		X /= Scalar;
		Y /= Scalar;
		Z /= Scalar;
		return *this;
	};

	friend inline Vector3D operator-( const Vector3D& Vector )
	{
		Vector3D Result;
		Result.X = -Vector.X;
		Result.Y = -Vector.Y;
		Result.Z = -Vector.Z;
		return Result;
	};

	inline float Dot( const Vector3D& Vector ) const
	{
		return X * Vector.X + Y * Vector.Y + Z * Vector.Z;
	}

	inline Vector3D Cross( const Vector3D& Vector ) const
	{
		return Vector3D(
			Y * Vector.Z - Vector.Y * Z,
			Z * Vector.X - Vector.Z * X,
			X * Vector.Y - Vector.X * Y
		);
	}

	inline float Length() const
	{
		return sqrt( Dot( *this ) );
	}

	inline float LengthSquared() const
	{
		return Dot( *this );
	}

	inline float Length( const Vector3D& Vector ) const
	{
		return sqrt( Vector.Dot( Vector ) );
	}

	inline float Distance( const Vector3D& Vector ) const
	{
		return Length( *this - Vector );
	}

	inline float DistanceSquared( const Vector3D& Vector ) const
	{
		const Vector3D Delta = *this - Vector;
		return Delta.Dot( Delta );
	}

	inline Vector3D Normalized() const
	{
		Vector3D Unit = *this;

		const float VectorLength = Length( Unit );
		if( VectorLength > 0.0f )
		{
			Unit /= VectorLength;
		}

		return Unit;
	}

	inline float Normalize()
	{
		const float VectorLength = Length( *this );
		if( VectorLength > 0.0f )
		{
			*this /= VectorLength;
		}

		return VectorLength;
	}
};

class Vector4D
{
public:
	union { float X, R, S; };
	union { float Y, G, T; };
	union { float Z, B, P; };
	union { float W, A, Q; };

	Vector4D() = default;
	Vector4D( float X, float Y, float Z, float W )
	{
		this->X = X;
		this->Y = Y;
		this->Z = Z;
		this->W = W;
	}

	inline const float* Base() const
	{
		return &X;
	}

	inline float& operator[]( size_t Index )
	{
		return ( &X )[Index];
	}

	inline const float& operator[]( size_t Index ) const
	{
		return ( &X )[Index];
	}

	inline Vector4D operator=( const Vector4D& Vector )
	{
		this->X = Vector.X;
		this->Y = Vector.Y;
		this->Z = Vector.Z;
		return *this;
	}

	inline Vector4D operator+( const Vector4D& Vector ) const
	{
		return Vector4D(
			X + Vector.X,
			Y + Vector.Y,
			Z + Vector.Z,
			W + Vector.W
		);
	};

	inline Vector4D operator-( const Vector4D& Vector ) const
	{
		return Vector4D(
			X - Vector.X,
			Y - Vector.Y,
			Z - Vector.Z,
			W - Vector.W
		);
	};

	inline Vector4D operator*( const Vector4D& Vector ) const
	{
		return Vector4D(
			X * Vector.X,
			Y * Vector.Y,
			Z * Vector.Z,
			W * Vector.W
		);
	};

	inline Vector4D operator+( const float& Scalar ) const
	{
		return Vector4D(
			X + Scalar,
			Y + Scalar,
			Z + Scalar,
			W + Scalar
		);
	};

	inline Vector4D operator-( const float& Scalar ) const
	{
		return Vector4D(
			X - Scalar,
			Y - Scalar,
			Z - Scalar,
			W - Scalar
		);
	};

	inline Vector4D operator*( const float& Scalar ) const
	{
		return Vector4D(
			X * Scalar,
			Y * Scalar,
			Z * Scalar,
			W * Scalar
		);
	};

	inline Vector4D operator+=( const Vector4D& Vector )
	{
		X += Vector.X;
		Y += Vector.Y;
		Z += Vector.Z;
		W += Vector.W;
		return *this;
	};

	inline Vector4D operator-=( const Vector4D& Vector )
	{
		X -= Vector.X;
		Y -= Vector.Y;
		Z -= Vector.Z;
		W -= Vector.W;
		return *this;
	};

	inline Vector4D operator*=( const Vector4D& Vector )
	{
		X *= Vector.X;
		Y *= Vector.Y;
		Z *= Vector.Z;
		W *= Vector.W;
		return *this;
	};

	inline Vector4D operator+=( const float& Scalar )
	{
		X += Scalar;
		Y += Scalar;
		Z += Scalar;
		W += Scalar;
		return *this;
	};

	inline Vector4D operator-=( const float& Scalar )
	{
		X -= Scalar;
		Y -= Scalar;
		Z -= Scalar;
		W -= Scalar;
		return *this;
	};

	inline Vector4D operator*=( const float& Scalar )
	{
		X *= Scalar;
		Y *= Scalar;
		Z *= Scalar;
		W *= Scalar;
		return *this;
	};

	inline float Dot( const Vector4D& Vector ) const
	{
		return X * Vector.X + Y * Vector.Y + Z * Vector.Z + W * Vector.W;
	}

	inline float Length()
	{
		return sqrt( Dot( *this ) );
	}

	inline float Length( const Vector4D& Vector )
	{
		return sqrt( Vector.Dot( Vector ) );
	}

	inline float Distance( const Vector4D& Vector )
	{
		return Length( *this - Vector );
	}

	inline Vector4D Normalized()
	{
		const float LengthBiased = 1.f / ( Length( *this ) + FLT_EPSILON );
		*this *= LengthBiased;

		return *this;
	}

	inline float Normalize()
	{
		const float LengthBiased = 1.f / ( Length( *this ) + FLT_EPSILON );
		*this *= LengthBiased;

		return LengthBiased;
	}

	inline float Normalize( const Vector4D& Vector, const float& Length )
	{
		const float LengthBiased = 1.f / ( Length + FLT_EPSILON );
		*this *= LengthBiased;

		return LengthBiased;
	}
};

/*class SIMDVector4D
{
public:
	SIMDVector4D() = default;

	SIMDVector4D( const Vector3D& Vector )
	{
		AssignVector( Vector );
	}

	SIMDVector4D( const glm::vec4& Vector )
	{
		AssignVector( Vector );
	}

	inline void AssignVector( const Vector3D& Vector )
	{
		StoredVector = _mm_setr_ps( Vector.X, Vector.Y, Vector.Z, 0.0f );
	}

	inline void AssignVector( const glm::vec4& Vector )
	{
		StoredVector = _mm_setr_ps( Vector.x, Vector.y, Vector.z, Vector.w );
	}

	inline void operator=( const Vector3D& Vector )
	{
		AssignVector( Vector );
	};

	inline void operator=( const glm::vec4& Vector )
	{
		AssignVector( Vector );
	};

	inline glm::vec4 ToVec4() const
	{
		float RawVector[4];
		_mm_storeu_ps( RawVector, StoredVector );
		return glm::vec4( RawVector[0], RawVector[1], RawVector[2], RawVector[3] );
	}

	inline float Length()
	{
		return Length( StoredVector );
	}

	inline __m128 Dot( const __m128& Vector )
	{
		return _mm_dp_ps( StoredVector, Vector, 0x71 );
	}

	inline float Distance( const __m128& Vector )
	{
		return Length( Subtract( StoredVector, Vector ) );
	}

	inline __m128 operator+=( const __m128& Vector )
	{
		StoredVector = Add( StoredVector, Vector );
		return StoredVector;
	};

	inline __m128 operator-=( const __m128& Vector )
	{
		StoredVector = Subtract( StoredVector, Vector );
		return StoredVector;
	};

	inline __m128 operator*=( const __m128& Vector )
	{
		StoredVector = Multiply( StoredVector, Vector );
		return StoredVector;
	};

	// Static functions
	static inline __m128 Add( const __m128& A, const __m128& B )
	{
		return _mm_add_ps( A, B );
	}

	static inline __m128 Subtract( const __m128& A, const __m128& B )
	{
		return _mm_sub_ps( A, B );
	}

	static inline __m128 Multiply( const __m128& A, const __m128& B )
	{
		return _mm_mul_ps( A, B );
	}

	static inline float Length( const __m128& Vector )
	{
		return _mm_cvtss_f32( _mm_sqrt_ss( _mm_dp_ps( Vector, Vector, 0x71 ) ) );
	}

private:
	__m128 StoredVector;
};
*/