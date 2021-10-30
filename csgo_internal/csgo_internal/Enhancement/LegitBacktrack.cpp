#include "LegitBacktrack.h"
#include "..\Globals.h"

void CLegitBacktrack::CreateMove(CUserCmd* cmd, bool* send_packet)
{
	if (!Enabled || C::Aimbot.Enabled)
		return;

	auto local_idx    = I::Engine->GetLocalPlayer();
	auto local_player = I::EntityList->GetClientEntity(local_idx);

	if (!local_player)
		return;

	last_local_simtime = local_player->m_flSimulationTime();

	//if (!local_player->IsAlive())
	//	return;
	
	int local_team = local_player->m_iTeamNum();
	int num        = cmd->command_number % MAX_LAGTICKS+1;

	for (size_t idx = 1; idx != MAX_PLAYERS; ++idx)
	{
		C_CSPlayer* entity = nullptr;

		if (idx == local_idx
			|| !(entity = I::EntityList->GetClientEntity(idx))
			|| !entity->IsPlayer()
			|| entity->IsDormant()
			|| !entity->IsAlive()
			|| entity->m_bGunGameImmunity()
			|| local_team == entity->m_iTeamNum())
		{
			records[idx][num].simtime = -1;
			continue;
		}

		// lazy. make a class to cache this later
		//matrix3x4_t boneMatrix[MAXSTUDIOBONES];
		entity->SetupBones(records[idx][num].bones, MAXSTUDIOBONES, BONE_USED_BY_ANYTHING, 0.f);

		studiohdr_t*	    studiohdr = I::ModelInfo->GetStudioModel(entity->GetModel());
		mstudiohitboxset_t* hitboxSet = studiohdr->pHitboxSet(0);

		mstudiobbox_t* hitbox = hitboxSet->GetHitbox(HITBOX_HEAD);

		Vector center;
		M::VectorTransform((hitbox->bbmin + hitbox->bbmax) / 2.f, records[idx][num].bones[hitbox->bone], &center);

		records[idx][num].simtime        = entity->m_flSimulationTime();
		records[idx][num].head_position  = center;
		records[idx][num].world_position = entity->GetEyePosition();
	}

	if (local_player->m_flNextAttack() > I::Globals->curtime)
		return;

	auto local_weapon = (CBaseCombatWeapon*)I::EntityList->GetClientEntityFromHandle(local_player->m_hActiveWeapon());

	if (!local_weapon)
		return;

	auto weapon_data = local_weapon->GetCSWeaponData();

	if (!weapon_data)
		return;

	// todo: knife backtrack by distance and hit scan?
	if (weapon_data->basic_weapon_type == BASICWEAPONTYPE_KNIFE || weapon_data->basic_weapon_type == BASICWEAPONTYPE_BOMB_NADES)
		return;

	if (local_weapon->m_iClip1() <= 0)
		return;

	if (cmd->buttons & IN_ATTACK)
	{
		float best_fov     = FLT_MAX;
		float best_simtime = -1;

		QAngle shoot_angle = cmd->viewangles;
		shoot_angle		  -= local_player->m_aimPunchAngle() * C::weapon_recoil_scale->GetFloat();

		M::AngleNormalize(&shoot_angle);

		for (size_t idx = 1; idx != MAX_PLAYERS; ++idx)
		{
			if (idx == local_idx)
				continue;

			for (size_t i = 0; i != LagTickLimit+1; ++i)
			{
				auto& cur_record = records[idx][i];

				if (cur_record.simtime > last_local_simtime - 1)
				{
					QAngle angle = M::CalcAngle(local_player->GetEyePosition(), cur_record.head_position);
					float  fov	 = M::GetFov(shoot_angle, angle);

					if (fov < best_fov)
					{
						best_fov     = fov;
						best_simtime = cur_record.simtime;
					}
				}
			}
		}

		if (best_simtime > last_local_simtime - 1)
		{
			cmd->tick_count = TIME_TO_TICKS(best_simtime);
		}
	}
}

void CLegitBacktrack::EndScene()
{
	if (Enabled && DrawLagTicks && (!C::VisualExtras.DeadVisuals || C::VisualExtras.DeadVisuals && !I::EntityList->GetClientEntity(I::Engine->GetLocalPlayer())->IsAlive()) )
	{
		for (size_t idx = 1; idx != MAX_PLAYERS; ++idx)
		{
			for (size_t i = 0; i != LagTickLimit + 1; ++i)
			{
				if (records[idx][i].simtime > last_local_simtime - 1)
				{
					Vector2D screen;

					if (R::WorldToScreen(records[idx][i].head_position, &screen))
					{
						R::FilledRect(screen.x - 2, screen.y - 2, 4, 4, D3DCOLOR_ARGB(200, 0, 0, 0));
						R::FilledRect(screen.x - 1, screen.y - 1, 2, 2, D3DCOLOR_XRGB(255, 255, 255));
					}
				}
			}
		}
	}
}
