#pragma once
#include "..\..\utils.h"

class CHudElement;
class CHud 
{
public:
	CHudElement* CHud::FindElement(const char* name)
	{
		using  FindElementFn              = CHudElement*(__thiscall*)(CHud* hud, const char* name);
		static FindElementFn oFindElement = (FindElementFn)utils::ModulePatternScan(utils::GetModuleEntry(hash("client_panorama.dll")), charenc("55 8B EC 53 8B 5D 08 56 57 8B F9 33 F6 39"));

		return oFindElement(this, name);
	}
};
