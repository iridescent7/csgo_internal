#include "Aimbot.h"
#include "..\SDK\SDK.h"
#include "..\Globals.h"

int head_hitboxes[] =
{
	HITBOX_HEAD
};

int body_hitboxes[] =
{
	HITBOX_BODY,
	HITBOX_PELVIS,
	HITBOX_THORAX,
	HITBOX_UPPER_CHEST,
	HITBOX_CHEST
};

int limb_hitboxes[] =
{
	HITBOX_LEFT_FOOT,
	HITBOX_RIGHT_FOOT,
	HITBOX_LEFT_UPPER_ARM,
	HITBOX_RIGHT_UPPER_ARM,
	HITBOX_LEFT_THIGH,
	HITBOX_RIGHT_THIGH,
	HITBOX_LEFT_CALF,
	HITBOX_RIGHT_CALF,
};

std::pair<int*, size_t> hitbox_options[]
{
	{ head_hitboxes, ARRAYSIZE(head_hitboxes) },
	{ body_hitboxes, ARRAYSIZE(body_hitboxes) },
	{ limb_hitboxes, ARRAYSIZE(limb_hitboxes) }
};

void CAimbot::CreateMove(CUserCmd* cmd, bool* send_packet)
{
	if (!Enabled)
		return;

	auto local_player = I::EntityList->GetClientEntity(I::Engine->GetLocalPlayer());

	if (!local_player || !local_player->IsAlive())
		return;

	if (local_player->m_flNextAttack() > I::Globals->curtime)
		return;

	auto local_weapon = (CBaseCombatWeapon*)I::EntityList->GetClientEntityFromHandle(local_player->m_hActiveWeapon());

	if (!local_weapon)
		return;

	auto weapon_data = local_weapon->GetCSWeaponData();

	if (!weapon_data)
		return;

	if (weapon_data->basic_weapon_type == BASICWEAPONTYPE_KNIFE || weapon_data->basic_weapon_type == BASICWEAPONTYPE_BOMB_NADES)
		return;

	if (local_weapon->m_iClip1() <= 0)
		return;

	bool can_shoot = true;

	if (local_weapon->IsRevolver())
	{
		float ready_time = local_weapon->m_flPostponeFireReadyTime();

		if ( ready_time < 0.f || ready_time > I::Globals->curtime )
		{
			// always on for ragebot (auto revolver)
			cmd->buttons |= IN_ATTACK;
			can_shoot = false;
		}
	}

	if (local_weapon->m_flNextPrimaryAttack() > I::Globals->curtime)
	{
		can_shoot = false;
	}

	C_CSPlayer* best_target;
	QAngle      aimAngle;

	if (!FindTarget(local_player, local_weapon, weapon_data->range, cmd, &best_target, &aimAngle))
		return;

	bool on_ground = (local_player->m_fFlags() & FL_ONGROUND);
	bool jump = (cmd->buttons & IN_JUMP);

	if (on_ground && !jump)
	{
		if (AutoStop)
		{
			float speed     = local_player->m_vecVelocity().Length();
			float max_speed = local_player->m_bIsScoped() ? weapon_data->max_speed_alt : weapon_data->max_speed;

			if (speed > (max_speed / 3))
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
				cmd->sidemove	 = forward.y;
				cmd->upmove		 = 0.f;

				// wait until we get 100% accuracy
				return;
			}
		}

		if (AutoScope && !local_player->m_bIsScoped())
		{
			int id = local_weapon->m_iItemDefinitionIndex();

			if (id == WEAPON_AWP
				|| id == WEAPON_SSG08
				|| id == WEAPON_SCAR20
				|| id == WEAPON_G3SG1
				|| id == WEAPON_AUG
				|| id == WEAPON_SG556)
			{
				cmd->buttons |= IN_ATTACK2;
			}
		}
	}

	bool weapon_accurate = this->Hitchance(cmd, local_player, local_weapon, aimAngle, best_target, MinHitchance);

	if (!can_shoot || !weapon_accurate)
		return;

	// apply recoil
	aimAngle -= local_player->m_aimPunchAngle() * C::weapon_recoil_scale->GetFloat();

	M::AngleNormalize(&aimAngle);

	float new_forward, new_side;
	M::FixMovement(cmd, aimAngle.yaw, &new_forward, &new_side);
	
	cmd->viewangles  = aimAngle;
	cmd->buttons    |= IN_ATTACK;

	cmd->forwardmove = new_forward;
	cmd->sidemove	 = new_side;
	cmd->upmove		 = 0.f;

	if(ShowAim)
		I::Engine->SetViewAngles(aimAngle);
}

