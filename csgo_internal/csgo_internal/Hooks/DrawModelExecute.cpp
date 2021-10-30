#include "Hooks.h"
#include "..\SDK\SDK.h"
#include "..\Globals.h"

namespace H
{
	void __fastcall Hooked_DrawModelExecute(void* ecx, void* edx, IMatRenderContext* context, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* pCustomBoneToWorld)
	{
		C::ModelChams.DrawModelExecute(ecx, context, state, info, pCustomBoneToWorld);
	}
}