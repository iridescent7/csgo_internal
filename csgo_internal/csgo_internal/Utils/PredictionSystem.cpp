#include "PredictionSystem.h"
#include "..\SDK\SDK.h"
#include "..\Globals.h"
#include "..\Interfaces.h"

void CPredictionSystem::RunPrediction(CUserCmd* cmd, C_CSPlayer* local_player)
{
	int oldFlags = local_player->m_fFlags();

	this->oldCurtime   = I::Globals->curtime;
	this->oldFrametime = I::Globals->frametime;

	I::Globals->curtime   = local_player->m_nTickBase() * I::Globals->interval_per_tick;
	I::Globals->frametime = I::Globals->interval_per_tick;

	// todo: fix
	*O::nPredictionRandomSeed = MD5_PseudoRandom(cmd->command_number) & 0x7FFFFFFF;

	I::GameMovement->StartTrackPredictionErrors(local_player);

	CMoveData data;
	ZeroMemory(&data, sizeof(CMoveData));

	I::MoveHelper->SetHost(local_player);

	I::Prediction->SetupMove(local_player, cmd, I::MoveHelper, &data);
	I::GameMovement->ProcessMovement(local_player, &data);
	I::Prediction->FinishMove(local_player, cmd, &data);

	local_player->m_fFlags() = oldFlags;
}

void CPredictionSystem::EndPrediction(C_CSPlayer* local_player)
{
	I::GameMovement->FinishTrackPredictionErrors(local_player);
	I::MoveHelper->SetHost(NULL);

	I::Globals->curtime   = this->oldCurtime;
	I::Globals->frametime = this->oldFrametime;
}
