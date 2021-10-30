#include "Autowall.h"
#include "..\Globals.h"

void CAutoWall::ClipTraceToPlayer(C_CSPlayer* entity, const Vector& start, const Vector& end, unsigned int mask, trace_t* tr)
{
	Vector mins = entity->GetCollideable()->OBBMins(),
		maxs = entity->GetCollideable()->OBBMaxs();

	Vector dir(end - start);
	dir.NormalizeInPlace();

	Vector
		center = (maxs + mins) / 2,
		pos(center + entity->m_vecOrigin());

	Vector to = pos - start;

	float range_along = dir.Dot(to);
	float range;

	if (range_along < 0.f)
	{
		range = -to.Length();
	}
	else if (range_along > dir.Length())
	{
		range = -(pos - end).Length();
	}
	else
	{
		auto ray(pos - (dir * range_along + start));
		range = ray.Length();
	}

	if (range <= 60.f)
	{
		trace_t trace;

		Ray_t ray;
		ray.Init(start, end);

		I::Trace->ClipRayToEntity(ray, mask, entity, &trace);

		if (tr->fraction > trace.fraction)
			*tr = trace;
	}
}

void CAutoWall::GetBulletTypeParameters(float& max_range, float& max_distance, char* bullet_type)
{
	if (C::sv_penetration_type->GetBool())
	{
		max_range = 35.0;
		max_distance = 3000.0;
	}
	else
	{
		//Play tribune to framerate. Thanks, stringcompare
		//Regardless I doubt anyone will use the old penetration system anyway; so it won't matter much.
		if (!strcmp(bullet_type, "BULLET_PLAYER_338MAG"))
		{
			max_range = 45.0;
			max_distance = 8000.0;
		}
		if (!strcmp(bullet_type, "BULLET_PLAYER_762MM"))
		{
			max_range = 39.0;
			max_distance = 5000.0;
		}
		if (!strcmp(bullet_type, "BULLET_PLAYER_556MM") || !strcmp(bullet_type, "BULLET_PLAYER_556MM_SMALL") || !strcmp(bullet_type, "BULLET_PLAYER_556MM_BOX"))
		{
			max_range = 35.0;
			max_distance = 4000.0;
		}
		if (!strcmp(bullet_type, "BULLET_PLAYER_57MM"))
		{
			max_range = 30.0;
			max_distance = 2000.0;
		}
		if (!strcmp(bullet_type, "BULLET_PLAYER_50AE"))
		{
			max_range = 30.0;
			max_distance = 1000.0;
		}
		if (!strcmp(bullet_type, "BULLET_PLAYER_357SIG") || !strcmp(bullet_type, "BULLET_PLAYER_357SIG_SMALL") || !strcmp(bullet_type, "BULLET_PLAYER_357SIG_P250") || !strcmp(bullet_type, "BULLET_PLAYER_357SIG_MIN"))
		{
			max_range = 25.0;
			max_distance = 800.0;
		}
		if (!strcmp(bullet_type, "BULLET_PLAYER_9MM"))
		{
			max_range = 21.0;
			max_distance = 800.0;
		}
		if (!strcmp(bullet_type, "BULLET_PLAYER_45ACP"))
		{
			max_range = 15.0;
			max_distance = 500.0;
		}
		if (!strcmp(bullet_type, "BULLET_PLAYER_BUCKSHOT"))
		{
			max_range = 0.0;
			max_distance = 0.0;
		}
	}
}

bool CAutoWall::IsBreakableEntity(C_CSPlayer* entity)
{
	if (!entity || !entity->EntIndex())
		return false;

	char* take_damage = (char*)(entity + *(unsigned int*)(O::isBreakableEntity + 38));
	char  take_damage_backup = *take_damage;

	auto cclass = entity->GetClientClass();

	if ((cclass->m_pNetworkName[1] == 'B' && cclass->m_pNetworkName[9] == 'e' && cclass->m_pNetworkName[10] == 'S' && cclass->m_pNetworkName[16] == 'e')
		|| (cclass->m_pNetworkName[1] != 'B' || cclass->m_pNetworkName[5] != 'D'))
		*take_damage = DAMAGE_YES;

	using IsBreakableEntityFn = bool(__fastcall*)(C_CSPlayer*);

	bool breakable = ((IsBreakableEntityFn)O::isBreakableEntity)(entity);
	*take_damage = take_damage_backup;

	return breakable;

}

