#include "..\hooks.h"
#include "..\globals.h"

float __fastcall hooks::Hooked_GetViewmodelFov(IClientMode* clientmode)
{
	return hooks::oGetViewmodelFov(clientmode);
}
