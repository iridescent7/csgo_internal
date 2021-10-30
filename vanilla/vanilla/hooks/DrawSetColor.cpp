#include "..\hooks.h"
#include "..\globals.h"

void __fastcall hooks::Hooked_DrawSetColor(ISurface* surface, void* edx, int r, int g, int b, int a)
{
	return hooks::oDrawSetColor(surface, r, g, b ,a);
}