void CAutoWall::ScaleDamage(C_CSPlayer* entity, int hitgroup, float weapon_armor_ratio, float& current_damage)
{
	auto IsArmored = [&entity, &hitgroup]()-> bool
	{
		switch (hitgroup)
		{
		case HITGROUP_HEAD:
			return entity->m_bHasHelmet();
		case HITGROUP_GENERIC:
		case HITGROUP_CHEST:
		case HITGROUP_STOMACH:
		case HITGROUP_LEFTARM:
		case HITGROUP_RIGHTARM:
			return true;
		default:
			return false;
		}
	};

	switch (hitgroup)
	{
	case HITGROUP_HEAD:
		current_damage *= 4.f;
		break;
	case HITGROUP_STOMACH:
		current_damage *= 1.25f;
		break;
	case HITGROUP_LEFTLEG:
	case HITGROUP_RIGHTLEG:
		current_damage *= 0.75f;
		break;
	}

	int armor_value = entity->m_ArmorValue();

	if (armor_value > 0 && IsArmored())
	{
		float bonus_value = 1.f, armor_bonus_ratio = 0.5f,
			armor_ratio = weapon_armor_ratio / 2.f;

		float new_damage = current_damage * armor_ratio;

		if (((current_damage - new_damage) * (bonus_value * armor_bonus_ratio)) > armor_value)
			new_damage = current_damage - (armor_value / armor_bonus_ratio);

		current_damage = new_damage;
	}
}

bool CAutoWall::TraceToExit(trace_t& enter_trace, trace_t& exit_trace, const Vector& start_position, const Vector& direction)
{
	Vector start, end;
	float  max_distance = 90.f, ray_extension = 4.f, current_distance = 0;
	int    first_contents = 0;

	while (current_distance <= max_distance)
	{
		//Add extra distance to our ray
		current_distance += ray_extension;

		//Multiply the direction vector to the distance so we go outwards, add our position to it.
		start = start_position + direction * current_distance;

		if (!first_contents)
			first_contents = I::Trace->GetPointContents(start, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr);

		int point_contents = I::Trace->GetPointContents(start, MASK_SHOT_HULL | CONTENTS_HITBOX, nullptr);

		if (!(point_contents & MASK_SHOT_HULL) || point_contents & CONTENTS_HITBOX && point_contents != first_contents)
		{
			//Let's setup our end position by deducting the direction by the extra added distance
			end = start - (direction * ray_extension);

			CTraceFilter filter;
			filter.pSkip = nullptr;

			//Let's cast a ray from our start pos to the end pos
			U::TraceLine(start, end, MASK_SHOT_HULL | CONTENTS_HITBOX, &filter, &exit_trace);

			//Let's check if a hitbox is in-front of our enemy and if they are behind of a solid wall
			if (exit_trace.startsolid && exit_trace.surface.flags & SURF_HITBOX)
			{
				CTraceFilter filter;
				filter.pSkip = exit_trace.m_pEnt;

				U::TraceLine(start, start_position, MASK_SHOT_HULL, &filter, &exit_trace);

				if (exit_trace.DidHit() && !exit_trace.startsolid)
				{
					start = exit_trace.endpos;
					return true;
				}

				continue;
			}

			//Can we hit? Is the wall solid?
			if (exit_trace.DidHit() && !exit_trace.startsolid)
			{

				//Is the wall a breakable? If so, let's shoot through it.
				if (IsBreakableEntity(enter_trace.m_pEnt) && IsBreakableEntity(exit_trace.m_pEnt))
					return true;

				if (enter_trace.surface.flags & SURF_NODRAW || !(exit_trace.surface.flags & SURF_NODRAW) && (exit_trace.plane.normal.Dot(direction) <= 1.f))
				{
					float multAmount = exit_trace.fraction * 4.f;
					start -= direction * multAmount;
					return true;
				}

				continue;
			}

			if (!exit_trace.DidHit() || exit_trace.startsolid)
			{
				if (enter_trace.DidHit() && IsBreakableEntity(enter_trace.m_pEnt))
				{
					exit_trace = enter_trace;
					exit_trace.endpos = start + direction;
					return true;
				}

				continue;
			}
		}
	}
	return false;
}

