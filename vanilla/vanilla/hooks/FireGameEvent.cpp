#include "..\hooks.h"
#include "..\globals.h"

bool __fastcall hooks::Hooked_FireGameEvent(IGameEventManager2* gameventmanager2, void* edx, IGameEvent* pEvent)
{
	return hooks::oFireGameEvent(gameventmanager2, pEvent);
}
