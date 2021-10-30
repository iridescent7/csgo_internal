#pragma once
#include <Windows.h>

#include "SDK\SDK.h"

#include "Utils\WinAPI.h"
#include "Utils\Math.h"
#include "Utils\Util.h"
#include "Utils\Render.h"
#include "Utils\Netvars.h"
#include "Utils\AutoWall.h"
#include "Utils\PredictionSystem.h"

#include "GUI\Menu.h"

#include "Enhancement\Aimbot.h"
#include "Enhancement\AntiAim.h"
#include "Enhancement\Glow.h"
#include "Enhancement\LegitAimbot.h"
#include "./Enhancement/LegitTriggerbot.h"
#include "Enhancement\LegitBacktrack.h"
#include "Enhancement\LegitMisc.h"
#include "Enhancement\Miscellaneous.h"
#include "Enhancement\ModelChams.h"
#include "Enhancement\PlayerEsp.h"
#include "Enhancement\NightMode.h"
#include "Enhancement\VisualExtras.h"
#include "Enhancement\LegitAA.h"

namespace G
{
	extern HWND Window;
	extern HMODULE Dll;

	extern CMenu Menu;
	extern CNetvars Netvars;

	extern int width, height;

	extern bool x88Mode;
	extern unsigned long x88Font;

	void SetupVars();
}

namespace C
{
	extern QAngle LastAngle;

	// ===
	extern CPredictionSystem PredictionSystem;
	extern CAutoWall Autowall;

	// ===
	extern CAimbot  Aimbot;
	extern CAntiAim AntiAim;
	extern LegitAA LAA;

	extern CLegitAimbot     LegitAimbot;
	extern CLegitTriggerbot LegitTriggerbot;
	extern CLegitBacktrack  LegitBacktrack;
	extern CLegitMisc	    LegitMisc;

	extern CMiscellaneous Miscellaneous;

	extern CPlayerEsp    PlayerEsp;
	extern CGlow	     Glow;
	extern CModelChams   ModelChams;
	extern CNightMode    NightMode;
	extern CVisualExtras VisualExtras;

	// ===
	extern IMaterial* Flat;
	extern IMaterial* Regular;
	extern IMaterial* Reflective;
	extern IMaterial* mGlow;

	// ===
	extern SpoofedConvar sv_cheats;
	extern SpoofedConvar sv_skyname;

	extern SpoofedConvar zoom_sensitivity_ratio_mouse;
	extern SpoofedConvar zoom_sensitivity_ratio_joystick;

	extern SpoofedConvar view_recoil_tracking; // view punch/recoil (aimpunch * 0.45 (default))
	extern SpoofedConvar weapon_recoil_view_punch_extra; // extra viewpunch

	extern SpoofedConvar weapon_debug_spread_show;
	extern SpoofedConvar cl_crosshair_recoil;

	// ===
	extern ConVar* weapon_recoil_scale;
	extern ConVar* sv_penetration_type;

	extern ConVar* ff_damage_reduction_bullets;
	extern ConVar* ff_damage_bullet_penetration;

	void SetupCheat();
}
