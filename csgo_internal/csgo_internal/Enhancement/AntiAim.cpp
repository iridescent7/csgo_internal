#include "AntiAim.h"
#include "..\Globals.h"

// lazy
inline bool IsFiring(CUserCmd* cmd, CBaseCombatWeapon* local_weapon)
{
	auto weapon_data = local_weapon->GetCSWeaponData();

	if (!weapon_data)
		return false;

	auto weapon_type = weapon_data->weapon_type;

	if (weapon_type == WEAPONTYPE_GRENADE)
	{
		return true;

		//if (weapon->m_bPinPulled() && !(cmd->buttons & IN_ATTACK || cmd->buttons & IN_ATTACK2))
		//	return false;
	}
	else if (weapon_type == WEAPONTYPE_KNIFE || (local_weapon->IsRevolver() && local_weapon->m_iClip1() > 0))
	{
		if (cmd->buttons & (IN_ATTACK | IN_ATTACK2))
			return true;
	}
	else if (cmd->buttons & IN_ATTACK && local_weapon->m_iClip1() > 0)
		return true;

	return false;
}

void CAntiAim::CreateMove(CUserCmd* cmd, bool* send_packet)
{
	static bool shouldAntiTrigger = false;

	if (FakeLag || C::LegitAimbot.chvh || PowerPeek || AntiTrigger || ATList)
	{

		auto local_idx = I::Engine->GetLocalPlayer();
		auto local_player = I::EntityList->GetClientEntity(local_idx);

		if (!local_player || !local_player->IsAlive())
			return;

		if (local_player->m_bGunGameImmunity())
			return;

		// dont fakelag when defusing
		if (cmd->buttons & IN_USE)
			return;

		auto choked = I::Engine->GetNetChannel()->m_nChokedPackets;

		if ( C::AntiAim.AntiTrigger || C::AntiAim.ATList )
		{
			shouldAntiTrigger = false;

			for (int idx = 1; idx != MAX_PLAYERS; idx++)
			{
				ATPlayers[idx].valid = false;

				if (idx == local_idx)
					continue;

				auto entity = I::EntityList->GetClientEntity(idx);

				if (!entity || entity->IsDormant() || !entity->IsAlive() || entity->m_iTeamNum() == local_player->m_iTeamNum())
					continue;

				if (ATPlayers[idx].valid = M::GetFov(entity->m_angEyeAngles(), M::CalcAngle(entity->GetEyePosition(), local_player->GetEyePosition())) <= C::AntiAim.ATFOV)
				{
					player_info_t player_info;

					if (I::Engine->GetPlayerInfo(idx, &player_info))
					{
						ATPlayers[idx].name[24] = L'\0';

						U::ConvertANSIToUnicode(player_info.name, ATPlayers[idx].name, 25);

						if (ATPlayers[idx].name[24] != L'\0')
							wcsncpy(&ATPlayers[idx].name[20], L" ...\0", 5);
					}
				}

				if (!shouldAntiTrigger && ATPlayers[idx].valid)
					shouldAntiTrigger = true;


			}
		}

		// ghetto power peek
		if ( ( (C::AntiAim.PowerPeek && cmd->sidemove != 0.f) || FakeLag || (C::AntiAim.AntiTrigger && shouldAntiTrigger) ) && (choked < FakeLagAmount) && !(cmd->buttons & IN_ATTACK) )
			* send_packet = false;
		else if (Desync && choked == 0)
			* send_packet = false;

		/*

		if (local_player->m_nMoveType() == MOVETYPE_LADDER
			|| local_player->m_nMoveType() == MOVETYPE_NOCLIP)
			return;

		auto local_weapon = (CBaseCombatWeapon*)I::EntityList->GetClientEntityFromHandle(local_player->m_hActiveWeapon());
nex's
		if (!local_weapon)
			return;

		bool is_firing = (local_weapon && IsFiring(cmd, local_weapon));

		if (Freestanding && !is_firing)
		{
			int   local_team = local_player->m_iTeamNum();
			float best_fov = FLT_MAX;

			C_CSPlayer* best_target = nullptr;

			QAngle angle;
			I::Engine->GetViewAngles(angle);

			for (size_t idx = 1; idx != MAX_PLAYERS; ++idx)
			{
				if (idx == local_idx)
					continue;

				auto entity = I::EntityList->GetClientEntity(idx);

				if (!entity
					|| !entity->IsPlayer()
					|| entity->IsDormant()
					|| !entity->IsAlive()
					|| entity->m_iTeamNum() == local_team)
				{
					continue;
				}

				Vector origin = entity->m_vecOrigin();
				origin.z += 36.f + (-9.f * entity->m_flDuckAmount());

				QAngle aim_angle = M::CalcAngle(local_player->GetEyePosition(), origin);
				float  fov = M::GetFov(angle, aim_angle);

				if (fov < best_fov)
				{
					best_fov = fov;
					best_target = entity;
				}
			}

			bool  active = false;
			float best_yaw_rad = 0.f;

			CBaseCombatWeapon* target_weapon;

			if (best_target && (target_weapon = (CBaseCombatWeapon*)I::EntityList->GetClientEntityFromHandle(best_target->m_hActiveWeapon())))
			{
				// todo: do something better than this
				matrix3x4_t boneMatrix[MAXSTUDIOBONES];
				local_player->SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, 0.f);

				studiohdr_t* studiohdr = I::ModelInfo->GetStudioModel(local_player->GetModel());
				mstudiohitboxset_t* hitboxSet = studiohdr->pHitboxSet(0);

				mstudiobbox_t* hitbox = hitboxSet->GetHitbox(HITBOX_HEAD);

				Vector head_center;
				M::VectorTransform((hitbox->bbmin + hitbox->bbmax) / 2.f, boneMatrix[hitbox->bone], &head_center);

				Vector eyepos = local_player->GetEyePosition();
				Vector delta = local_player->m_vecOrigin() - head_center;

				float radius = fabs(delta.Length2D());
				float best_thickness = 0.f;

				float max_range = target_weapon->GetCSWeaponData()->range;

				for (float step = 0.f; step < PI * 2.f; step += PI * 2.f / 36.f)
				{
					Vector new_headpos = Vector
					(
						radius * cos(step) + eyepos.x,
						radius * sin(step) + eyepos.y,
						eyepos.z
					);

					float thickness = GetWallThickness(best_target->GetEyePosition(), new_headpos, max_range);

					if (thickness > best_thickness)
					{
						active = true;

						best_thickness = thickness;
						best_yaw_rad = step;
					}
				}
			}

			float yaw = active ? RAD2DEG(best_yaw_rad) : cmd->viewangles.yaw + 180.f;
			M::NormalizeYaw(&yaw);

			float forward, side;
			M::FixMovement(cmd, yaw, &forward, &side);

			cmd->viewangles.pitch = 89;
			cmd->viewangles.yaw = yaw;
			cmd->viewangles.roll = 0.f;

			cmd->forwardmove = forward;
			cmd->sidemove = side;
			cmd->upmove = 0.f;

			if (!C::Miscellaneous.Slidewalk)
			{
				cmd->buttons &= ~(IN_FORWARD | IN_BACK | IN_MOVELEFT | IN_MOVERIGHT);

				if (forward != 0.f)
				{
					cmd->buttons |= (forward > 0.f) ? IN_FORWARD : IN_BACK;
				}

				if (side != 0.f)
				{
					cmd->buttons |= (side > 0.f) ? IN_MOVERIGHT : IN_MOVELEFT;
				}
			}
		}

		if (Desync)
		{
			// todo: use lby break later
		//	const float current_lby  = local_player->m_flLowerBodyYawTarget();
		//	const float current_time = local_player->m_nTickBase() * I::Globals->interval_per_tick;

		//	const float delta      = ceilf((current_time - last_update) * 100.f) / 100.f;
		//	const float next_delta = ceilf((delta + I::Globals->interval_per_tick) * 100.f) / 100.f;

		//	if (local_player->m_vecVelocity().Length2D() <= 0.f)
		//	{
		//		if (current_lby != 180.f && last_lby != current_lby)
		//		{
		//			//printf("lby updated after %.4f", delta);

		//			last_lby    = current_lby;
		//			last_update = current_time - I::Globals->interval_per_tick;
		//		}
		//		else if (next_delta >= 1.1f)
		//		{
		//			//printf("curr: %.4f; next: %.4f", delta, next_delta);

		//			*send_packet = true;
		//			last_update  = current_time;
		//		}
		//	}
		//	else
		//	{
		//		last_lby    = current_lby;
		//		last_update = current_time;
		//	}

			if (*send_packet)
			{
				auto  animstate = local_player->GetAnimState();
				float max_desync_delta = U::GetMaxDesyncDelta(animstate); // no need for max delta, it gets clamped anyway. but whatever

				// this should do, i'll just put in a big number later, since it gets clamped anyway
				cmd->viewangles.yaw += flip_desync ? max_desync_delta : -max_desync_delta;
				M::NormalizeYaw(&cmd->viewangles.yaw);

				flip_desync = !flip_desync;
			}
		}
		*/
		M::AngleNormalize(&cmd->viewangles);
	}
}

