#include "Hooks.h"
#include "..\Globals.h"

namespace H
{
	LRESULT __stdcall Hooked_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if(D3DInit)
			G::Menu.WndProcCallbk(hWnd, uMsg, wParam, lParam);

		bool ingame = I::Engine->IsInGame();

		if (!ingame && G::x88Mode)
			G::x88Mode = false;

		if (I::Engine && ingame)
		{
			if (uMsg == WM_KEYDOWN && wParam != VK_TAB)
			{

				if (wParam == VK_DELETE)
				{
					G::x88Mode = !G::x88Mode;
					I::InputSystem->EnableInput(!G::x88Mode);

					return TRUE;
				}

				if (G::x88Mode)
				{
					switch (wParam)
					{
						case VK_F1:
						{
							C::LegitAimbot.chvh = !C::LegitAimbot.chvh;
							C::LegitAimbot.FovLimit = C::LegitAimbot.chvh ? 1.5f : 1.22f;
							C::VisualExtras.VisualRemovals[REMOVALS_FLASH] = C::LegitAimbot.chvh;
							C::VisualExtras.VisualRemovals[REMOVALS_ZOOM] = C::LegitAimbot.chvh;
							C::VisualExtras.VisualRemovals[REMOVALS_SCOPE_OVERLAY] = C::LegitAimbot.chvh;
							C::VisualExtras.VisualRemovals[REMOVALS_SCOPE_BLUR] = C::LegitAimbot.chvh;
							
							break;
						}

						case VK_F2:
							C::LegitAimbot.EnableLegitAim = !C::LegitAimbot.EnableLegitAim;
							break;

						case VK_F3:
							C::LegitAimbot.SilentAim = !C::LegitAimbot.SilentAim;
							break;

						case VK_F4:
							C::LegitTriggerbot.Enabled = !C::LegitTriggerbot.Enabled;
							break;

						case VK_F6:
							C::LegitBacktrack.Enabled = !C::LegitBacktrack.Enabled;
							break;

						case VK_F7:
							C::LegitMisc.EnableRCS = !C::LegitMisc.EnableRCS;
							break;

						case VK_F8:
							C::AntiAim.FakeLag = !C::AntiAim.FakeLag;
							break;

						case VK_F9:
							C::AntiAim.PowerPeek = !C::AntiAim.PowerPeek;
							break;

						case VK_F11:
							C::AntiAim.AntiTrigger = !C::AntiAim.AntiTrigger;
							break;

						case 0x31:
							C::Miscellaneous.BunnyHop = !C::Miscellaneous.BunnyHop;
							break;

						case 0x32:
							C::Miscellaneous.Slidewalk = !C::Miscellaneous.Slidewalk;
							break;

						case 0x33:
							C::VisualExtras.DeadVisuals = !C::VisualExtras.DeadVisuals;
							break;

						case 0x34:
							C::LegitMisc.SniperCrosshair = !C::LegitMisc.SniperCrosshair;
							break;

						case 0x35:
							C::ModelChams.WeaponChams = !C::ModelChams.WeaponChams;
							break;

						case 0x36:
							C::LAA.Mode = ((C::LAA.Mode + 1) > 2) ? 0 : C::LAA.Mode + 1;
							break;

						case 0x37:
							C::Miscellaneous.AnimatedClantag = !C::Miscellaneous.AnimatedClantag;
							break;

						case VK_OEM_4:
						{
							if (C::Miscellaneous.AnimatedClantag && C::Miscellaneous.ClantagStyle != 0)
								C::Miscellaneous.ClantagStyle--;
							break;
						}

						case VK_OEM_6:
						{
							if (C::Miscellaneous.AnimatedClantag && C::Miscellaneous.ClantagStyle != 22)
								C::Miscellaneous.ClantagStyle++;
							break;
						}
					}

					return TRUE;
				}
			}
		}

		return CallWindowProcA(oWndProc, hWnd, uMsg, wParam, lParam);
	}
}