bool CAutoWall::HandleBulletPenetration(FireBulletData* data, CSWeaponInfo* weapon_data, trace_t& enter_trace, Vector& start_pos, const Vector& direction, int& possible_hits_remaining, float& current_damage, float penetration_power)
{
	trace_t exit_trace;

	surfacedata_t* enter_surface_data = I::PhysSurface->GetSurfaceData(enter_trace.surface.surfaceProps);
	int            enter_material     = enter_surface_data->game.material;

	float enter_surf_penetration_modifier = enter_surface_data->game.flPenetrationModifier;
	float enter_damage_modifier			  = enter_surface_data->game.flDamageModifier;

	float thickness, modifier, lost_damage, 
		final_damage_modifier,
		combined_penetration_modifier;

	bool is_solid_surf = ((enter_trace.contents >> 3) & CONTENTS_SOLID);
	bool is_light_surf = ((enter_trace.surface.flags >> 7) & SURF_LIGHT);

	if (possible_hits_remaining <= 0
		//Test for "DE_CACHE/DE_CACHE_TELA_03" as the entering surface and "CS_ITALY/CR_MISCWOOD2B" as the exiting surface.
		//Fixes a wall in de_cache which seems to be broken in some way. Although bullet penetration is not recorded to go through this wall
		//Decals can be seen of bullets within the glass behind of the enemy. Hacky method, but works.
		//You might want to have a check for this to only be activated on de_cache.
		|| (!strcmp(enter_trace.surface.name, "DE_CACHE/DE_CACHE_TELA_03") && !strcmp(exit_trace.surface.name, "CS_ITALY/CR_MISCWOOD2B"))
		|| (!possible_hits_remaining && !is_light_surf && !is_solid_surf && enter_material != CHAR_TEX_GRATE && enter_material != CHAR_TEX_GLASS)
		|| weapon_data->penetration <= 0.f
		|| !TraceToExit(enter_trace, exit_trace, enter_trace.endpos, direction)
		&& !(I::Trace->GetPointContents(enter_trace.endpos, MASK_SHOT_HULL, NULL) & MASK_SHOT_HULL))
		return false;

	surfacedata_t* exit_surface_data = I::PhysSurface->GetSurfaceData(exit_trace.surface.surfaceProps);

	int   exit_material					 = exit_surface_data->game.material;
	float exit_surf_penetration_modifier = exit_surface_data->game.flPenetrationModifier;
	float exit_damage_modifier			 = exit_surface_data->game.flDamageModifier;

	//Are we using the newer penetration system?
	if (C::sv_penetration_type->GetBool())
	{
		if (enter_material == CHAR_TEX_GRATE || enter_material == CHAR_TEX_GLASS)
		{
			combined_penetration_modifier = 3.f;
			final_damage_modifier = 0.05f;
		}
		else if (is_solid_surf || is_light_surf)
		{
			combined_penetration_modifier = 1.f;
			final_damage_modifier = 0.16f;
		}
		else if (enter_material == CHAR_TEX_FLESH)
		{
			//Look's like you aren't shooting through your teammate today
			if (C::ff_damage_bullet_penetration->GetFloat() == 0.f)
				return false;

			// todo: 

			//Let's shoot through teammates and get kicked for teamdmg! Whatever, atleast we did damage to the enemy. I call that a win.
			combined_penetration_modifier = C::ff_damage_bullet_penetration->GetFloat();
			final_damage_modifier = 0.16f;
		}
		else
		{
			combined_penetration_modifier = (enter_surf_penetration_modifier + exit_surf_penetration_modifier) / 2.f;
			final_damage_modifier = 0.16f;
		}

		//Do our materials line up?
		if (enter_material == exit_material)
		{
			if (exit_material == CHAR_TEX_CARDBOARD || exit_material == CHAR_TEX_WOOD)
				combined_penetration_modifier = 3.f;
			else if (exit_material == CHAR_TEX_PLASTIC)
				combined_penetration_modifier = 2.f;
		}

		//Calculate thickness of the wall by getting the length of the range of the trace and squaring
		thickness = (exit_trace.endpos - enter_trace.endpos).LengthSqr();
		modifier = fmaxf(1.f / combined_penetration_modifier, 0.f);

		//This calculates how much damage we've lost depending on thickness of the wall, our penetration, damage, and the modifiers set earlier
		lost_damage = fmaxf(
			((modifier * thickness) / 24.f)
			+ ((current_damage * final_damage_modifier)
				+ (fmaxf(3.75f / penetration_power, 0.f) * 3.f * modifier)), 0.f);

		// Did we lose too much damage?
		if (lost_damage > current_damage)
			return false;

		// We can't use any of the damage that we've lost
		if (lost_damage > 0.f)
			current_damage -= lost_damage;

		// Do we still have enough damage to deal?
		if (current_damage < 1.f)
			return false;

		start_pos = exit_trace.endpos;
		--possible_hits_remaining;

		return true;
	}
	else //Legacy penetration system
	{
		combined_penetration_modifier = 1.f;

		if (is_solid_surf || is_light_surf)
			final_damage_modifier = 0.99f; //Good meme :^)
		else
		{
			final_damage_modifier = fminf(enter_damage_modifier, exit_damage_modifier);
			combined_penetration_modifier = fminf(enter_surf_penetration_modifier, exit_surf_penetration_modifier);
		}

		if (enter_material == exit_material && (exit_material == CHAR_TEX_METAL || exit_material == CHAR_TEX_WOOD))
			combined_penetration_modifier += combined_penetration_modifier;

		thickness = (exit_trace.endpos - enter_trace.endpos).LengthSqr();

		if (sqrt(thickness) <= combined_penetration_modifier * penetration_power)
		{
			current_damage *= final_damage_modifier;
			start_pos = exit_trace.endpos;
			--possible_hits_remaining;

			return true;
		}

		return false;
	}
}

