#pragma once

#include "getvfunc.h"

// todo: just grab idx
// id & 0xFFF
class CBaseHandle
{
public:
	// The low NUM_SERIAL_BITS hold the index. If this value is less than MAX_EDICTS, then the entity is networkable.
	// The high NUM_SERIAL_NUM_BITS bits are the serial number.
	// unsigned long m_Index;

	int index;
	unsigned int serialnum;
};

class C_CSPlayer;

class CClientEntityList
{
public:
	C_CSPlayer* GetClientEntity(int index)
	{
		typedef C_CSPlayer*(__thiscall* Fn)(void*, int);
		return getvfunc<Fn>(this, 3)(this, index);
	}

	C_CSPlayer* GetClientEntityFromHandle(CBaseHandle* handle)
	{
		int idx = handle->index & 0xFFF;

		if (idx < 1)
			return nullptr;

		return GetClientEntity(idx);
	}

	int GetHighestEntityIndex()
	{
		typedef int(__thiscall* Fn)(void*);
		return getvfunc<Fn>(this, 8)(this);
	}
};