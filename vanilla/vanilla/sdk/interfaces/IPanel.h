#pragma once
#include "vfunc.h"

#undef GetClassName

class IPanel
{
public:
	const char* GetName(unsigned int vguiPanel)
	{ 
		using OriginalFn = const char* (__thiscall*)(void*, unsigned int);
		return GetVFunc<OriginalFn>(this, 36)(this, vguiPanel);
	}

	const char* GetClassName(unsigned int vguiPanel)
	{
		using OriginalFn = const char* (__thiscall*)(void*, unsigned int);
		return GetVFunc<OriginalFn>(this, 37)(this, vguiPanel);
	}
};
