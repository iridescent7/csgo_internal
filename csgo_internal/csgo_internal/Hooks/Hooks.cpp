#include "Hooks.h"
#include "..\SDK\SDK.h"
#include "..\Globals.h"

#include <WinUser.h>
#include <intrin.h>

namespace H
{
	bool D3DInit = false;

	CreateMoveFn oCreateMove;
	PaintTraverseFn oPaintTraverse;

	FrameStageNotifyFn oFrameStageNotify;
	FireGameEventFn oFireGameEvent;
	OverrideViewFn oOverrideView;

	SceneEndFn oSceneEnd;
	DrawModelExecuteFn oDrawModelExecute;

	ResetFn oReset;
	EndSceneFn oEndScene;

	TraceRayFn oTraceRay;
	GetViewmodelFovFn oGetViewmodelFov;
	LockCursorFn oLockCursor;

	DrawSetColorFn oDrawSetColor;
	ListLeavesInBoxFn oListLeavesInBox;

	ClientCMDFn oClientCmd;

	WNDPROC oWndProc;

	struct x88color
	{
		int r, g, b;
	};

	struct x88item
	{
		const wchar_t* text;
		wchar_t* opt;
		x88color color;
	};

	void __stdcall Hooked_PaintTraverse(unsigned int vguiPanel, bool forceRepaint, bool allowForce)
	{
		static unsigned int MatSystemTopPanel = 0;

		if (!MatSystemTopPanel)
		{
			const char* panel_name = I::Panel->GetName(vguiPanel);

			if (!strcmp(panel_name, "MatSystemTopPanel"))
			{
				MatSystemTopPanel = vguiPanel;
			}
			else
			{
				return oPaintTraverse(I::Panel, vguiPanel, forceRepaint, allowForce);
			}
		}

		if (vguiPanel == MatSystemTopPanel)
		{
			// dumb fix
			CopyMemory(&R::WorldViewMatrix, &I::Engine->WorldToScreenMatrix(), sizeof(VMatrix));

			static x88color red    = {255, 0, 0};
			static x88color green  = {0, 255, 0};
			static x88color yellow = {255, 255, 0};
			static x88color blue   = {0, 0, 255};
			static int x = G::width * 0.2, x_opt = x + 95;

			#define autox88(value, text) {text, value ? L"ON" : L"OFF", value ? green : red}

			I::Surface->DrawTextW(L"x88cheets", I::Engine->IsInGame() ? x : 12, 30, G::x88Font, G::x88Mode ? rand()%255 : 255, G::x88Mode ? rand()%255 : 192, G::x88Mode ? rand()%255 : 203);

			if (I::Engine->IsInGame())
			{
				player_info_t pit;
				wchar_t nameBuffer[128] = L"konnichiwa ";
				wchar_t buff[128];
				I::Engine->GetPlayerInfo(I::Engine->GetLocalPlayer(), &pit);

				U::ConvertANSIToUnicode(pit.name, buff, 128);
				wcscat(nameBuffer, buff);

				I::Surface->DrawTextW(nameBuffer, x, 43, G::x88Font, 255, 192, 203);

				I::Surface->DrawTextW( (L"Choked Packets: " + std::to_wstring(I::Engine->GetNetChannel()->m_nChokedPackets)).c_str(), x, 56, G::x88Font, 255, 255, 0);

				int y = 80;

				x88item items[] =
				{
					autox88(C::LegitAimbot.chvh,           L"[F01] HvHMode:"),
					autox88(C::LegitAimbot.EnableLegitAim, L"[F02] Aimbot:"),
					autox88(C::LegitAimbot.SilentAim,      L"[F03] SilentAim:"),
					autox88(C::LegitTriggerbot.Enabled,    L"[F04] Triggerbot:"),
					autox88(C::LegitBacktrack.Enabled,     L"[F06] Backtrack:"),
					autox88(C::LegitMisc.EnableRCS,        L"[F07] RecoilControl:"),
					autox88(C::AntiAim.FakeLag,            L"[F08] FakeLag:"),
					autox88(C::AntiAim.PowerPeek,          L"[F09] PowerPeek"),
					autox88(C::AntiAim.AntiTrigger,        L"[F11] AntiTrigger:"),
					autox88(C::Miscellaneous.BunnyHop,     L"[001] BunnyHop:"),
					autox88(C::Miscellaneous.Slidewalk,    L"[002] SlideWalk:"),
					{ L"[003] ESP:", C::VisualExtras.DeadVisuals ? L"DEAD" : L"ON", C::VisualExtras.DeadVisuals ? blue : green},
					autox88(C::LegitMisc.SniperCrosshair,  L"[004] xhair:"),
					autox88(C::ModelChams.WeaponChams,     L"[005] WepChams:"),
				};

				for (x88item item : items)
				{
					I::Surface->DrawTextW(item.text, x, y, G::x88Font, 255, 255, 255);
					I::Surface->DrawTextW(item.opt, x_opt, y, G::x88Font, item.color.r, item.color.g, item.color.b);
					y += 13;
				}

				wchar_t* aa_txt = nullptr;
				x88color aa_color = C::LAA.Mode ? green : red;

				switch (C::LAA.Mode)
				{
					case LAA_OFF:
						aa_txt = L"OFF";
						break;

					case LAA_LBY:
						aa_txt = L"LBY";
						break;

					case LAA_DESYNC:
						aa_txt = L"DESYNC";
						break;
				}

				I::Surface->DrawTextW(L"[006] LAntiAim:", x, y, G::x88Font, 255, 255, 255);
				I::Surface->DrawTextW(aa_txt, x_opt, y, G::x88Font, aa_color.r, aa_color.g, aa_color.b);

				y += 13;

				static const wchar_t* clantags[] =
				{
					L"Nanahira",
					L"CSJAPAN",
					L"BOT",
					L"netaminity",
					L"BOT + Florette",
					L"suika.xyz",
					L"x88cheats",
					L"desu",
					L"closethvh",
					L"UWUWARE.net",
					L"WatchTheDemo",
					L"acc-suicide",
					L"Black Florette",
					L"White Florette",
					L"awoo",
					L"ZettaiRyoiki",
					L"DraculaPremium",
					L"too.lewd.se",
					L"cute",
					L"hentaiware.moe",
					L"Stars (Anim)",
					L"gayysense (Anim)",
					L"blendS (Anim)"
				};

				I::Surface->DrawTextW(L"[007] Clantag: ", x, y, G::x88Font, 255, 255, 255);
				I::Surface->DrawTextW(C::Miscellaneous.AnimatedClantag ? clantags[C::Miscellaneous.ClantagStyle] : L"OFF", x_opt, y, G::x88Font, C::Miscellaneous.AnimatedClantag ? yellow.r : red.r, C::Miscellaneous.AnimatedClantag ? yellow.g : red.g, C::Miscellaneous.AnimatedClantag ? yellow.b : red.b);

				C::AntiAim.PaintTraverse();
			}
		}

		oPaintTraverse(I::Panel, vguiPanel, forceRepaint, allowForce);
	}

