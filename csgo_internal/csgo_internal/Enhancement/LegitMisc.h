#pragma once
#include "..\SDK\SDK.h"

class CLegitMisc
{
public:
	void CreateMove(CUserCmd* cmd, bool* send_packet);
	void EndScene();

public:
	Vector2D screenp = Vector2D (0, 0);
	bool draw_rch = false;

	QAngle last_punch = QAngle(0, 0, 0);
	bool last_attack = false;

public:
	bool EnableRCS = false;

	int RCSAfterShotsAmount = 0;

	int RCSStrengthPitch = 90;
	int RCSStrengthYaw = 90;
	int AimPause = 0;

	bool RecoilCrosshair = true;
	bool SniperCrosshair = true;
};