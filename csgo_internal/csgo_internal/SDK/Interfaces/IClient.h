#pragma once
#include "getvfunc.h"

class ClientClass;

class IClient
{
public:
	ClientClass* GetAllClasses(void)
	{
		typedef ClientClass* (__thiscall* OriginalFn)(void*); //Anything inside a VTable is a __thiscall unless it completly disregards the thisptr. You can also call them as __stdcalls, but you won't have access to the __thisptr.
		return getvfunc<OriginalFn>(this, 8)(this); //Return the pointer to the head CClientClass.
	}

	bool DispatchUserMessage(int messageType, int arg, int arg1, void* data)
	{
		return reinterpret_cast<bool(__thiscall*)(void *, int, int, int, void*)>(reinterpret_cast<void***>(this)[0][38])(this, messageType, arg, arg1, data);
	}
};