bool CAntiAim::TraceToExitQuick(Vector& point, const Vector& dir, const float step, float distance)
{
	float current_distance = 0.f;

	while (current_distance <= distance)
	{
		current_distance += step;
		point            += dir * current_distance;

		int point_contents = I::Trace->GetPointContents(point, MASK_SHOT_HULL);

		if (!(point_contents & MASK_SHOT_HULL))
		{
			// found first free point
			return true;
		}
	}

	return false;
}

float CAntiAim::GetWallThickness(const Vector& start, const Vector& end, float distance)
{
	Vector dir  = end - start;
	Vector step = start;

	if (distance >= 0.f && dir.Length() > distance)
		return -1;

	dir.NormalizeInPlace();

	CTraceWorldOnly filter;
	trace_t trace;
	Ray_t ray;

	float thickness = 0.f;

	while (true)
	{
		ray.Init(step, end);
		I::Trace->TraceRay(ray, MASK_SHOT_HULL, &filter, &trace);

		if (!trace.DidHit())
			return thickness;

		const Vector lastStep = trace.endpos;
		step = trace.endpos;

		if ((end - start).Length() <= (step - start).Length())
			break;

		if (!TraceToExitQuick(step, dir, 5, 90))
			return FLT_MAX;

		thickness += (step - lastStep).Length();
	}

	return thickness;
}

