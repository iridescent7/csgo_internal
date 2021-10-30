#pragma once

#include "..\SDK\SDK.h"

enum _LEGIT_AA_DIR : bool
{
	LAA_LEFT  = 0,
	LAA_RIGHT = 1
};

enum _LEGIT_AA_MODE : int
{
	LAA_OFF    = 0,
	LAA_LBY    = 1,
	LAA_DESYNC = 2,
};

class LegitAA
{
public:
	void CreateMove(CUserCmd*, bool*);
	void EndScene(void);
	void PaintTraverse(void);

public:
	int   Mode = LAA_OFF;
	bool  Direction = LAA_LEFT;

private:
	bool QueueAttack = false; // queue IN_ATTACK when choking and only send it on unchoke
};