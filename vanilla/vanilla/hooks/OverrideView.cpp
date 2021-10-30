#include "..\hooks.h"
#include "..\globals.h"

void __fastcall hooks::Hooked_OverrideView(IClientMode* clientmode, void* edx, CViewSetup* viewsetup)
{
	return hooks::oOverrideView(clientmode, viewsetup);
}
