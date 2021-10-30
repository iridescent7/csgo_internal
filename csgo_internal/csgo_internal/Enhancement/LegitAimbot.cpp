#include "LegitAimbot.h"
#include "..\SDK\SDK.h"
#include "..\Globals.h"

void CLegitAimbot::CreateMove(CUserCmd* cmd, bool* send_packet)
{
	static bool queueAttack = false;
	static int nextAimPause = 0;

	if (!EnableLegitAim || !I::Engine->IsInGame())
		return;

	if (current_smoothing < 0)
		target_lock = nullptr;

	auto local_player = I::EntityList->GetClientEntity(I::Engine->GetLocalPlayer());

	if (!local_player || !local_player->IsAlive())
		return;

	auto local_weapon = (CBaseCombatWeapon*)I::EntityList->GetClientEntityFromHandle(local_player->m_hActiveWeapon());

	if (!local_weapon)
		return;
	
	if (local_weapon->m_flNextPrimaryAttack() > I::Globals->curtime)
		return;

	auto weapon_data = local_weapon->GetCSWeaponData();

	if (!weapon_data)
		return;

	if (local_player->m_flNextAttack() > I::Globals->curtime)
		return;

	bool on_ground = (local_player->m_fFlags() & FL_ONGROUND);

	if (!on_ground || (cmd->buttons & IN_JUMP))
		return;

	if (nextAimPause > I::Globals->curtime)
		return;

	if (weapon_data->basic_weapon_type == BASICWEAPONTYPE_KNIFE || weapon_data->basic_weapon_type == BASICWEAPONTYPE_BOMB_NADES)
		return;

	if (local_weapon->m_iClip1() <= 0)
		return;

	//if (ShotAutoStop && cmd->buttons & IN_ATTACK)
	//	cmd->sidemove = cmd->forwardmove = 0.f;

	QAngle aimAngle;

	if (!target_lock)
	{
		bool aim_onkey = chvh || (!locking && OnKey && GetAsyncKeyState(VK_MBUTTON));

		// who the fuck uses the garbage revolver in an actual match?
		if (!aim_onkey)
		{
			if (!(cmd->buttons & IN_ATTACK)) // || !(local_weapon->IsRevolver() && (cmd->buttons & IN_ATTACK2)))
				return;
			else if (last_attack)
				return;
		}

		C_CSPlayer* best_target;

		if (!FindTarget(local_player, local_weapon, weapon_data->range, cmd, &best_target, &aimAngle))
			return;

		locking = true;
		target_lock = best_target;
		current_smoothing = SmoothAmount;
	}
	else
	{
		if (target_lock->IsDormant() || !target_lock->IsAlive())
		{
			current_smoothing = -1;
			return;
		}

		Vector point;
		GetHitbox(target_lock, point);

		trace_t trace;
		CTraceFilter filter;
		filter.pSkip = local_player;

		U::TraceLine(local_player->GetEyePosition(), point, MASK_SHOT, &filter, &trace);

		if (trace.m_pEnt != target_lock)
		{
			current_smoothing = -1;
			return;
		}

		aimAngle = M::CalcAngle(local_player->GetEyePosition(), point);
		M::AngleNormalize(&aimAngle);
	}

	if (chvh || AutoStop)
	{
		float speed = local_player->m_vecVelocity().Length();
		float max_speed = local_player->m_bIsScoped() ? weapon_data->max_speed_alt : weapon_data->max_speed;

		if (speed > (max_speed / 3.3f))
		{
			QAngle angle;
			M::VectorAngles(local_player->m_vecVelocity(), &angle);

			angle.yaw = cmd->viewangles.yaw - angle.yaw;

			Vector forward;
			M::AngleVectors(angle, &forward);

			float ratio = 450.f / max(fabs(forward.x), fabs(forward.y));

			forward *= ratio;

			Clamp(forward.x, -450.f, 450.f);
			Clamp(forward.y, -450.f, 450.f);

			cmd->forwardmove = forward.x;
			cmd->sidemove = forward.y;
			cmd->upmove = 0.f;

			// wait until we get 100% accuracy
			cmd->buttons &= ~IN_ATTACK;
			return;
		}
	}

	if (current_smoothing > 0)
	{
		QAngle viewang_recoil = cmd->viewangles;
		viewang_recoil -= local_player->m_aimPunchAngle() * C::weapon_recoil_scale->GetFloat();

		QAngle delta;

		float opitch = viewang_recoil.pitch + 89;
		float pitch = aimAngle.pitch + 89;
		float pitch_diff = fabs(opitch - pitch);

		if (opitch > pitch)
			delta.pitch = -(pitch_diff / current_smoothing);
		else
			delta.pitch = pitch_diff / current_smoothing;

		float oyaw = viewang_recoil.yaw + 180;
		float yaw = aimAngle.yaw + 180;
		float yaw_diff = fabs(oyaw - yaw);

		if (yaw_diff > 180.f)
		{
			// negate the minus in the upcoming if else condition
			yaw_diff = -(360.f - yaw_diff);
		}

		if (oyaw > yaw)
			delta.yaw = -(yaw_diff / current_smoothing);
		else
			delta.yaw = yaw_diff / current_smoothing;

		delta.roll = 0.f;

		float fov = delta.LengthSqr();

		if (fov < (1 / current_smoothing * fov))
		{
			current_smoothing = -1;

			cmd->viewangles = aimAngle;
			cmd->buttons |= IN_ATTACK;
			
			if (C::LegitMisc.AimPause)
				nextAimPause = I::Globals->curtime + C::LegitMisc.AimPause;
		}
		else
		{
			cmd->viewangles += delta;
			M::AngleNormalize(&cmd->viewangles);

			/*
				if (local_weapon->IsRevolver())
					cmd->buttons &= ~(IN_ATTACK | IN_ATTACK2);
				else
			*/
			cmd->buttons &= ~IN_ATTACK;
			--current_smoothing;
			/*
					Vector forward;
					M::AngleVectors(cmd->viewangles, &forward);
					forward.NormalizeInPlace();

					trace_t trace;
					CTraceFilter filter;
					filter.pSkip = local_player;

					U::TraceLine(local_player->GetEyePosition(), forward, MASK_SHOT, &filter, &trace);

					if (trace.m_pEnt == target_lock)
					{
						current_smoothing = -1;
						cmd->buttons |= IN_ATTACK;
					}
			*/
		}
	}
	else
	{
		current_smoothing = -1;

		cmd->viewangles = aimAngle;
		cmd->buttons |= IN_ATTACK;

		if (C::LegitMisc.AimPause)
			nextAimPause = I::Globals->curtime + C::LegitMisc.AimPause;
	}

	if (!SilentAim)
		I::Engine->SetViewAngles(cmd->viewangles);
}

