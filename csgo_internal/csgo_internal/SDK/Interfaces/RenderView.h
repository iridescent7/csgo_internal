#pragma once
#include "getvfunc.h"
#include "..\Color.h"

class CRenderView
{
public:
	void SetBlend(float blend)
	{
		typedef void(__thiscall* Fn)(void*, float);
		getvfunc<Fn>(this, 4)(this, blend);
	}
	float GetBlend()
	{
		typedef float(__thiscall* Fn)(void*);
		return getvfunc<Fn>(this, 5)(this);
	}
	void SetColorModulation(Color color)
	{
		typedef void(__thiscall* Fn)(void*, float, float, float);
		getvfunc<Fn>(this, 6)(this, color.rgba[0], color.rgba[1], color.rgba[2]);
	}
	void GetColorModulation(float* blend)
	{
		typedef void(__thiscall* Fn)(void*, float*);
		getvfunc<Fn>(this, 7)(this, blend);
	}
};
