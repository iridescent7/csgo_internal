#include "..\hooks.h"
#include "..\globals.h"

void __fastcall hooks::Hooked_DrawModelExecute(IVModelRender* modelrender, void* edx, IMatRenderContext* context, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* pCustomBoneToWorld)
{
	return hooks::oDrawModelExecute(modelrender, context, state, info, pCustomBoneToWorld);
}
