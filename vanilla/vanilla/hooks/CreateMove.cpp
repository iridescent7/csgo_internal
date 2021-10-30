#include "..\hooks.h"
#include "..\globals.h"

bool __fastcall hooks::Hooked_CreateMove(IClientMode* clientmode, void* edx, float sample_input_frametime, CUserCmd* cmd)
{
	return hooks::oCreateMove(clientmode, sample_input_frametime, cmd);
}
