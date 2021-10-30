#pragma once
#include "..\SDK\SDK.h"

struct ATPlayerInfo
{
	wchar_t     name[25] = L"...";
	bool        valid = false;
};

class CAntiAim
{
private:
	//float last_lby = 0.f;
	//float last_update = 0.f;

	bool flip_desync = false;

public:
	void CreateMove(CUserCmd* cmd, bool* send_packet);
	void EndScene();
	void PaintTraverse();

	// Credits to Inure for this, since im lazy to do this myself and really want to get it done quick
	// link to his UC thread: https://www.unknowncheats.me/forum/counterstrike-global-offensive/326195-free-standing.html
	bool  TraceToExitQuick(Vector& point, const Vector& dir, const float step, float distance);
	float GetWallThickness(const Vector& start, const Vector& end, float distance);

public:
	bool Enabled = false;

	bool FakeLag     = false,
	     PowerPeek   = false,
	     AntiTrigger = false,
	     ATList      = false;

	int ATAmount      = 9,
	    FakeLagAmount = 9;

	float ATFOV       = 40;

	ATPlayerInfo ATPlayers[MAX_PLAYERS];

	bool Freestanding = false;
	bool Desync = false;
};
