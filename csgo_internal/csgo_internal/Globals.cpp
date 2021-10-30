#include "Globals.h"
#include "Interfaces.h"
#include "Offsets.h"

namespace G
{
	HWND Window;
	HMODULE Dll;

	CMenu Menu;
	CNetvars Netvars;

	int width, height;

	bool x88Mode = false;
	unsigned long x88Font;

	void SetupVars()
	{
		// for menu
		C::Aimbot  = CAimbot();
		C::AntiAim = CAntiAim();
		C::LAA = LegitAA();

		C::LegitAimbot    = CLegitAimbot();
		C::LegitBacktrack = CLegitBacktrack();
		C::LegitMisc      = CLegitMisc();

		C::Miscellaneous  = CMiscellaneous();

		C::PlayerEsp    = CPlayerEsp();
		C::Glow         = CGlow();
		C::ModelChams   = CModelChams();
		C::NightMode    = CNightMode();
		C::VisualExtras = CVisualExtras();

		// ===
		Menu = CMenu();
		Netvars = CNetvars();
	}
}

namespace C
{
	QAngle LastAngle;

	// ===
	CPredictionSystem PredictionSystem;
	CAutoWall Autowall;

	// ===
	CAimbot  Aimbot;
	CAntiAim AntiAim;
	LegitAA LAA;

	CLegitAimbot     LegitAimbot;
	CLegitTriggerbot LegitTriggerbot;
	CLegitBacktrack  LegitBacktrack;
	CLegitMisc	     LegitMisc;

	CMiscellaneous Miscellaneous;

	CPlayerEsp    PlayerEsp;
	CGlow	      Glow;
	CModelChams   ModelChams;
	CNightMode    NightMode;
	CVisualExtras VisualExtras;

	// ===
	IMaterial* Flat;
	IMaterial* Regular;
	IMaterial* Reflective;
	IMaterial* mGlow;

	// ===
	SpoofedConvar sv_cheats;
	SpoofedConvar sv_skyname;

	SpoofedConvar zoom_sensitivity_ratio_mouse;
	SpoofedConvar zoom_sensitivity_ratio_joystick;

	SpoofedConvar view_recoil_tracking; // view punch/recoil (aimpunch * 0.45 (default))
	SpoofedConvar weapon_recoil_view_punch_extra; // extra viewpunch

	SpoofedConvar weapon_debug_spread_show;
	SpoofedConvar cl_crosshair_recoil;

	// ===
	ConVar* weapon_recoil_scale;
	ConVar* sv_penetration_type;

	ConVar* ff_damage_reduction_bullets;
	ConVar* ff_damage_bullet_penetration;

	void SetupCheat()
	{
		LastAngle = QAngle(0, 0, 0);

		// ===
		PredictionSystem = CPredictionSystem();
		Autowall = CAutoWall();

		// ===
		sv_cheats = SpoofedConvar("sv_cheats");
		sv_skyname = SpoofedConvar("sv_skyname");

		view_recoil_tracking = SpoofedConvar("view_recoil_tracking");
		weapon_recoil_view_punch_extra = SpoofedConvar("weapon_recoil_view_punch_extra");

		weapon_debug_spread_show = SpoofedConvar("weapon_debug_spread_show");
		cl_crosshair_recoil = SpoofedConvar("cl_crosshair_recoil");

		// ===

		zoom_sensitivity_ratio_mouse = I::Cvar->FindVar("zoom_sensitivity_ratio_mouse");
		zoom_sensitivity_ratio_joystick = I::Cvar->FindVar("zoom_sensitivity_ratio_joystick");

		weapon_recoil_scale = I::Cvar->FindVar("weapon_recoil_scale");
		sv_penetration_type = I::Cvar->FindVar("sv_penetration_type");

		ff_damage_reduction_bullets = I::Cvar->FindVar("ff_damage_reduction_bullets");
		ff_damage_bullet_penetration = I::Cvar->FindVar("ff_damage_bullet_penetration");

		// ===
		I::Engine->ClientCmd_Unrestricted("exec csfg");

		// ===
		Flat = U::CreateMaterial("UnlitGeneric", R"#(
"UnlitGeneric"
{
      "$basetexture" "vgui/white_additive"
}
)#");
		Regular = U::CreateMaterial("VertexLitGeneric", R"#(
"VertexLitGeneric"
{
      "$basetexture" "vgui/white_additive"
}
)#");
		Reflective = U::CreateMaterial("glowOverlay", R"#(
"VertexLitGeneric"
{
	"$additive" "1"
	"$envmap"     "effects/cube_white"
	"$envmaptint" "[1 0 0]"
	"$envmapfresnel" "1"
	"$envmapfresnelminmaxexp" "[0 1 2]"
	"$alpha" "0.8"
}
)#");

		mGlow = U::CreateMaterial("glowOverlay", R"#("VertexLitGeneric" {
	    "$additive" "1"
	    "$envmap" "models/effects/cube_white"
	    "$envmaptint" "[1 0 0]"
	    "$envmapfresnel" "1"
	    "$envmapfresnelminmaxexp" "[0 1 2]"
	    "$alpha" "0.8"
    })#");
	}
}