bool CLegitAimbot::FindTarget(C_CSPlayer* local_player, CBaseCombatWeapon* local_weapon, float max_range, CUserCmd* cmd, C_CSPlayer** best_target, QAngle* best_angle)
{
	*best_target = nullptr;

	float bestFov = FovLimit + FLT_EPSILON;

	if (local_player->m_bIsScoped())
	{
		int id = local_weapon->m_iItemDefinitionIndex();

		if (id == WEAPON_AWP
			|| id == WEAPON_SCAR20
			|| id == WEAPON_G3SG1)
		{
			bestFov *= 0.3f;
		}
		else if (id == WEAPON_SSG08)
		{
			bestFov *= 0.4f;
		}
		else if (id == WEAPON_AUG || id == WEAPON_SG556)
		{
			bestFov *= 0.5f;
		}
	}

	QAngle recoil_delta = local_player->m_aimPunchAngle() * C::weapon_recoil_scale->GetFloat();

	QAngle local_view_angles = cmd->viewangles - recoil_delta;
	Vector local_eye_pos     = local_player->GetEyePosition();

	M::AngleNormalize(&local_view_angles);

	for (int idx = 1; idx != MAX_PLAYERS; ++idx)
	{
		C_CSPlayer* entity = I::EntityList->GetClientEntity(idx);

		if (!entity
			|| entity == local_player
			|| !entity->IsPlayer()
			|| entity->IsDormant()
			|| !entity->IsAlive()
			|| entity->m_bGunGameImmunity()
			|| entity->m_iTeamNum() == local_player->m_iTeamNum())
			continue;

		Vector point;
		GetHitbox(entity, point);

		trace_t trace;
		CTraceFilter filter;
		filter.pSkip = local_player;

		U::TraceLine(local_player->GetEyePosition(), point, MASK_SHOT, &filter, &trace);

		if (trace.m_pEnt != entity)
			continue;

		float distance = (local_eye_pos - point).Length();

		if (distance > max_range) // uhh
			continue;

		QAngle aim_angle = M::CalcAngle(local_eye_pos, point);
		aim_angle       -= recoil_delta;

		M::AngleNormalize(&aim_angle);

		float fov = M::GetFov(local_view_angles, aim_angle);

		if (fov < bestFov)
		{
			bestFov = fov;
			*best_angle = aim_angle;
			*best_target = entity;
		}
	}

	return (*best_target != nullptr);
}

