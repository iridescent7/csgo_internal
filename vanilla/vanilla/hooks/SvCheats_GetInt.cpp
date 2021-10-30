#include "..\hooks.h"
#include "..\globals.h"

bool __fastcall hooks::Hooked_SvCheats_GetInt(ConVar* sv_cheats)
{
	return hooks::oSvCheats_GetInt(sv_cheats);
}
