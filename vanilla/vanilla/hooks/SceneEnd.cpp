#include "..\hooks.h"
#include "..\globals.h"

void __fastcall hooks::Hooked_SceneEnd(IVRenderView* renderview)
{
	return hooks::oSceneEnd(renderview);
}
