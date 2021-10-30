#include "Hooks.h"
#include "..\SDK\SDK.h"
#include "..\Globals.h"

namespace H
{
	void __fastcall Hooked_OverrideView(void* thisptr, void* edx, CViewSetup* pSetup)
	{
		C::VisualExtras.OverrideView(pSetup);

		oOverrideView(thisptr, pSetup);
	}
}