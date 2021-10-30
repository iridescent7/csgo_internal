#include "Hooks.h"
#include "..\SDK\SDK.h"

namespace H
{
	bool __fastcall Hooked_FireGameEvent(void* thisptr, void* edx, IGameEvent* pEvent)
	{
		if (!pEvent)
			return oFireGameEvent(thisptr, pEvent);

		return oFireGameEvent(thisptr, pEvent);
	}
}