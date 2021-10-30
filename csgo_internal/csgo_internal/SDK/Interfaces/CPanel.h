#pragma once

#include "getvfunc.h"

class CPanel
{
public:
	const char* GetName(unsigned int Panel)
	{
		typedef const char*(__thiscall* Fn)(void*, unsigned int);
		return getvfunc<Fn>(this, 36)(this, Panel);
	}
	void SetMouseInputEnabled(unsigned int iPanel, bool bState)
	{
		typedef void(__thiscall *Fn)(void*, int, bool);
		return getvfunc<Fn>(this, 32)(this, iPanel, bState);
	}
};
