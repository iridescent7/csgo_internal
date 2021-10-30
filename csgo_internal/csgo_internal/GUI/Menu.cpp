#include "Menu.h"
#include "Colors.h"
#include "..\Globals.h"

#include <chrono>

CMenu::CMenu()
{
	Enabled = false;
	bAnimFinished = true;

	alpha = 0;

	LButton = 0;
	CurrentKey = -1;

	// Initialize our window
	window = Window("cheet", 200, 200, 720, 600,
	{
		/*
		Tab("RAGEBOT",
		{
			new GroupBox("Aimbot",200,500,
			{
				new CheckBox("Enabled", &C::Aimbot.Enabled),

				new CheckBox("Enable autowall", &C::Aimbot.Autowall),
				new Slider("Fov limit", &C::Aimbot.FovLimit,1,180),

				new ComboBox("Target selection",{ "Most damage", "Nearest distance", "Closest fov" }, &C::Aimbot.TargetSelection),
				new ItemSelector("Hitbox selection",{ "Head","Body","Limbs" }, C::Aimbot.HitboxSelection),

				new SliderFloat("Multipoint scale", &C::Aimbot.MultipointScale),

				new Slider("Minimum hitchance", &C::Aimbot.MinHitchance),
				new Slider("Minimum damage", &C::Aimbot.MinDamage),

				new CheckBox("Auto stop", &C::Aimbot.AutoStop),
				new CheckBox("Auto scope", &C::Aimbot.AutoScope),

				new CheckBox("Show aim", &C::Aimbot.ShowAim)
			}),
		}),
		*/
		Tab("LEGITBOT",
		{
			new GroupBox("Legit",200,500,
			{
				new CheckBox("Closet HvH", &C::LegitAimbot.chvh),

				new CheckBox("Enable aimbot", &C::LegitAimbot.EnableLegitAim),
				new CheckBox("Enable triggerbot (MMB)", &C::LegitTriggerbot.Enabled),
				new CheckBox("Enable backtrack", &C::LegitBacktrack.Enabled),

				new Space(),

				//new CheckBox("Silent Aim", &C::LegitAimbot.SilentAim),
				new CheckBox("Aim Auto stop", &C::LegitAimbot.AutoStop),
				new ComboBox("Hitbox", { "Head", "Body", "Any Body", "Any Limb", "All - Priority Head", "All - Priority Body"}, &C::LegitAimbot.HitboxSelect),
				new SliderFloat("Fov", &C::LegitAimbot.FovLimit, 0.f, 5.f),
				new Slider("Smooth amount", &C::LegitAimbot.SmoothAmount,0,12),
				new Slider("Aim Min Damage", nullptr, 0, 120),
				new Slider("Aim Hit Chance", nullptr, 0, 100),
				
				new Slider("Trigger Min Damage", &C::LegitTriggerbot.mindamage, 1, 120),
				new Slider("Trigger Hit Chance", &C::LegitTriggerbot.hitchance, 0, 100),
				//new Slider("Pause Aim Shot", &C::LegitMisc.AimPause, 0, 1000),

			}),
			new GroupBox("Extra",200,500,
			{
				new Slider("Backtrack Tick limit", &C::LegitBacktrack.LagTickLimit, 1, MAX_LAGTICKS),
				
				new Space(),

				new CheckBox("Enable RCS", &C::LegitMisc.EnableRCS),
				new Slider("RCS pitch strength", &C::LegitMisc.RCSStrengthPitch, 0, 100),
				new Slider("RCS yaw strength", &C::LegitMisc.RCSStrengthYaw, 0, 100),
			}),
			new GroupBox("Anti aim",200,500,
			{
				//new CheckBox("Enable anti aim", &C::AntiAim.Enabled),

				new CheckBox("Fake lag",&C::AntiAim.FakeLag),
				new CheckBox("Power (Side) Peek",&C::AntiAim.PowerPeek),
				new Slider("Amount",&C::AntiAim.FakeLagAmount,1,17),
				
				new Space(),

				new CheckBox("Anti Trigger",&C::AntiAim.AntiTrigger),
				new CheckBox("List Players",&C::AntiAim.ATList),
				new SliderFloat("FOV",&C::AntiAim.ATFOV,1,180),
				new Slider("Amount",&C::AntiAim.ATAmount,1,17),
				//new CheckBox("Legit Anti Aim", nullptr),
				/*
				new CheckBox("Freestanding",&C::AntiAim.Freestanding),
				new CheckBox("Random desync flip",&C::AntiAim.Desync),
				*/
			})
		}),
		Tab("VISUAL",
		{
			new GroupBox("Player esp", 200, 500,
			{
				new CheckBox("Dead Visuals", &C::VisualExtras.DeadVisuals),

				new ItemSelector("Esp team selection",{ "Team", "Enemy" }, C::PlayerEsp.EspTeams),
				new ItemSelector("Feature selection",{ "Name", "Weapon", "Bounding box", "Health bar", "Ammo bar", "Barrel" }, C::PlayerEsp.EspFeatures),
			
				new Space(),

				new ItemSelector("Glow team selection",{ "Team", "Enemy" }, C::Glow.GlowTeams),
				new ComboBox("Glow style",{ "Regular", "Pulsing", "Outline" }, &C::Glow.GlowStyle),

				new Space(),

				new ItemSelector("Chams team selection",{ "Team", "Enemy" }, C::ModelChams.ChamsTeams),
				new ItemSelector("Chams mode selection",{ "Visible", "Invisible" }, C::ModelChams.ChamsMode),

				new Space(2),

				new ComboBox("Visible chams type",{ "Flat", "Regular", "Original" }, &C::ModelChams.VisibleChamsType),
				new ComboBox("Invisible chams type",{ "Flat", "Regular", "Original" }, &C::ModelChams.InvisibleChamsType)
			}),
			new GroupBox("Others",200,500,
			{
				new CheckBox("Custom crosshair", &C::LegitMisc.SniperCrosshair),
				new CheckBox("Show recoil crosshair", &C::LegitMisc.RecoilCrosshair),

				new ItemSelector("Visual removals",{"Recoil","Zoom","Decals","Scope overlay","Scope blur","Flash","Smoke"},C::VisualExtras.VisualRemovals),

				new Space(),

				new CheckBox("Weapons Chams",&C::ModelChams.WeaponChams),
				//new CheckBox("C4 Chams",&C::ModelChams.C4Chams),
				//new KeyBind("Thirdperson key",&C::VisualExtras.ThirdpersonKey),

				new Space(),

				new CheckBox("Override fov", &C::VisualExtras.OverrideFov),
				new Slider("Fov amount",&C::VisualExtras.Fov, 60, 140),
				new Slider("Thirdperson fov amount",&C::VisualExtras.ThirdpersonFov, 60, 140),

				new Space(),

				new CheckBox("Nightmode", &C::NightMode.NightModeEnabled),
				new CheckBox("Transparent props", &C::NightMode.AsusPropsEnabled),
				new SliderFloat("Transparent amount", &C::NightMode.AsusPropsAmount),

				new Space(),

				new CheckBox("Draw lag ticks", &C::LegitBacktrack.DrawLagTicks)
			})
		}),
		Tab("MISC",
		{
			new GroupBox("Misc", 200, 500,
			{
				new ComboBox("Clantag",
					{
						"Nanahira",
						"CSJAPAN",
						"BOT",
						"netaminity",
						"BOT + Florette",
						"suika.xyz",
						"x88cheats",
						"desu",
						"closethvh",
						"UWUWARE.net",
						"WatchTheDemo",
						"acc-suicide",
						"Black Florette",
						"White Florette",
						"awoo",
						"ZettaiRyoiki",
						"DraculaPremium",
						"too.lewd.se",
						"cute",
						"hentaiware.moe",
						"Stars (Anim)",
						"gayysense (Anim)",
						"blendS (Anim)"
					}, &C::Miscellaneous.ClantagStyle),
				new CheckBox("Enable clantag spam",&C::Miscellaneous.AnimatedClantag),
				new CheckBox("Enable rank revealer",&C::Miscellaneous.RankRevealer),

				new Space(),

				new CheckBox("Enable bunnyhop",&C::Miscellaneous.BunnyHop),
				new CheckBox("Enable unlimited duck",&C::Miscellaneous.UnlimitedDuck),
				new CheckBox("Enable slide walk",&C::Miscellaneous.Slidewalk),

				new Space(),

				new CheckBox("Enable block bot",&C::Miscellaneous.BlockBot)
				
			})
		})
	});
}

