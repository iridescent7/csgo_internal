#pragma once
#include <Windows.h>
#include "..\SDK\SDK.h"
#include <cmath>

namespace M
{

	float RandFloat(float min, float max);

	void   FixMovement(CUserCmd* cmd, float new_yaw, float* forwardmove, float* sidemove);
	QAngle CalcAngle(const Vector& src, const Vector& dst);
	float  GetFov(const QAngle& a1, const QAngle& a2);

	void AngleNormalize(QAngle* angle);
	void AngleVectors(const QAngle& angles, Vector* forward);
	void AngleVectors(const QAngle& angles, Vector* forward, Vector* right, Vector* up);

	void VectorAngles(const Vector& forward, QAngle* angles);
	void VectorAngles(const Vector& forward, Vector* up, QAngle* angles);
	void VectorTransform(const Vector& vecIn, const matrix3x4_t& matrix, Vector* vecOut);

	inline void NormalizeYaw(float* yaw)
	{
		float abs_yaw = fabs(*yaw);

		if ( abs_yaw > 180.f )
		{
			int   rotate_count = (int)((abs_yaw + 180.f) / 360.f);
			float rotate_amount = 360.f * rotate_count;

			if (*yaw < 0.f)
				*yaw += rotate_amount;
			else
				*yaw -= rotate_amount;
		}

		Clamp(*yaw, -180.f, 180.f);
	}
}