#include "LegitMisc.h"
#include "..\SDK\SDK.h"
#include "..\Globals.h"

void CLegitMisc::CreateMove(CUserCmd* cmd, bool* send_packet)
{
	auto local_player = I::EntityList->GetClientEntity(I::Engine->GetLocalPlayer());

	if (!local_player)
		return;

	draw_rch = false;
	
	auto local_weapon = (CBaseCombatWeapon*)I::EntityList->GetClientEntityFromHandle(local_player->m_hActiveWeapon());

	if (!local_weapon)
		return;

	auto weapon_data = local_weapon->GetCSWeaponData();

	if (!weapon_data)
		return;

	bool is_recoil_weapon =
		(weapon_data->weapon_type == WEAPONTYPE_SUBMACHINEGUN
			|| weapon_data->weapon_type == WEAPONTYPE_RIFLE
			|| weapon_data->weapon_type == WEAPONTYPE_MACHINEGUN);

	if (RecoilCrosshair)
	{
		QAngle punch = local_player->m_aimPunchAngle();
		QAngle lp_ang = QAngle(cmd->viewangles.pitch, cmd->viewangles.yaw, cmd->viewangles.roll);

		if (punch.pitch || punch.yaw)
		{
			lp_ang += punch * C::weapon_recoil_scale->GetFloat();

			Vector lp_eyepos = local_player->GetEyePosition();
			Vector dst(0, 0, 0);

			dst.x = weapon_data->range * cosf(DEG2RAD(lp_ang.pitch)) * cosf(DEG2RAD(lp_ang.yaw));
			dst.y = weapon_data->range * cosf(DEG2RAD(lp_ang.pitch)) * sinf(DEG2RAD(lp_ang.yaw));
			dst.z = weapon_data->range * -sinf(DEG2RAD(lp_ang.pitch));
			dst += lp_eyepos;

			CTraceFilter filter;
			filter.pSkip = local_player;
			trace_t trace;

			U::TraceLine(lp_eyepos, dst, MASK_SHOT, &filter, &trace);

			R::WorldToScreen(trace.endpos, &screenp);

			draw_rch = true;
		}
	}

	/*
	bool should_recoil_crosshair = (RecoilCrosshair && is_recoil_weapon);

	if ( should_recoil_crosshair != C::cl_crosshair_recoil.GetBool() )
	{
		C::cl_crosshair_recoil.SetInt(should_recoil_crosshair ? 1 : 0);
	}
	*/

	if (C::Aimbot.Enabled)
		return;

	if (!local_player->IsAlive()
		|| local_player->m_flNextAttack() > I::Globals->curtime)
	{
		last_punch = QAngle(0, 0, 0);
		return;
	}

	bool first_shot = !last_attack && (cmd->buttons & IN_ATTACK);

	if ( first_shot && local_player->m_iShotsFired() < RCSAfterShotsAmount )
		return;

	if (EnableRCS && is_recoil_weapon /*&& !C::LegitAimbot.target_lock*/)
	{
		QAngle delta = QAngle(local_player->m_aimPunchAngle().pitch - last_punch.pitch,
			local_player->m_aimPunchAngle().yaw - last_punch.yaw, 0.f);

		float scale = C::weapon_recoil_scale->GetFloat();

		if (RCSStrengthPitch > 0)
			cmd->viewangles.pitch -= delta.pitch * (((float)RCSStrengthPitch / 100.f) * scale);

		if (RCSStrengthYaw > 0)
			cmd->viewangles.yaw -= delta.yaw * (((float)RCSStrengthYaw / 100.f) * scale);

		M::AngleNormalize(&cmd->viewangles);
		I::Engine->SetViewAngles(cmd->viewangles);
	}

	last_punch.pitch = local_player->m_aimPunchAngle().pitch;
	last_punch.yaw   = local_player->m_aimPunchAngle().yaw;
}

void CLegitMisc::EndScene()
{
	if (!I::Engine->IsInGame())
		return;

	if (RecoilCrosshair && draw_rch)
	{
		R::FilledRect(screenp.x - 2, screenp.y - 2, 5, 5, D3DCOLOR_XRGB(0, 0, 0));
		R::FilledRect(screenp.x - 1, screenp.y - 1, 3, 3, D3DCOLOR_XRGB(255, 192, 203));
	}

	if (C::LegitMisc.SniperCrosshair)
	{
		R::FilledRect(G::width / 2 - 2, G::height / 2 - 2, 5, 5, D3DCOLOR_XRGB(0, 0, 0));
		R::FilledRect(G::width / 2 - 1, G::height / 2 - 1, 3, 3, D3DCOLOR_XRGB(135, 206, 235));
	}
}

		/*
		if (RecoilCrosshair)
		{
			auto local_player = I::EntityList->GetClientEntity(I::Engine->GetLocalPlayer());
			
			if (local_player)
			{
				QAngle punch = local_player->m_aimPunchAngle();

				if (punch.pitch || punch.yaw)
				{
					R::FilledRect(G::width / 2 - 2 - punch.yaw * 9 * C::weapon_recoil_scale->GetFloat(), G::height / 2 - 2 + punch.pitch * 9, 5, 5, D3DCOLOR_XRGB(0, 0, 0));
					R::FilledRect(G::width / 2 - 1 - punch.yaw * 9 * C::weapon_recoil_scale->GetFloat(), G::height / 2 - 1 + punch.pitch * 9, 3, 3, D3DCOLOR_XRGB(255, 192, 203));

				}
			}
		}
		*/