bool CAimbot::FindTarget(C_CSPlayer* local_player, CBaseCombatWeapon* local_weapon, float max_range, CUserCmd* cmd, C_CSPlayer** best_target, QAngle* best_angle)
{
	*best_target = nullptr;

	float bestDamage   = 0.f;
	float bestFov      = FLT_MAX;
	float bestDistance = FLT_MAX;

	int target_selection = TargetSelection;

	C_CSPlayer* potential_lethal_target = nullptr;
	QAngle      potential_lethal_target_aim_angle;
	float       potential_lethal_target_score = (target_selection != TARGET_MOST_DAMAGE) ? FLT_MAX : 0.f;

	QAngle local_view_angles = cmd->viewangles;
	Vector local_eye_pos     = local_player->GetEyePosition();

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
		float damage;

		float best_dmg = 0.f;
		bool  found = false;

		for (size_t i = 0; i < ARRAYSIZE(hitbox_options); ++i)
		{
			Vector temp;

			if (HitboxSelection[i] && GetBestPoint(local_player, local_weapon, entity, hitbox_options[i].first, hitbox_options[i].second, &temp, &damage))
			{
				if (damage > best_dmg)
				{
					point = temp;

					best_dmg = damage;
					found = true;
				}
			}
		}

		if (!found)
			continue;

		damage = best_dmg;

		bool lethal = (damage >= 1.f) && (damage >= entity->m_iHealth());

		if (!lethal && damage < MinDamage)
			continue;

		float distance = (local_eye_pos - point).Length();

		if (distance > max_range) // uhh
			continue;

		QAngle aim_angle = M::CalcAngle(local_eye_pos, point);
		M::AngleNormalize(&aim_angle);

		float fov = M::GetFov(local_view_angles, aim_angle);

		if (FovLimit < 180 && fov > (float)FovLimit)
			continue;

		if (target_selection == TARGET_MOST_DAMAGE)
		{
			if (lethal && damage > potential_lethal_target_score)
			{
				potential_lethal_target = entity;
				potential_lethal_target_aim_angle = aim_angle;
				potential_lethal_target_score = damage;
			}
			if (damage > bestDamage)
			{
				bestDamage   = damage;
				*best_angle  = aim_angle;
				*best_target = entity;
			}
		}

		else if (target_selection == TARGET_NEAREST_DIST)
		{
			if (lethal && distance < potential_lethal_target_score)
			{
				potential_lethal_target = entity;
				potential_lethal_target_aim_angle = aim_angle;
				potential_lethal_target_score = distance;
			}
			if (distance < bestDistance)
			{
				bestDistance = distance;
				*best_angle  = aim_angle;
				*best_target = entity;
			}
		}

		else if (target_selection == TARGET_CLOSEST_FOV)
		{
			if (lethal && fov < potential_lethal_target_score)
			{
				potential_lethal_target = entity;
				potential_lethal_target_aim_angle = aim_angle;
				potential_lethal_target_score = fov;
			}
			if (fov < bestFov)
			{
				bestFov      = fov;
				*best_angle  = aim_angle;
				*best_target = entity;
			}
		}
	}

	if (!*best_target && potential_lethal_target)
	{
		*best_target = potential_lethal_target;
		*best_angle  = potential_lethal_target_aim_angle;
	}

	return ( *best_target != nullptr );
}

