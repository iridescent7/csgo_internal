#pragma once
#include "..\SDK\SDK.h"

enum
{
	AIM_HEAD,
	AIM_NECK
};

class CLegitAimbot
{
private:
	int current_smoothing = 0;

	bool GetHitbox(C_CSPlayer* entity, Vector& point);
public:
	void CreateMove(CUserCmd* cmd, bool* send_packet);

	bool FindTarget(C_CSPlayer* local_player, CBaseCombatWeapon* local_weapon, float max_range, CUserCmd* cmd, C_CSPlayer** best_target, QAngle* best_angle);
	void GetHitboxPoint(C_CSPlayer* target, int iHitbox, Vector* point);

public:
	C_CSPlayer* target_lock = nullptr;
	bool last_attack = false;
	bool locking = false;

public:
	bool EnableLegitAim = false;

	bool OnKey = false;
	bool chvh  = false;
	int AimKey = VK_MBUTTON;

	int   HitboxSelect = AIM_HEAD;
	float FovLimit = 1.22f;

	int SmoothAmount = 3;
	bool AutoStop = true;
	bool ShotAutoStop = false;
	bool SilentAim = false;
	//float RandomizationScale = 0.5f;
};