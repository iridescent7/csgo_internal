#pragma once
#include "..\SDK\SDK.h"

struct FireBulletData
{
	Vector          current_position;
	trace_t         enter_trace;
	Vector          direction;
	CTraceFilter    filter;
	float           trace_length;
	float           trace_length_remaining;
	float           current_damage;
	int             penetrate_count;
};

class CAutoWall
{
public:
	void ClipTraceToPlayer(C_CSPlayer* entity, const Vector& start, const Vector& end, unsigned int mask, trace_t* tr);

	void GetBulletTypeParameters(float& max_range, float& max_distance, char* bullet_type);
	bool IsBreakableEntity(C_CSPlayer* entity);

	void ScaleDamage(C_CSPlayer* entity, int hitgroup, float weapon_armor_ratio, float& current_damage);
	bool TraceToExit(trace_t& enter_trace, trace_t& exit_trace, const Vector& start_position, const Vector& direction);

	bool HandleBulletPenetration(FireBulletData* data, CSWeaponInfo* weapon_data, trace_t& enter_trace, Vector& start_pos, const Vector& direction, int& possible_hits_remaining, float& current_damage, float penetration_power);
	bool SimulateFireBullet(FireBulletData* data, C_CSPlayer* local, CBaseCombatWeapon* weapon, C_CSPlayer* target, Vector& start_pos, const Vector& direction, float& current_damage);

	float GetDamage(C_CSPlayer* local, CBaseCombatWeapon* local_weapon, C_CSPlayer* target, Vector& start_pos, const Vector& end_pos);
};
extern CAutoWall * new_autowall;
