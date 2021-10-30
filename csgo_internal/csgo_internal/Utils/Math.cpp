#include "Math.h"
#include <cstdlib>

namespace M
{
	inline void SinCos(const float& radian, float* sine, float* cosine)
	{
		*sine = sin(radian);
		*cosine = cos(radian);
	}

	float RandFloat(float min, float max)
	{
		return (min + (float)(rand()) / ((float)(RAND_MAX / (max - min))));
	}

	// todo: cleanup
	void FixMovement(CUserCmd* cmd, float new_yaw, float* forwardmove, float* sidemove)
	{
		if (cmd->forwardmove == 0.f && cmd->sidemove == 0.f)
		{
			*forwardmove = 0.f;
			*sidemove    = 0.f;
			return;
		}

		float deltaView;
		float f1;
		float f2;

		if (cmd->viewangles.yaw < 0.f)
			f1 = 360.f + cmd->viewangles.yaw;
		else
			f1 = cmd->viewangles.yaw;

		if (new_yaw < 0.f)
			f2 = 360.f + new_yaw;
		else
			f2 = new_yaw;

		if (f2 < f1)
			deltaView = fabs(f2 - f1);
		else
			deltaView = 360.f - fabs(f1 - f2);

		deltaView = 360.f - deltaView;

		float forward = cos(DEG2RAD(deltaView)) * cmd->forwardmove + cos(DEG2RAD(deltaView + 90.f)) * cmd->sidemove;
		float side    = sin(DEG2RAD(deltaView)) * cmd->forwardmove + sin(DEG2RAD(deltaView + 90.f)) * cmd->sidemove;

		float ratio = 450.f / max(fabs(forward), fabs(side));

		forward *= ratio;
		side    *= ratio;

		Clamp(forward, -450.f, 450.f);
		Clamp(side,    -450.f, 450.f);

		*forwardmove = forward;
		*sidemove    = side;
	}

	QAngle CalcAngle(const Vector& src, const Vector& dst)
	{
		Vector delta = src - dst;
		delta.NormalizeInPlace();

		QAngle angle;
		VectorAngles(delta, &angle);

		return angle;
	}

	float GetFov(const QAngle& a1, const QAngle& a2)
	{
		QAngle delta = a2 - a1;
		AngleNormalize(&delta);

		return sqrt(pow(delta.pitch, 2) + pow(delta.yaw, 2));
	}

	void AngleNormalize(QAngle* angle)
	{
		if (angle->pitch > 89.f)
			angle->pitch = 89.f;

		else if (angle->pitch < -89.f)
			angle->pitch = -89.f;

		float abs_yaw = fabs(angle->yaw);

		if (abs_yaw > 180.f)
		{
			int   rotate_count  = (int)((abs_yaw + 180.f) / 360.f);
			float rotate_amount = 360.f * rotate_count;

			if (angle->yaw < 0.f)
				angle->yaw += rotate_amount;
			else
				angle->yaw -= rotate_amount;
		}

		Clamp(angle->yaw, -180.f, 180.f);

		angle->roll = 0.f;
	}

	void AngleVectors(const QAngle& angles, Vector* forward)
	{
		float sp, sy, cp, cy;

		SinCos(DEG2RAD(angles[1]), &sy, &cy);
		SinCos(DEG2RAD(angles[0]), &sp, &cp);

		forward->x = cp * cy;
		forward->y = cp * sy;
		forward->z = -sp;
	}

	void AngleVectors(const QAngle& angles, Vector* forward, Vector* right, Vector* up)
	{
		float sr, sp, sy, cr, cp, cy;

		SinCos(DEG2RAD(angles[0]), &sp, &cp);
		SinCos(DEG2RAD(angles[1]), &sy, &cy);
		SinCos(DEG2RAD(angles[2]), &sr, &cr);

		if (forward)
		{
			forward->x = cp*cy;
			forward->y = cp*sy;
			forward->z = -sp;
		}

		if (right)
		{
			right->x = (-1 * sr*sp*cy + -1 * cr*-sy);
			right->y = (-1 * sr*sp*sy + -1 * cr*cy);
			right->z = -1 * sr*cp;
		}

		if (up)
		{
			up->x = (cr*sp*cy + -sr*-sy);
			up->y = (cr*sp*sy + -sr*cy);
			up->z = cr*cp;
		}
	}

	void VectorAngles(const Vector& forward, QAngle* angles)
	{
		if (forward.y == 0.0f && forward.x == 0.0f)
		{
			angles->pitch = (forward[2] > 0.0f) ? 270.0f : 90.0f;
			angles->yaw = 0.0f;
		}
		else
		{
			angles->pitch = atan2(-forward.z, forward.Length2D()) * -180.0f / PI;
			angles->yaw = atan2(forward.y, forward.x) * 180.0f / PI;

			if (angles->yaw > 90.0f)
				angles->yaw -= 180;

			else if (angles->yaw < 90.0f)
				angles->yaw += 180.0f;

			else if (angles->yaw == 90.0f)
				angles->yaw = 0;
		}

		angles->roll = 0.0f;
	}

	void VectorAngles(const Vector& forward, Vector* up, QAngle* angles)
	{
		Vector left = up->Cross(forward);
		left.NormalizeInPlace();

		float forward_dist = forward.Length2D();

		if (forward_dist > 0.001f)
		{
			angles->pitch = atan2f(-forward.z, forward_dist) * 180.f / PI;
			angles->yaw = atan2f(forward.y, forward.x) * 180.f / PI;

			float upZ = (left.y * forward.x) - (left.x * forward.y);
			angles->roll = atan2f(left.z, upZ) * 180.f / PI;
		}
		else
		{
			angles->pitch = atan2f(-forward.z, forward_dist) * 180.f / PI;
			angles->yaw = atan2f(-left.x, left.y) * 180.f / PI;
			angles->roll = 0.f;
		}
	}

	void VectorTransform(const Vector& vecIn, const matrix3x4_t& matrix, Vector* vecOut)
	{
		vecOut->x = vecIn.Dot(matrix[0]) + matrix[0][3];
		vecOut->y = vecIn.Dot(matrix[1]) + matrix[1][3];
		vecOut->z = vecIn.Dot(matrix[2]) + matrix[2][3];
	}
}
