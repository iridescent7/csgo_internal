#include "LegitTriggerbot.h"
#include "../Globals.h"

void CLegitTriggerbot::CreateMove(CUserCmd *cmd, bool *send_packet)
{
	if (!Enabled || !I::Engine->IsInGame())
		return;

	if (C::LegitAimbot.chvh || GetAsyncKeyState(key))
	{

		auto localplayer = I::EntityList->GetClientEntity(I::Engine->GetLocalPlayer());

		if (!localplayer)
			return;

		if (localplayer->m_flNextAttack() > I::Globals->curtime)
			return;

		auto lpWeapon = (CBaseCombatWeapon*)I::EntityList->GetClientEntityFromHandle(localplayer->m_hActiveWeapon());

		if (!lpWeapon)
			return;

		if (lpWeapon->m_flNextPrimaryAttack() > I::Globals->curtime)
			return;

		CSWeaponInfo* weapondata = lpWeapon->GetCSWeaponData();

		if (lpWeapon->m_iClip1() < 1 || weapondata->basic_weapon_type == BASICWEAPONTYPE_BOMB_NADES || weapondata->basic_weapon_type == BASICWEAPONTYPE_KNIFE)
			return;

		trace_t trace;
		CTraceFilter filter;
		filter.pSkip = localplayer;

		// https://stackoverflow.com/questions/30619901/calculate-3d-point-coordinates-using-horizontal-and-vertical-angles-and-slope-di?rq=1
		dst.x = weapondata->range *  cosf(DEG2RAD(cmd->viewangles.pitch)) * cosf(DEG2RAD(cmd->viewangles.yaw));
		dst.y = weapondata->range *  cosf(DEG2RAD(cmd->viewangles.pitch)) * sinf(DEG2RAD(cmd->viewangles.yaw));
		dst.z = weapondata->range * -sinf(DEG2RAD(cmd->viewangles.pitch));

		

		dst += localplayer->GetEyePosition();

		//U::TraceLine(localplayer->GetEyePosition(), dst, MASK_SHOT, &filter, &trace);

		C_CSPlayer* entity = nullptr;

		for (int idx = 1; idx < MAX_PLAYERS; idx++)
		{
			C_CSPlayer* _entity = I::EntityList->GetClientEntity(idx);

			if (!_entity || !_entity->IsPlayer() || _entity->IsDormant() || !_entity->IsAlive() || _entity->m_iTeamNum() == localplayer->m_iTeamNum() || _entity->m_bGunGameImmunity())
				continue;

			float dmg = C::Autowall.GetDamage(localplayer, lpWeapon, _entity, localplayer->GetEyePosition(), dst);

			if (dmg < mindamage && dmg < _entity->m_iHealth())
				continue;

			entity = _entity;

			break;
		}

		if (!entity)
			return;

		float speed = localplayer->m_vecVelocity().Length();
		float max_speed = localplayer->m_bIsScoped() ? weapondata->max_speed_alt : weapondata->max_speed;

		if (!C::Aimbot.Hitchance(cmd, localplayer, lpWeapon, localplayer->m_angEyeAngles(), entity, hitchance))
		{
			cmd->forwardmove = -cmd->forwardmove;
			cmd->sidemove = -cmd->sidemove;
			cmd->upmove = 0.f;

			return;
		}

		cmd->buttons |= IN_ATTACK;
	}
}

void CLegitTriggerbot::EndScene()
{

}