void CAntiAim::EndScene()
{
	auto lp = I::EntityList->GetClientEntity(I::Engine->GetLocalPlayer());

	if (!lp || !lp->IsAlive())
		return;

	if (C::AntiAim.AntiTrigger || C::AntiAim.ATList)
	{
		static int x = G::width - 200,
				   y = G::height / 2 - 100;

		int offset = 0;

		R::Text(F::EspText, x, y, D3DCOLOR_ARGB(255, 255, 255, 0), "Enemy Player FOV: ");

		for (int idx = 1; idx != MAX_PLAYERS; idx++)
			if (ATPlayers[idx].valid)
				R::TextW(F::EspText, x, y + (++offset * 20), D3DCOLOR_ARGB(255, 255, 255, 0), ATPlayers[idx].name);
	}
}

void CAntiAim::PaintTraverse()
{
	auto lp = I::EntityList->GetClientEntity(I::Engine->GetLocalPlayer());

	if (!lp || !lp->IsAlive())
		return;

	if (C::AntiAim.AntiTrigger || C::AntiAim.ATList)
	{
		static int x = G::width - 100,
				   y = G::height / 2 - 100;

		int offset = 0;

		I::Surface->DrawTextW(L"Enemy Player FOV: ", x, y, G::x88Font, 255, 255, 255);

		for (int idx = 1; idx != MAX_PLAYERS; idx++)
			if (ATPlayers[idx].valid)
				I::Surface->DrawTextW(ATPlayers[idx].name, x, y + (++offset * 15), G::x88Font, 255, 255, 255);
	}
}