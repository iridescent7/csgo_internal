#pragma once

#include "LegitAA.h"

void LegitAA::CreateMove(CUserCmd* cmd, bool* send_packet)
{
	bool ingame = false;

	if (!Mode || !(ingame = I::Engine->IsInGame()))
		return;

	C_CSPlayer* lp = I::EntityList->GetClientEntity(I::Engine->GetLocalPlayer());
	if (!lp)
		return;

	CCSPlayerAnimState* animstate = lp->GetAnimState();
	if (!animstate)
		return;



	if (!*send_packet && cmd->buttons & IN_ATTACK)
	{
		QueueAttack = true;
		cmd->buttons &= ~IN_ATTACK;
	}
	else if (*send_packet && QueueAttack)
	{
		QueueAttack = false;
		cmd->buttons |= IN_ATTACK;
	}
}

void LegitAA::EndScene()
{

}

void LegitAA::PaintTraverse()
{

}