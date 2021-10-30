#pragma once

class QAngle;
class Quaternion;

class RadianEuler
{
public:
	inline RadianEuler(void)
	{ }

	inline RadianEuler(float X, float Y, float Z)
	{
		x = X;
		y = Y;
		z = Z;
	}

	//inline RadianEuler(Quaternion const& q); // evil auto type promotion!!!
	//inline RadianEuler(QAngle const& angles); // evil auto type promotion!!!
	
	// Initialization
	inline void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f)
	{
		x = ix;
		y = iy;
		z = iz;
	}

	//	conversion to qangle
	QAngle ToQAngle(void) const;
	bool IsValid() const;
	void Invalidate();

	inline float* Base()
	{
		return &x;
	}

	inline const float* Base() const
	{
		return &x;
	}

	// array access...
	float operator[](int i) const;
	float& operator[](int i);

	float x, y, z;
};