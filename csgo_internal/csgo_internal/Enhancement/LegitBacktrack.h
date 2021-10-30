#pragma once
#include "..\SDK\SDK.h"
#include <vector>

#define MAX_LAGTICKS 12

struct TickRecord
{
	float       simtime;
	Vector      head_position;
	Vector      world_position;
	matrix3x4_t bones[MAXSTUDIOBONES];
};

class CLegitBacktrack
{

public:
	void CreateMove(CUserCmd* cmd, bool* send_packet);
	void EndScene();

	CLegitBacktrack()
	{
		records = new TickRecord*[MAX_PLAYERS];

		for (size_t idx = 1; idx != MAX_PLAYERS; ++idx)
		{
			records[idx] = new TickRecord[MAX_LAGTICKS + 1];

			for (size_t i = 0; i != MAX_LAGTICKS+1; ++i)
				records[idx][i].simtime = -1;
		}
	}

public:
	bool Enabled = false;
	int LagTickLimit = MAX_LAGTICKS;
	
	bool DrawLagTicks = true;

	TickRecord** records = nullptr;//TickRecord records[MAX_PLAYERS][MAX_LAGTICKS+1];
	float last_local_simtime = 0.f;
};