#pragma once

#include <math.h>
#include <float.h> // FLT_EPSILON

#define CHECK_VALID( _v ) 0
#define Assert( _exp ) ((void)0)

#define PI 3.14159265358979323846f
#define DEG2RAD( x ) ( ( float )( x ) * ( float )( ( float )( PI ) / 180.0f ) )
#define RAD2DEG( x ) ( ( float )( x ) * ( float )( 180.0f / ( float )( PI ) ) )
#define RADPI 57.295779513082f

class Vector
{
public:
	float x, y, z;
	//Vector(void);
	//Vector(float X, float Y, float Z);

	__forceinline float LengthSqr(void) const
	{
		CHECK_VALID(*this);
		return (this->x*this->x + this->y*this->y + this->z*this->z);
	}
	__forceinline bool IsZero(float tolerance = 0.01f) const
	{
		return (x > -tolerance && x < tolerance &&
			y > -tolerance && y < tolerance &&
			z > -tolerance && z < tolerance);
	}
	//===============================================
	inline void Vector::Init(float ix, float iy, float iz)
	{
		x = ix; y = iy; z = iz;
		CHECK_VALID(*this);
	}
	//===============================================
	inline Vector::Vector(float X, float Y, float Z)
	{
		x = X; y = Y; z = Z;
		CHECK_VALID(*this);
	}
	//===============================================
	inline Vector::Vector(void) {}
	//===============================================
	inline void Vector::Zero()
	{
		x = y = z = 0.0f;
	}
	//===============================================
	inline void Vector::Rotate2D(const float &f)
	{
		float _x, _y;

		float s, c;

		float r = f * PI / 180.0f;
		s = sinf(r);
		c = cosf(r);

		_x = x;
		_y = y;

		x = (_x * c) - (_y * s);
		y = (_x * s) + (_y * c);
	}
	//===============================================
	inline void VectorClear(Vector& a)
	{
		a.x = a.y = a.z = 0.0f;
	}
	//===============================================
	inline Vector& Vector::operator=(const Vector &vOther)
	{
		CHECK_VALID(vOther);
		x = vOther.x; y = vOther.y; z = vOther.z;
		return *this;
	}
	//===============================================
	inline float& Vector::operator[](int i)
	{
		Assert((i >= 0) && (i < 3));
		return ((float*)this)[i];
	}
	//===============================================
	inline float Vector::operator[](int i) const
	{
		Assert((i >= 0) && (i < 3));
		return ((float*)this)[i];
	}
	//===============================================
	inline bool Vector::operator==(const Vector& src) const
	{
		CHECK_VALID(src);
		CHECK_VALID(*this);
		return (src.x == x) && (src.y == y) && (src.z == z);
	}
	//===============================================
	inline bool Vector::operator!=(const Vector& src) const
	{
		CHECK_VALID(src);
		CHECK_VALID(*this);
		return (src.x != x) || (src.y != y) || (src.z != z);
	}
	//===============================================
	__forceinline void VectorCopy(const Vector& src, Vector& dst)
	{
		CHECK_VALID(src);
		dst.x = src.x;
		dst.y = src.y;
		dst.z = src.z;
	}
	//===============================================
	__forceinline  Vector& Vector::operator+=(const Vector& v)
	{
		CHECK_VALID(*this);
		CHECK_VALID(v);
		x += v.x; y += v.y; z += v.z;
		return *this;
	}
	//===============================================
	__forceinline  Vector& Vector::operator-=(const Vector& v)
	{
		CHECK_VALID(*this);
		CHECK_VALID(v);
		x -= v.x; y -= v.y; z -= v.z;
		return *this;
	}
	//===============================================
	__forceinline  Vector& Vector::operator*=(float fl)
	{
		x *= fl;
		y *= fl;
		z *= fl;
		CHECK_VALID(*this);
		return *this;
	}
	//===============================================
	__forceinline  Vector& Vector::operator*=(const Vector& v)
	{
		CHECK_VALID(v);
		x *= v.x;
		y *= v.y;
		z *= v.z;
		CHECK_VALID(*this);
		return *this;
	}
	//===============================================
	__forceinline Vector&	Vector::operator+=(float fl)
	{
		x += fl;
		y += fl;
		z += fl;
		CHECK_VALID(*this);
		return *this;
	}
	//===============================================
	__forceinline Vector&	Vector::operator-=(float fl)
	{
		x -= fl;
		y -= fl;
		z -= fl;
		CHECK_VALID(*this);
		return *this;
	}
	//===============================================
	__forceinline  Vector& Vector::operator/=(float fl)
	{
		Assert(fl != 0.0f);
		float oofl = 1.0f / fl;
		x *= oofl;
		y *= oofl;
		z *= oofl;
		CHECK_VALID(*this);
		return *this;
	}
	//===============================================
	__forceinline  Vector& Vector::operator/=(const Vector& v)
	{
		CHECK_VALID(v);
		Assert(v.x != 0.0f && v.y != 0.0f && v.z != 0.0f);
		x /= v.x;
		y /= v.y;
		z /= v.z;
		CHECK_VALID(*this);
		return *this;
	}
	//===============================================
	inline float Vector::Length(void) const
	{
		CHECK_VALID(*this);

		float root = 0.0f;

		float sqsr = x * x + y * y + z * z;

		__asm sqrtss xmm0, sqsr
		__asm movss root, xmm0

		return root;
	}
	//===============================================
	inline float Vector::Length2D(void) const
	{
		CHECK_VALID(*this);

		float root = 0.0f;

		float sqst = x * x + y * y;

		__asm
		{
			sqrtss xmm0, sqst
			movss root, xmm0
		}

		return root;
	}
	//===============================================
	inline float Vector::Length2DSqr(void) const
	{
		return (x*x + y*y);
	}
	//===============================================
	inline Vector Cross(const Vector& v)
	{
		return Vector(y*v.z - z*v.y, z*v.x - x*v.z, x*v.y - y*v.x);
	}
	//===============================================
	float Vector::DistToSqr(const Vector &vOther) const
	{
		Vector delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;

		return delta.LengthSqr();
	}

