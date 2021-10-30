#include "..\hooks.h"
#include "..\globals.h"

void __fastcall hooks::Hooked_LockCursor(ISurface* surface)
{
	if (g_menu.open)
		return surface->UnlockCursor();

	return hooks::oLockCursor(surface);
}