void CLegitAimbot::GetHitboxPoint(C_CSPlayer* target, int iHitbox, Vector* point)
{
	matrix3x4_t boneMatrix[MAXSTUDIOBONES];
	target->SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, 0.f);

	studiohdr_t*	    studiohdr = I::ModelInfo->GetStudioModel(target->GetModel());
	mstudiohitboxset_t* hitboxSet = studiohdr->pHitboxSet(0);

	mstudiobbox_t* hitbox = hitboxSet->GetHitbox(iHitbox);

	M::VectorTransform((hitbox->bbmin + hitbox->bbmax) / 2.f, boneMatrix[hitbox->bone], point);
}

bool CLegitAimbot::GetHitbox(C_CSPlayer* target, Vector& point)
{	
	static const int hb_body[] =
	{
		HITBOX_BODY,
		HITBOX_PELVIS,
		HITBOX_THORAX,
		HITBOX_UPPER_CHEST,
		HITBOX_CHEST
	};
	
	static const int hb_limb[] =
	{
		HITBOX_LEFT_UPPER_ARM,
		HITBOX_RIGHT_UPPER_ARM,
		HITBOX_LEFT_THIGH,
		HITBOX_RIGHT_THIGH,
		HITBOX_LEFT_FOOT,
		HITBOX_RIGHT_FOOT,
		HITBOX_LEFT_CALF,
		HITBOX_RIGHT_CALF,
	};

	static const int hb_all_head[] =
	{
		HITBOX_HEAD,
		HITBOX_NECK,
		HITBOX_BODY,
		HITBOX_PELVIS,
		HITBOX_THORAX,
		HITBOX_UPPER_CHEST,
		HITBOX_CHEST,
		HITBOX_LEFT_FOOT,
		HITBOX_RIGHT_FOOT,
		HITBOX_LEFT_UPPER_ARM,
		HITBOX_RIGHT_UPPER_ARM,
		HITBOX_LEFT_THIGH,
		HITBOX_RIGHT_THIGH,
		HITBOX_LEFT_CALF,
		HITBOX_RIGHT_CALF
	};

	static const int hb_all_body[] =
	{
		HITBOX_BODY,
		HITBOX_PELVIS,
		HITBOX_THORAX,
		HITBOX_UPPER_CHEST,
		HITBOX_CHEST,
		HITBOX_HEAD,
		HITBOX_NECK,
		HITBOX_LEFT_FOOT,
		HITBOX_RIGHT_FOOT,
		HITBOX_LEFT_UPPER_ARM,
		HITBOX_RIGHT_UPPER_ARM,
		HITBOX_LEFT_THIGH,
		HITBOX_RIGHT_THIGH,
		HITBOX_LEFT_CALF,
		HITBOX_RIGHT_CALF
	};

	static const int* hb_list[] =
	{
		{ },
		{ },
		hb_body,
		hb_limb,
		hb_all_head,
		hb_all_body
	};

	static const int hb_size[] =
	{
		1,
		1,
		ARRAYSIZE(hb_body),
		ARRAYSIZE(hb_limb),
		ARRAYSIZE(hb_all_head),
		ARRAYSIZE(hb_all_body),
	};

	C_CSPlayer* lp = I::EntityList->GetClientEntity(I::Engine->GetLocalPlayer());

	if (!lp)
		return false;

	if (HitboxSelect == 0 || HitboxSelect == 1)
	{
		int hb_tmp = HitboxSelect == 0 ? HITBOX_HEAD : HITBOX_BODY;
		GetHitboxPoint(target, hb_tmp, &point);
		return true;
	}
	else
	{
		Vector point_buff;
		float fov_buff;

		Vector nearest_point;
		float  nearest_fov = 180.0f;

		for (int x = 0; x < hb_size[HitboxSelect]; x++)
		{
			GetHitboxPoint(target, hb_list[HitboxSelect][x], &point_buff);

			if ((fov_buff = M::GetFov(lp->m_angEyeAngles(), M::CalcAngle(lp->GetEyePosition(), point_buff))) <= FovLimit && fov_buff < nearest_fov)
			{
				nearest_point = point_buff;
				nearest_fov   = fov_buff;
			}
		}

		point = nearest_point;

		return true;
	}

	return false;
}