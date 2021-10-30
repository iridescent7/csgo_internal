#pragma once

#include "..\math\math.h"
#include "..\interfaces\vfunc.h"
#include "..\utils\utils.h" // sdk utils

#include "..\netvarmgr.h"
#include "..\..\globals.h"

#define NETVAR(type, name, table, prop) \
	type& name##() { \
        static unsigned long _##name = netvarmgr::GetOffset(hash(table), hash(prop)); \
        return *(type*)((unsigned long)this + _##name); \
    }

#define PNETVAR(type, name, table, prop) \
    type* name##() { \
        static unsigned long _##name = netvarmgr::GetOffset(hash(table), hash(prop)); \
        return (type*)((unsigned long)this + _##name); \
    }

#define NETPROP(name, table, prop) \
	RecvProp* name##() \
	{ \
		static auto prop_ptr = netvarmgr::GetNetvarProp(hash(table), hash(prop)); \
		return prop_ptr; \
	}

struct datamap_t;
class CAnimationLayer;
class CBasePlayerAnimState;
class CCSGOPlayerAnimState;
class C_BaseEntity;

class C_EconItemView
{
	using str_32 = char[32];
public:
	NETVAR(int, m_bInitialized, "DT_BaseAttributableItem", "m_bInitialized");
	NETVAR(int, m_iItemDefinitionIndex, "DT_BaseAttributableItem", "m_iItemDefinitionIndex");
	NETVAR(int, m_iEntityLevel, "DT_BaseAttributableItem", "m_iEntityLevel");
	NETVAR(int, m_iAccountID, "DT_BaseAttributableItem", "m_iAccountID");
	NETVAR(int, m_iItemIDLow, "DT_BaseAttributableItem", "m_iItemIDLow");
	NETVAR(int, m_iItemIDHigh, "DT_BaseAttributableItem", "m_iItemIDHigh");
	NETVAR(int, m_iEntityQuality, "DT_BaseAttributableItem", "m_iEntityQuality");

	NETVAR(str_32, m_szCustomName, "DT_BaseAttributableItem", "m_szCustomName");
};

class C_BaseEntity : public IClientEntity
{
public:
	static __forceinline C_BaseEntity* GetEntityByIndex(int index)
	{
		return static_cast<C_BaseEntity*>(g_entitylist->GetClientEntity(index));
	}

	static __forceinline C_BaseEntity* GetEntityFromHandle(CBaseHandle h)
	{
		return static_cast<C_BaseEntity*>(g_entitylist->GetClientEntityFromHandle(h));
	}

	NETVAR(int, m_nModelIndex, "DT_BaseEntity", "m_nModelIndex");
	NETVAR(int, m_iTeamNum, "DT_BaseEntity", "m_iTeamNum");

	NETVAR(Vector, m_vecOrigin, "DT_BaseEntity", "m_vecOrigin");
	NETVAR(Vector, m_vecAngles, "DT_BaseEntity", "m_vecAngles");

	NETVAR(bool, m_bShouldGlow, "DT_DynamicProp", "m_bShouldGlow");
	NETVAR(bool, m_bSpotted, "DT_BaseEntity", "m_bSpotted");

	NETVAR(CHandle<C_BasePlayer>, m_hOwnerEntity, "DT_BaseEntity", "m_hOwnerEntity");

	datamap_t* GetDataDescMap()
	{
		typedef datamap_t*(__thiscall *GetPredDescMapFn)(void*);
		return GetVFunc<GetPredDescMapFn>(this, 15)(this);
	}

	datamap_t* GetPredDescMap()
	{
		typedef datamap_t*(__thiscall *GetPredDescMapFn)(void*);
		return GetVFunc<GetPredDescMapFn>(this, 17)(this);
	}

	const matrix3x4_t& m_rgflCoordinateFrame()
	{
		static auto _m_rgflCoordinateFrame = netvarmgr::GetOffset(hash("DT_BaseEntity"), hash("m_CollisionGroup")) - 0x30;
		return *(matrix3x4_t*)(this + _m_rgflCoordinateFrame);
	}

	bool IsPlayer();
	bool IsLoot();
	bool IsWeapon();
	bool IsPlantedC4();
	bool IsDefuseKit();
};

class C_PlantedC4
{
public:
	NETVAR(bool, m_bBombTicking, "DT_PlantedC4", "m_bBombTicking");
	NETVAR(bool, m_bBombDefused, "DT_PlantedC4", "m_bBombDefused");

