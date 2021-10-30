#include "..\hooks.h"
#include "..\globals.h"

void __fastcall hooks::Hooked_FrameStageNotify(IBaseClientDLL* client, void* edx, int stage)
{
	return oFrameStageNotify(client, stage);
}
