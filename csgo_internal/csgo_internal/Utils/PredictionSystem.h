#pragma once
#include "..\SDK\SDK.h"

class CPredictionSystem
{
public:
	void RunPrediction(CUserCmd* cmd, C_CSPlayer* local_player);
	void EndPrediction(C_CSPlayer* local_player);

private:
	float oldCurtime;
	float oldFrametime;
};