	float Vector::DistTo(const Vector &vOther) const
	{
		Vector delta;

		delta.x = x - vOther.x;
		delta.y = y - vOther.y;
		delta.z = z - vOther.z;

		return delta.Length();
	}

	//===============================================
	inline float Vector::NormalizeInPlace()
	{
		float length = this->Length();
		float radius = 1.0f / (length + FLT_EPSILON);

		this->x *= radius;
		this->y *= radius;
		this->z *= radius;

		return length;
	}
	//===============================================
	Vector Normalized() const
	{
		Vector vec = *this;
		float length = vec.Length();

		if (length != 0.0f)
		{
			vec.x /= length;
			vec.y /= length;
			vec.z /= length;
		}
		else
		{
			vec.x = vec.y = vec.z = 0.0f;
		}
		return vec;
	}
	//===============================================
	inline Vector Vector::operator+(const Vector& v) const
	{
		Vector res;
		res.x = x + v.x;
		res.y = y + v.y;
		res.z = z + v.z;
		return res;
	}

	//===============================================
	inline Vector Vector::operator-(const Vector& v) const
	{
		Vector res;
		res.x = x - v.x;
		res.y = y - v.y;
		res.z = z - v.z;
		return res;
	}
	//===============================================
	inline Vector Vector::operator*(float fl) const
	{
		Vector res;
		res.x = x * fl;
		res.y = y * fl;
		res.z = z * fl;
		return res;
	}
	//===============================================
	inline Vector Vector::operator*(const Vector& v) const
	{
		Vector res;
		res.x = x * v.x;
		res.y = y * v.y;
		res.z = z * v.z;
		return res;
	}
	//===============================================
	inline Vector Vector::operator/(float fl) const
	{
		Vector res;
		res.x = x / fl;
		res.y = y / fl;
		res.z = z / fl;
		return res;
	}
	//===============================================
	inline Vector Vector::operator/(const Vector& v) const
	{
		Vector res;
		res.x = x / v.x;
		res.y = y / v.y;
		res.z = z / v.z;
		return res;
	}
	inline float Vector::Dot(const Vector& vOther) const
	{
		return (x*vOther.x + y*vOther.y + z*vOther.z);
	}
	inline float Vector::Dot(const float* fOther) const
	{
		return (x * fOther[0] + y * fOther[1] + z * fOther[2]);
	}
};