void CMenu::EndScene()
{
	if (!Enabled && bAnimFinished)
		return;

	auto diff_count = GetTickCount() - last_update_tick;

	if (Enabled)
	{
		if (diff_count < 200)
			this->alpha = (byte)(diff_count / 200.f * 255 + 0.5f);
		else
			this->alpha = 255;

		if(G::Window == GetForegroundWindow())
			this->PollInput();
	}
	else
	{
		if (diff_count < 200)
			this->alpha = (byte)((200-diff_count) / 200.f * 255.f + 0.5f);
		else
		{
			this->alpha = 0;
			bAnimFinished = true;
		}
	}

	this->window.SetAlpha(alpha);
	this->window.Draw();
}

void CMenu::PollInput()
{
	POINT mouse_pos;
	GetCursorPos(&mouse_pos);
	ScreenToClient(G::Window, &mouse_pos);

	Mouse = mouse_pos;

	// todo
	if (GetAsyncKeyState(VK_LBUTTON))
	{
		if (LButton == 0)
			LButton = 1;
		else
			LButton = 2;
	}
	else
		LButton = 0;

	this->window.Think();
}

void CMenu::WndProcCallbk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (!Enabled)
	{
		if (uMsg == WM_KEYUP && wParam == VK_INSERT)
		{
			I::InputSystem->EnableInput(false);

			last_update_tick = GetTickCount();
			Enabled = true;
			bAnimFinished = false;
		}
		return;
	}

	switch (uMsg)
	{
	case WM_KEYDOWN:
		if (wParam <= 255)
			CurrentKey = wParam;
		break;
	case WM_KEYUP:
		if (wParam <= 255)
		{
			if (wParam == VK_INSERT || wParam == VK_ESCAPE)
			{
				I::InputSystem->EnableInput(true);

				last_update_tick = GetTickCount();
				Enabled = false;

				CurrentKey = -1;
			}
			else if (wParam == CurrentKey)
				CurrentKey = -1;
		}
		break;
	default:
		return;
	}
}

bool CMenu::MouseOver(int x, int y, int x1, int y1)
{
	return (Mouse.x > x
		&& Mouse.y > y
		&& Mouse.x < x1
		&& Mouse.y < y1);
}

void CMenu::SaveConfig(std::string filename)
{
}

void CMenu::LoadConfig(std::string filename)
{
}