	void __fastcall Hooked_TraceRay(void* thisptr, void* edx, const Ray_t& ray, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace)
	{
		oTraceRay(thisptr, ray, fMask, pTraceFilter, pTrace);

		if (C::VisualExtras.VisualRemovals[REMOVALS_DECALS] && I::Engine->IsInGame())
			pTrace->surface.flags |= SURF_SKY;
	}

	float __fastcall Hooked_GetViewmodelFov()
	{
		auto lp = I::EntityList->GetClientEntity(I::Engine->GetLocalPlayer());

		if (I::Engine->IsInGame() && lp && !lp->m_bIsScoped())
			return 98.f;
			
		return oGetViewmodelFov();
	}

	void __fastcall Hooked_LockCursor(void* thisptr, void* edx)
	{
		if (G::Menu.Enabled || G::x88Mode)
		{
			I::Surface->UnlockCursor();
			return;
		}

		oLockCursor(thisptr);
	}

	void __fastcall Hooked_DrawSetColor(void* thisptr, void* edx, int r, int g, int b, int a)
	{
		DWORD return_address = (DWORD)_ReturnAddress();

		if (C::VisualExtras.VisualRemovals[REMOVALS_SCOPE_OVERLAY]
			&& (return_address == O::DrawSetColor_ScopeArc || return_address == O::DrawSetColor_ScopeLens))
		{
			// override alpha by zero
			a = 0;
		}

		oDrawSetColor(thisptr, r, g, b, a);
	}

