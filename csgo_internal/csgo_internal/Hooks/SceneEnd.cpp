#include "Hooks.h"
#include "..\SDK\SDK.h"
#include "..\Globals.h"

namespace H
{
	void __fastcall Hooked_SceneEnd(void* thisptr, void* edx)
	{
		C::Glow.SceneEnd();

		oSceneEnd(thisptr);
	}
}