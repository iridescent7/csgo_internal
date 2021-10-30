#pragma once

#include "QAngle.h"
#include "Quaternion.h"
#include "RadianEuler.h"
#include "Vector.h"
#include "Vector2D.h"
#include "Vector4D.h"
#include "VMatrix.h"

#define PI 3.14159265358979323846f
#define DEG2RAD( x ) ( ( float )( x ) * ( float )( ( float )( PI ) / 180.0f ) )
#define RAD2DEG( x ) ( ( float )( x ) * ( float )( 180.0f / ( float )( PI ) ) )
#define RADPI 57.295779513082f

namespace math
{
	void AngleVectors(const QAngle& angles, Vector* forward);
	void AngleVectors(const QAngle& angles, Vector* forward, Vector* right, Vector* up);

	void VectorAngles(const Vector& forward, QAngle* angles);
	void VectorAngles(const Vector& forward, Vector* up, QAngle* angles);

	void AngleNormalize(QAngle* angle);
	void VectorTransform(const Vector& vecIn, const matrix3x4_t& matrix, Vector* vecOut);

	float RandFloat(float min, float max);

	inline void SinCos(const float& radian, float* sine, float* cosine)
	{
		*sine   = sin(radian);
		*cosine = cos(radian);
	}
}