	int __fastcall Hooked_ListLeavesInBox(void* bsp, void* edx, Vector& mins, Vector& maxs, unsigned short* pList, int listMax)
	{
		// occlusion getting updated on player movement/angle change,
		// in RecomputeRenderableLeaves ( https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/client/clientleafsystem.cpp#L674 );
		// check for return in CClientLeafSystem::InsertIntoTree
		static auto InsertIntoTreeCallListLeavesInBox = U::FindSignature("client.dll", "56 52 FF 50 18") + 0x5;

        if ((DWORD)_ReturnAddress() != InsertIntoTreeCallListLeavesInBox || !C::ModelChams.ChamsMode[CHAMS_INVISIBLE])
            return oListLeavesInBox(bsp, mins, maxs, pList, listMax);

		// get current renderable info from stack ( https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/client/clientleafsystem.cpp#L1470 )
		auto info = *(RenderableInfo_t**)((DWORD)_AddressOfReturnAddress() + 0x14);

		if (!info || !info->m_pRenderable)
			return oListLeavesInBox(bsp, mins, maxs, pList, listMax);

		// check if disabling occlusion for players ( https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/client/clientleafsystem.cpp#L1491 )
		auto base_entity = info->m_pRenderable->GetIClientUnknown()->GetBaseEntity();
		if (!base_entity || !base_entity->IsPlayer())
			return oListLeavesInBox(bsp, mins, maxs, pList, listMax);

		// fix render order, force translucent group ( https://www.unknowncheats.me/forum/2429206-post15.html )
		// AddRenderablesToRenderLists: https://i.imgur.com/hcg0NB5.png ( https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/client/clientleafsystem.cpp#L2473 )
		info->m_Flags &= ~0x100;
		info->m_Flags2 |= 0xC0;

		// extend world space bounds to maximum ( https://github.com/pmrowla/hl2sdk-csgo/blob/master/game/client/clientleafsystem.cpp#L707 )
		static const Vector map_min = Vector(MIN_COORD_FLOAT, MIN_COORD_FLOAT, MIN_COORD_FLOAT);
		static const Vector map_max = Vector(MAX_COORD_FLOAT, MAX_COORD_FLOAT, MAX_COORD_FLOAT);

		auto   count = oListLeavesInBox(bsp, map_min, map_max, pList, listMax);
		return count;
	}

	HRESULT __stdcall Hooked_Reset(D3DPRESENT_PARAMETERS* pPresentParameters)
	{
		if (D3DInit)
		{
			R::OnLostDevice();

			HRESULT ret = oReset(pPresentParameters);

			R::OnResetDevice();
			return ret;
		}
		else
			return oReset(pPresentParameters);
	}

	void SetupHooks()
	{
		oWndProc = (WNDPROC)SetWindowLongA(G::Window, GWLP_WNDPROC, (LONG_PTR)Hooked_WndProc);

		oCreateMove = (CreateMoveFn)DetourFunction((*(PBYTE**)I::ClientMode)[24], (PBYTE)Hooked_CreateMove);
		oPaintTraverse = (PaintTraverseFn)DetourFunction((*(PBYTE**)I::Panel)[41], (PBYTE)Hooked_PaintTraverse);

		oFrameStageNotify = (FrameStageNotifyFn)DetourFunction((*(PBYTE**)I::Client)[37], (PBYTE)Hooked_FrameStageNotify);
		oFireGameEvent = (FireGameEventFn)DetourFunction((*(PBYTE**)I::GameEventManager)[9], (PBYTE)Hooked_FireGameEvent);
		oOverrideView = (OverrideViewFn)DetourFunction((*(PBYTE**)I::ClientMode)[18], (PBYTE)Hooked_OverrideView);

		oSceneEnd = (SceneEndFn)DetourFunction((*(PBYTE**)I::RenderView)[9], (PBYTE)Hooked_SceneEnd);
		oDrawModelExecute = (DrawModelExecuteFn)DetourFunction((*(PBYTE**)I::ModelRender)[21], (PBYTE)Hooked_DrawModelExecute);

		oTraceRay = (TraceRayFn)DetourFunction((*(PBYTE**)I::Trace)[5], (PBYTE)Hooked_TraceRay);
		oGetViewmodelFov = (GetViewmodelFovFn)DetourFunction((*(PBYTE**)I::ClientMode)[35], (PBYTE)Hooked_GetViewmodelFov);
		oLockCursor = (LockCursorFn)DetourFunction((*(PBYTE**)I::Surface)[67], (PBYTE)Hooked_LockCursor);

		oDrawSetColor = (DrawSetColorFn)DetourFunction((*(PBYTE**)I::Surface)[15], (PBYTE)Hooked_DrawSetColor);
		oListLeavesInBox = (ListLeavesInBoxFn)DetourFunction((*(PBYTE**)I::Engine->GetBSPTreeQuery())[6], (PBYTE)Hooked_ListLeavesInBox);

		oClientCmd = (ClientCMDFn)DetourFunction((*(PBYTE**)I::Engine)[7], (PBYTE)Hooked_ClientCmd);

		//oReset = (ResetFn)DetourFunction((*(PBYTE**)O::deviceStructureAddress)[16], (PBYTE)Hooked_Reset);
		oEndScene = (EndSceneFn)DetourFunction((*(PBYTE**)O::deviceStructureAddress)[42], (PBYTE)Hooked_EndScene);
	}
}