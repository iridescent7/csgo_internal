#pragma once

class RadianEuler;

class Quaternion // same data-layout as engine's vec4_t,
{ //		which is a float[4]
public:
	inline Quaternion(void)
	{}

	inline Quaternion(float ix, float iy, float iz, float iw)
		: x(ix),
		y(iy),
		z(iz),
		w(iw)
	{ }

	//inline Quaternion(RadianEuler const& angle); // evil auto type promotion!!!

	inline void Init(float ix = 0.0f, float iy = 0.0f, float iz = 0.0f, float iw = 0.0f)
	{
		x = ix;
		y = iy;
		z = iz;
		w = iw;
	}

	bool IsValid() const;
	void Invalidate();

	bool operator==(const Quaternion& src) const;
	bool operator!=(const Quaternion& src) const;

	float* Base()
	{
		return (float*)this;
	}

	const float* Base() const
	{
		return (float*)this;
	}

	// array access...
	float operator[](int i) const;
	float& operator[](int i);

	float x, y, z, w;
};
