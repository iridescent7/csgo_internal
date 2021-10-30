#include "..\hooks.h"
#include "..\globals.h"

int __fastcall hooks::Hooked_ListLeavesInBox(void* bsptreequery, void* edx, Vector& mins, Vector& maxs, unsigned short* pList, int listMax)
{
	return hooks::oListLeavesInBox(bsptreequery, mins, maxs, pList, listMax);
}