bool CAutoWall::SimulateFireBullet(FireBulletData* data, C_CSPlayer* local, CBaseCombatWeapon* weapon, C_CSPlayer* target, Vector& start_pos, const Vector& direction, float& current_damage)
{
	if (!weapon)
		return false;

	float current_distance = 0.f,
		penetration_power, // Current bullet travel Power
		penetration_distance, // penetrate Distance
		max_range, // penetrate Range
		ray_extension = 40.f; // Amount to extend ray by

	auto weapon_data = weapon->GetCSWeaponData();

	if (!weapon_data)
		return false;

	max_range = weapon_data->range;

	GetBulletTypeParameters(penetration_power, penetration_distance, weapon_data->bullet_type);

	if (C::sv_penetration_type->GetBool())
		penetration_power = weapon_data->penetration;

	//This gets set in FX_Firebullets to 4 as a pass-through value.
	//CS:GO has a maximum of 4 surfaces a bullet can pass-through before it 100% stops.
	//Excerpt from Valve: https://steamcommunity.com/sharedfiles/filedetails/?id=275573090
	//"The total number of surfaces any bullet can penetrate in a single flight is capped at 4." -CS:GO Official
	int possibleHitsRemaining = 4;

	//Set our current damage to what our gun's initial damage reports it will do
	current_damage = weapon_data->damage;

	trace_t enter_trace;

	//We should be skipping localplayer when casting a ray to players.
	CTraceFilter filter;
	filter.pSkip = local;

	//If our damage is greater than (or equal to) 1, and we can shoot, let's shoot.
	while (possibleHitsRemaining > 0 && current_damage >= 1.f)
	{
		// Calculate max bullet range
		max_range -= current_distance;

		// Create endpoint of bullet
		Vector end = start_pos + direction * max_range;

		U::TraceLine(start_pos, end, MASK_SHOT_HULL | CONTENTS_HITBOX, &filter, &enter_trace);
		ClipTraceToPlayer(target, start_pos, end + direction * ray_extension, MASK_SHOT_HULL | CONTENTS_HITBOX, &enter_trace);

		// We have to do this *after* tracing to the player.
		surfacedata_t* enter_surface_data			   = I::PhysSurface->GetSurfaceData(enter_trace.surface.surfaceProps);
		float		   enter_surf_penetration_modifier = enter_surface_data->game.flPenetrationModifier;
		int			   enter_material				   = enter_surface_data->game.material;

		// "Fraction == 1" means that we didn't hit anything. We don't want that- so let's break on it.
		if (enter_trace.fraction == 1.f)
			break;

		// calculate the damage based on the distance the bullet traveled.
		current_distance += enter_trace.fraction * max_range;

		// Let's make our damage drops off the further away the bullet is.
		current_damage *= pow(weapon_data->range_modifier, (current_distance / 500.f));

		// Sanity checking / Can we actually shoot through?
		if (current_distance > penetration_distance && weapon_data->penetration > 0.f || enter_surf_penetration_modifier < 0.1f)
			break;

		if ( (enter_trace.m_pEnt == target) && (enter_trace.hitgroup != HITGROUP_GEAR && enter_trace.hitgroup != HITGROUP_GENERIC) )
		{
			ScaleDamage(enter_trace.m_pEnt, enter_trace.hitgroup, weapon_data->armor_ratio, current_damage);
			return true;
		}

		//Calling HandleBulletPenetration here reduces our penetrationCounter, and if it returns true, we can't shoot through it.
		if (!HandleBulletPenetration(data, weapon_data, enter_trace, start_pos, direction, possibleHitsRemaining, current_damage, penetration_power))
			break;
	}

	return false;
}

float CAutoWall::GetDamage(C_CSPlayer* local, CBaseCombatWeapon* local_weapon, C_CSPlayer* target, Vector& start_pos, const Vector& end_pos)
{
	FireBulletData data;
	data.current_position = start_pos;
	data.filter.pSkip = local;

	QAngle angle = M::CalcAngle(start_pos, end_pos);

	Vector direction;
	M::AngleVectors(angle, &direction);

	direction.NormalizeInPlace();

	float current_damage;
	bool  succeed = SimulateFireBullet(&data, local, local_weapon, target, start_pos, direction, current_damage);

	if (succeed)
		return current_damage;
	else
		return -1.f;
}