	NETVAR(float, m_flC4Blow, "DT_PlantedC4", "m_flC4Blow");
	NETVAR(float, m_flTimerLength, "DT_PlantedC4", "m_flTimerLength");
	NETVAR(float, m_flDefuseLength, "DT_PlantedC4", "m_flDefuseLength");
	NETVAR(float, m_flDefuseCountDown, "DT_PlantedC4", "m_flDefuseCountDown");

	NETVAR(CHandle<C_BasePlayer>, m_hBombDefuser, "DT_PlantedC4", "m_hBombDefuser");
};

class C_BaseAttributableItem : public C_BaseEntity
{
public:
	NETVAR(uint64_t, m_OriginalOwnerXuid, "DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
	NETVAR(int32_t, m_OriginalOwnerXuidLow, "DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
	NETVAR(int32_t, m_OriginalOwnerXuidHigh, "DT_BaseAttributableItem", "m_OriginalOwnerXuidHigh");
	NETVAR(int32_t, m_nFallbackStatTrak, "DT_BaseAttributableItem", "m_nFallbackStatTrak");
	NETVAR(int32_t, m_nFallbackPaintKit, "DT_BaseAttributableItem", "m_nFallbackPaintKit");
	NETVAR(int32_t, m_nFallbackSeed, "DT_BaseAttributableItem", "m_nFallbackSeed");
	NETVAR(float_t, m_flFallbackWear, "DT_BaseAttributableItem", "m_flFallbackWear");

	NETVAR(C_EconItemView, m_Item2, "DT_BaseAttributableItem", "m_Item");

	C_EconItemView& m_Item()
	{
		// Cheating. It should be this + m_Item netvar but then the netvars inside C_EconItemView wont work properly.
		// A real fix for this requires a rewrite of the netvar manager
		return *(C_EconItemView*)this;
	}
	void SetGloveModelIndex(int modelIndex);

};

class C_BaseWeaponWorldModel : public C_BaseEntity
{
public:
	NETVAR(int32_t, m_nModelIndex, "DT_BaseWeaponWorldModel", "m_nModelIndex");
};

class C_BaseCombatWeapon : public C_BaseAttributableItem
{
public:
	NETVAR(float_t, m_flNextPrimaryAttack, "DT_BaseCombatWeapon", "m_flNextPrimaryAttack");
	NETVAR(float_t, m_flNextSecondaryAttack, "DT_BaseCombatWeapon", "m_flNextSecondaryAttack");
	NETVAR(int32_t, m_iClip1, "DT_BaseCombatWeapon", "m_iClip1");
	NETVAR(int32_t, m_iClip2, "DT_BaseCombatWeapon", "m_iClip2");
	NETVAR(float_t, m_flRecoilIndex, "DT_WeaponCSBase", "m_flRecoilIndex");
	NETVAR(int32_t, m_iViewModelIndex, "DT_BaseCombatWeapon", "m_iViewModelIndex");
	NETVAR(int32_t, m_iWorldModelIndex, "DT_BaseCombatWeapon", "m_iWorldModelIndex");
	NETVAR(int32_t, m_iWorldDroppedModelIndex, "DT_BaseCombatWeapon", "m_iWorldDroppedModelIndex");
	NETVAR(bool, m_bPinPulled, "DT_BaseCSGrenade", "m_bPinPulled");
	NETVAR(float_t, m_fThrowTime, "DT_BaseCSGrenade", "m_fThrowTime");
	NETVAR(float_t, m_flPostponeFireReadyTime, "DT_BaseCombatWeapon", "m_flPostponeFireReadyTime");
	NETVAR(CHandle<C_BaseWeaponWorldModel>, m_hWeaponWorldModel, "DT_BaseCombatWeapon", "m_hWeaponWorldModel");

	CCSWeaponInfo* GetCSWeaponData();

	bool HasBullets();
	bool CanFire();
	bool IsGrenade();
	bool IsKnife();
	bool IsReloading();
	bool IsRifle();
	bool IsPistol();
	bool IsSniper();
	bool IsGun();

	float GetInaccuracy();
	float GetSpread();
	void UpdateAccuracyPenalty();

	CUtlVector<IRefCounted*>& m_CustomMaterials();
	bool* m_bCustomMaterialInitialized();
};

class C_BasePlayer : public C_BaseEntity
{
public:
	static __forceinline C_BasePlayer* GetPlayerByUserId(int id)
	{
		return static_cast<C_BasePlayer*>(GetEntityByIndex(g_engine->GetPlayerForUserID(id)));
	}
	static __forceinline C_BasePlayer* GetPlayerByIndex(int i)
	{
		return static_cast<C_BasePlayer*>(GetEntityByIndex(i));
	}

	NETVAR(bool, m_bHasDefuser, "DT_CSPlayer", "m_bHasDefuser");
	NETVAR(bool, m_bGunGameImmunity, "DT_CSPlayer", "m_bGunGameImmunity");
	NETVAR(int32_t, m_iShotsFired, "DT_CSPlayer", "m_iShotsFired");
	NETVAR(QAngle, m_angEyeAngles, "DT_CSPlayer", "m_angEyeAngles[0]");
	NETVAR(int, m_ArmorValue, "DT_CSPlayer", "m_ArmorValue");
	NETVAR(bool, m_bHasHeavyArmor, "DT_CSPlayer", "m_bHasHeavyArmor");
	NETVAR(bool, m_bHasHelmet, "DT_CSPlayer", "m_bHasHelmet");
	NETVAR(bool, m_bIsScoped, "DT_CSPlayer", "m_bIsScoped");;
	NETVAR(float, m_flLowerBodyYawTarget, "DT_CSPlayer", "m_flLowerBodyYawTarget");
	NETVAR(int32_t, m_iHealth, "DT_BasePlayer", "m_iHealth");
	NETVAR(int32_t, m_lifeState, "DT_BasePlayer", "m_lifeState");
	NETVAR(int32_t, m_fFlags, "DT_BasePlayer", "m_fFlags");
	NETVAR(int32_t, m_nTickBase, "DT_BasePlayer", "m_nTickBase");
	NETVAR(Vector, m_vecViewOffset, "DT_BasePlayer", "m_vecViewOffset[0]");
	NETVAR(QAngle, m_viewPunchAngle, "DT_BasePlayer", "m_viewPunchAngle");
	NETVAR(QAngle, m_aimPunchAngle, "DT_BasePlayer", "m_aimPunchAngle");
	NETVAR(CHandle<C_BaseViewModel>, m_hViewModel, "DT_BasePlayer", "m_hViewModel[0]");
	NETVAR(Vector, m_vecVelocity, "DT_BasePlayer", "m_vecVelocity[0]");
	NETVAR(float, m_flMaxspeed, "DT_BasePlayer", "m_flMaxspeed");
	NETVAR(CHandle<C_BasePlayer>, m_hObserverTarget, "DT_BasePlayer", "m_hObserverTarget");
	NETVAR(float, m_flFlashMaxAlpha, "DT_CSPlayer", "m_flFlashMaxAlpha");
	NETVAR(int32_t, m_nHitboxSet, "DT_BaseAnimating", "m_nHitboxSet");
	NETVAR(CHandle<C_BaseCombatWeapon>, m_hActiveWeapon, "DT_BaseCombatCharacter", "m_hActiveWeapon");
	NETVAR(int32_t, m_iAccount, "DT_CSPlayer", "m_iAccount");
	NETVAR(float, m_flFlashDuration, "DT_CSPlayer", "m_flFlashDuration");
	NETVAR(float, m_flSimulationTime, "DT_BaseEntity", "m_flSimulationTime");
	NETVAR(float, m_flCycle, "DT_ServerAnimationData", "m_flCycle");
	NETVAR(int, m_nSequence, "DT_BaseViewModel", "m_nSequence");
	PNETVAR(char, m_szLastPlaceName, "DT_BasePlayer", "m_szLastPlaceName");
	NETPROP(m_flLowerBodyYawTargetProp, "DT_CSPlayer", "m_flLowerBodyYawTarget");
	NETVAR(QAngle, m_angAbsAngles, "DT_BaseEntity", "m_angAbsAngles");
	NETVAR(Vector, m_angAbsOrigin, "DT_BaseEntity", "m_angAbsOrigin");
	NETVAR(float, m_flDuckSpeed, "DT_BaseEntity", "m_flDuckSpeed");
	NETVAR(float, m_flDuckAmount, "DT_BaseEntity", "m_flDuckAmount");

	PNETVAR(CHandle<C_BaseCombatWeapon>, m_hMyWeapons, "DT_BaseCombatCharacter", "m_hMyWeapons");
	PNETVAR(CHandle<C_BaseAttributableItem>, m_hMyWearables, "DT_BaseCombatCharacter", "m_hMyWearables");

	float* m_flPoseParameter() const
	{
		static int _m_flPoseParameter = netvarmgr::GetOffset(hash("DT_BaseAnimating"), hash("m_flPoseParameter"));
		return *(float**)((uintptr_t)this + _m_flPoseParameter); // std::array<float, 24>
	}

	CUserCmd*& m_pCurrentCommand();

	void InvalidateBoneCache();
	int  GetNumAnimOverlays();

	CAnimationLayer *GetAnimOverlays();
	CAnimationLayer *GetAnimOverlay(int i);

	int GetSequenceActivity(int sequence);
	CCSGOPlayerAnimState* GetPlayerAnimState();

	static void UpdateAnimationState(CCSGOPlayerAnimState *state, QAngle angle);
	static void ResetAnimationState(CCSGOPlayerAnimState *state);

	void CreateAnimationState(CCSGOPlayerAnimState *state);

	float& m_surfaceFriction()
	{
		static unsigned long _m_surfaceFriction = datamap::FindInDataMap(GetPredDescMap(), hash("m_surfaceFriction"));
		return *(float*)((unsigned long)this + _m_surfaceFriction);
	}

	Vector& m_vecBaseVelocity()
	{
		static unsigned long _m_vecBaseVelocity = datamap::FindInDataMap(GetPredDescMap(), hash("m_vecBaseVelocity"));
		return *(Vector*)((unsigned long)this + _m_vecBaseVelocity);
	}

	float& m_flMaxspeed2()
	{
		static unsigned long _m_flMaxspeed = datamap::FindInDataMap(GetPredDescMap(), hash("m_flMaxspeed"));
		return *(float*)((unsigned long)this + _m_flMaxspeed);
	}

	Vector        GetEyePos();
	player_info_t GetPlayerInfo();
	bool          IsAlive();
	bool		  IsFlashed();
	bool          HasC4();
	Vector        GetHitboxPos(int hitbox_id);
	mstudiobbox_t* GetHitbox(int hitbox_id);
	bool          GetHitboxPos(int hitbox, Vector &output);
	Vector        GetBonePos(int bone);
	bool          CanSeePlayer(C_BasePlayer* player, int hitbox);
	bool          CanSeePlayer(C_BasePlayer* player, const Vector& pos);

	void UpdateClientSideAnimation();

	int		m_nMoveType();
	float_t m_flSpawnTime();

	QAngle* GetViewangles();
	float	GetFlashBangTime();
};

class C_BaseViewModel : public C_BaseEntity
{
public:
	NETVAR(int32_t, m_nModelIndex, "DT_BaseViewModel", "m_nModelIndex");
	NETVAR(int32_t, m_nViewModelIndex, "DT_BaseViewModel", "m_nViewModelIndex");
	NETVAR(CHandle<C_BaseCombatWeapon>, m_hWeapon, "DT_BaseViewModel", "m_hWeapon");
	NETVAR(CHandle<C_BasePlayer>, m_hOwner, "DT_BaseViewModel", "m_hOwner");
	NETPROP(m_nSequenceProp, "DT_BaseViewModel", "m_nSequence");

	void SendViewModelMatchingSequence(int sequence);
};

class DT_CSPlayerResource
{
public:
	PNETVAR(int32_t, m_nActiveCoinRank, "DT_CSPlayerResource", "m_nActiveCoinRank");
	PNETVAR(int32_t, m_nMusicID, "DT_CSPlayerResource", "m_nMusicID");
	PNETVAR(int32_t, m_nPersonaDataPublicLevel, "DT_CSPlayerResource", "m_nPersonaDataPublicLevel");
	PNETVAR(int32_t, m_nPersonaDataPublicCommendsLeader, "DT_CSPlayerResource", "m_nPersonaDataPublicCommendsLeader");
	PNETVAR(int32_t, m_nPersonaDataPublicCommendsTeacher, "DT_CSPlayerResource", "m_nPersonaDataPublicCommendsTeacher");
	PNETVAR(int32_t, m_nPersonaDataPublicCommendsFriendly, "DT_CSPlayerResource", "m_nPersonaDataPublicCommendsFriendly");
	PNETVAR(int32_t, m_iCompetitiveRanking, "DT_CSPlayerResource", "m_iCompetitiveRanking");
	PNETVAR(int32_t, m_iCompetitiveWins, "DT_CSPlayerResource", "m_iCompetitiveWins");
	PNETVAR(int32_t, m_iPlayerVIP, "DT_CSPlayerResource", "m_iPlayerVIP");
	PNETVAR(int32_t, m_iMVPs, "DT_CSPlayerResource", "m_iMVPs");
	PNETVAR(int32_t, m_iScore, "DT_CSPlayerResource", "m_iScore");
};