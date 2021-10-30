#pragma once
#include "..\SDK\SDK.h"

enum
{
	TARGET_MOST_DAMAGE,
	TARGET_NEAREST_DIST,
	TARGET_CLOSEST_FOV
};

enum
{
	HIT_HEAD,
	HIT_BODY,
	HIT_LIMBS
};

class CAimbot
{
public:
	void CreateMove(CUserCmd* cmd, bool* send_packet);

	bool FindTarget(C_CSPlayer* local_player, CBaseCombatWeapon* local_weapon, float max_range, CUserCmd* cmd, C_CSPlayer** best_target, QAngle* best_angle);
	bool GetBestPoint(C_CSPlayer* local_player, CBaseCombatWeapon* weapon, C_CSPlayer* target, int* hitboxes, size_t hitboxes_length, Vector* bestPoint, float* bestDamage);

	bool Hitchance(CUserCmd* cmd, C_CSPlayer* local_player, CBaseCombatWeapon* weapon, QAngle angle, C_CSPlayer* target, float min_chance);

public:
	bool Enabled = false;

	bool Autowall = false;
	int  FovLimit = 180;

	int  TargetSelection = TARGET_MOST_DAMAGE;
	bool HitboxSelection[3] = { true, true, true };

	float MultipointScale = 0.5f;

	int MinHitchance = 45;
	int MinDamage = 34;

	bool AutoStop = false;
	bool AutoScope = false;

	bool ShowAim = false;


};