bool CAimbot::GetBestPoint(C_CSPlayer* local_player, CBaseCombatWeapon* weapon, C_CSPlayer* target, int* hitboxes, size_t hitboxes_length, Vector* bestPoint, float* bestDamage)
{
	*bestDamage = 0.f;

	matrix3x4_t boneMatrix[MAXSTUDIOBONES];
	target->SetupBones(boneMatrix, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, 0.f);

	studiohdr_t*	    studiohdr = I::ModelInfo->GetStudioModel(target->GetModel());
	mstudiohitboxset_t* hitboxSet = studiohdr->pHitboxSet(0);

	Vector points[4];
	Vector local_eye_position = local_player->GetEyePosition();

	bool autowall = Autowall;

	for (size_t i = 0; i < hitboxes_length; ++i)
	{
		mstudiobbox_t* hitbox = hitboxSet->GetHitbox(hitboxes[i]);

		Vector center;
		M::VectorTransform((hitbox->bbmin + hitbox->bbmax) / 2.f, boneMatrix[hitbox->bone], &center);

		QAngle angle = M::CalcAngle(local_eye_position, center);
		Vector forward;
		M::AngleVectors(angle, &forward);

		Vector right = forward.Cross(Vector(0, 0, 1));
		Vector left = Vector(-right.x, -right.y, right.z);
		Vector top = Vector(0, 0, 1);

		// hands / feet are still OBB nad has radius set to -1
		//float radius = hitbox->radius < 0.f ?
		float ratio = hitbox->radius * MultipointScale;

		points[0] = center;
		points[1] = center + left  * ratio;
		points[2] = center + right * ratio;

		size_t point_count;

		if (hitboxes[i] == HITBOX_HEAD)
		{
			points[3] = center + top * ratio;
			point_count = 4;
		}
		else
		{
			point_count = 3;
		}

		int target_health = target->m_iHealth();

		for (size_t i = 0; i < point_count; ++i)
		{
			float damage;

			if (autowall)
			{
				damage = C::Autowall.GetDamage(local_player, weapon, target, local_eye_position, points[i]);
			}
			else
			{
				trace_t trace;
				CTraceFilter filter;
				filter.pSkip = local_player;

				U::TraceLine(local_eye_position, points[i], MASK_SHOT, &filter, &trace);

				if (trace.m_pEnt != target)
					continue;

				damage = weapon->GetCSWeaponData()->damage;

				C::Autowall.ScaleDamage(target, trace.hitgroup, weapon->GetCSWeaponData()->armor_ratio, damage);
			}

			if (damage > *bestDamage)
			{
				*bestDamage = damage;
				*bestPoint  = points[i];

				if (*bestDamage >= target_health)
					return true;
			}
		}
	}

	return (*bestDamage >= 1.f);
}

// pasted from subliminal
// link: https://www.unknowncheats.me/forum/cs-go-releases/302199-subliminal.html
bool CAimbot::Hitchance(CUserCmd* cmd, C_CSPlayer* local_player, CBaseCombatWeapon* weapon, QAngle angle, C_CSPlayer* target, float min_chance)
{
	Vector
		forward,
		right,
		up,
		src = local_player->GetEyePosition();

	M::AngleVectors(angle, &forward, &right, &up);

	int
		hits = 0,
		needed_hits = (int)(256.f * (min_chance / 100.f));

	float
		weap_range = weapon->GetCSWeaponData()->range,
		weap_spread = weapon->GetSpread(),
		weap_inaccuracy = weapon->GetInaccuracy();

	for (int i = 0; i < 256; ++i)
	{
		float
			a = M::RandFloat(0.f, 1.f),
			b = M::RandFloat(0.f, 2.f * PI),
			c = M::RandFloat(0.f, 1.f),
			d = M::RandFloat(0.f, 2.f * PI),
			inaccuracy = a * weap_inaccuracy,
			spread = c * weap_spread;

		if (weapon->IsRevolver())
		{
			if (cmd->buttons & IN_ATTACK2)
			{
				a = 1.f - a * a;
				c = 1.f - c * c;
			}
		}

		Vector
			spread_view((cos(b) * inaccuracy) + (cos(d) * spread), (sin(b) * inaccuracy) + (sin(d) * spread), 0),
			direction;

		direction.x = forward.x + (spread_view.x * right.x) + (spread_view.y * up.x);
		direction.y = forward.y + (spread_view.x * right.y) + (spread_view.y * up.y);
		direction.z = forward.z + (spread_view.x * right.z) + (spread_view.y * up.z);
		direction.NormalizeInPlace();

		QAngle viewangles_spread;
		Vector view_forward;

		M::VectorAngles(direction, &up, &viewangles_spread);
		M::AngleNormalize(&viewangles_spread);

		M::AngleVectors(viewangles_spread, &view_forward);
		view_forward.NormalizeInPlace();

		view_forward = src + (view_forward * weap_range);

		trace_t tr;
		Ray_t ray;

		ray.Init(src, view_forward);
		I::Trace->ClipRayToEntity(ray, MASK_SHOT | CONTENTS_GRATE, target, &tr);

		if (tr.m_pEnt == target)
			++hits;

		if ((int)(((float)hits / 256.f) * 100.f) >= min_chance)
			return true;

		if ((256 - i + hits) < needed_hits)
			return false;
	}

	return false;
}
