#include "Offsets.h"
#include "Globals.h"
#include "Interfaces.h"

namespace O
{
	DWORD m_iHealth;
	DWORD m_fFlags;
	DWORD m_vecVelocity;
	DWORD m_flLowerBodyYawTarget;
	DWORD deadflag;
	DWORD m_vecOrigin;
	DWORD m_iTeamNum;
	DWORD m_nTickBase;
	DWORD m_bDormant;
	DWORD animstate;
	DWORD m_Collision;
	DWORD m_angEyeAngles;
	DWORD m_flSimulationTime;
	DWORD m_vecViewOffset;
	DWORD m_dwBoneMatrix;
	DWORD m_aimPunchAngle;
	DWORD m_bGunGameImmunity;
	DWORD m_nForceBone;
	DWORD m_flPoseParameter;
	DWORD m_flNextPrimaryAttack;
	DWORD m_flNextAttack;
	DWORD m_hActiveWeapon;
	DWORD m_ArmorValue;
	DWORD m_bHasHelmet;
	DWORD m_iObserverMode;
	DWORD m_bIsScoped;
	DWORD m_iAccount;
	DWORD m_iPlayerC4;
	DWORD m_iItemDefinitionIndex;
	DWORD m_lifeState;
	DWORD m_flPostponeFireReadyTime;
	DWORD m_fThrowTime;
	DWORD m_bPinPulled;
	DWORD m_nMoveType;
	DWORD m_iItemIDHigh;
	DWORD m_iEntityQuality;
	DWORD m_iAccountID;
	DWORD m_flFallbackWear;
	DWORD m_nFallbackSeed;
	DWORD m_nFallbackStatTrak;
	DWORD m_nFallbackPaintKit;
	DWORD m_hMyWearables;
	DWORD m_nModelIndex;
	DWORD m_hOwner;
	DWORD m_iViewModelIndex;
	DWORD m_iWorldModelIndex;
	DWORD m_hWeaponWorldModel;
	DWORD m_Item;
	DWORD m_OriginalOwnerXuidLow;
	DWORD m_OriginalOwnerXuidHigh;
	DWORD m_iShotsFired;
	DWORD m_iClip1;
	DWORD m_hMyWeapons;
	DWORD animlayer;
	DWORD m_flFlashDuration;
	DWORD m_viewPunchAngle;
	DWORD m_flDuckAmount;
	DWORD m_hObserverTarget;

	DWORD invalidateBoneCache;
	DWORD linegoesthrusmoke;

	DWORD isBreakableEntity;
	DWORD initKeyValues;
	DWORD loadFromBuffer;
	DWORD getSequenceActivity;
	DWORD serverRankRevealAll;
	DWORD sendClantagChanged;

	DWORD DrawSetColor_ScopeArc;
	DWORD DrawSetColor_ScopeLens;

	DWORD deviceStructureAddress;

	int* nPredictionRandomSeed;

	void SetupOffsets()
	{
		G::Netvars.Dump();

		deadflag = G::Netvars.GetOffset("DT_BasePlayer", "pl", "deadflag");

		m_iItemIDHigh = G::Netvars.GetOffset("DT_BaseAttributableItem", "m_AttributeManager", "m_Item", "m_iItemIDHigh");
		m_iEntityQuality = G::Netvars.GetOffset("DT_BaseAttributableItem", "m_AttributeManager", "m_Item", "m_iEntityQuality");

		m_Item = G::Netvars.GetOffset("DT_BaseAttributableItem", "m_AttributeManager", "m_Item");
		m_OriginalOwnerXuidLow = G::Netvars.GetOffset("DT_BaseAttributableItem", "m_OriginalOwnerXuidLow");
		m_OriginalOwnerXuidHigh = G::Netvars.GetOffset("DT_BaseAttributableItem", "m_OriginalOwnerXuidHigh");

		m_iAccountID = G::Netvars.GetOffset("DT_BaseAttributableItem", "m_AttributeManager", "m_Item", "m_iAccountID");
		m_flFallbackWear = G::Netvars.GetOffset("DT_BaseAttributableItem", "m_flFallbackWear");
		m_nFallbackSeed = G::Netvars.GetOffset("DT_BaseAttributableItem", "m_nFallbackSeed");
		m_nFallbackStatTrak = G::Netvars.GetOffset("DT_BaseAttributableItem", "m_nFallbackStatTrak");
		m_nFallbackPaintKit = G::Netvars.GetOffset("DT_BaseAttributableItem", "m_nFallbackPaintKit");

		m_nModelIndex = G::Netvars.GetOffset("DT_BaseViewModel", "m_nModelIndex");
		m_hOwner = G::Netvars.GetOffset("DT_BaseViewModel", "m_hOwner");

		m_iViewModelIndex = G::Netvars.GetOffset("DT_BaseCombatWeapon", "m_iViewModelIndex");
		m_iWorldModelIndex = G::Netvars.GetOffset("DT_BaseCombatWeapon", "m_iWorldModelIndex");
		m_hWeaponWorldModel = G::Netvars.GetOffset("DT_BaseCombatWeapon", "m_hWeaponWorldModel");
		m_flLowerBodyYawTarget = G::Netvars.GetOffset("DT_CSPlayer", "m_flLowerBodyYawTarget");
		m_vecOrigin = G::Netvars.GetOffset("DT_BaseEntity", "m_vecOrigin");

		m_nTickBase = G::Netvars.GetOffset("DT_BasePlayer", "localdata", "m_nTickBase");
		m_Collision = G::Netvars.GetOffset("DT_BaseEntity", "m_Collision");
		m_flSimulationTime = G::Netvars.GetOffset("DT_BaseEntity", "m_flSimulationTime");

		m_vecViewOffset = G::Netvars.GetOffset("DT_BasePlayer", "localdata", "m_vecViewOffset[0]");
		m_nForceBone = G::Netvars.GetOffset("DT_BaseAnimating", "m_nForceBone");
		m_flNextAttack = G::Netvars.GetOffset("DT_BaseCombatCharacter", "bcc_localdata", "m_flNextAttack");
		m_hActiveWeapon = G::Netvars.GetOffset("DT_BaseCombatCharacter", "m_hActiveWeapon");
		m_iObserverMode = G::Netvars.GetOffset("DT_BasePlayer", "m_iObserverMode");
		m_iItemDefinitionIndex = G::Netvars.GetOffset("DT_BaseAttributableItem", "m_AttributeManager", "m_Item", "m_iItemDefinitionIndex");

		m_flPostponeFireReadyTime = G::Netvars.GetOffset("DT_WeaponCSBase", "m_flPostponeFireReadyTime");
		m_fThrowTime = G::Netvars.GetOffset("DT_BaseCSGrenade", "m_fThrowTime");
		m_viewPunchAngle = G::Netvars.GetOffset("DT_BasePlayer", "m_viewPunchAngle");
		m_iHealth = G::Netvars.GetOffset("DT_BasePlayer", "m_iHealth");;

		m_aimPunchAngle = G::Netvars.GetOffset("DT_BasePlayer", "localdata", "m_Local", "m_aimPunchAngle");
		m_hMyWearables = G::Netvars.GetOffset("DT_BaseCombatCharacter", "m_hMyWearables");
		m_iShotsFired = G::Netvars.GetOffset("DT_CSPlayer","cslocaldata", "m_iShotsFired");
		m_fFlags = G::Netvars.GetOffset("DT_BasePlayer", "m_fFlags");

		m_hObserverTarget = G::Netvars.GetOffset("DT_BasePlayer", "m_hObserverTarget");

		m_vecVelocity = G::Netvars.GetOffset("DT_BasePlayer", "localdata", "m_vecVelocity[0]");
		m_angEyeAngles = G::Netvars.GetOffset("DT_CSPlayer", "m_angEyeAngles[0]");
		m_bGunGameImmunity = G::Netvars.GetOffset("DT_CSPlayer", "m_bGunGameImmunity");
		m_flPoseParameter = G::Netvars.GetOffset("DT_BaseAnimating", "m_flPoseParameter");

		m_ArmorValue = G::Netvars.GetOffset("DT_CSPlayer", "m_ArmorValue");
		m_bHasHelmet = G::Netvars.GetOffset("DT_CSPlayer", "m_bHasHelmet");
		m_bIsScoped = G::Netvars.GetOffset("DT_CSPlayer", "m_bIsScoped");

		//m_iAccount = G::Netvars.GetOffset("DT_CSPlayer", "m_iAccount"); // no idea whats this

		m_iTeamNum = G::Netvars.GetOffset("DT_BaseEntity", "m_iTeamNum");
		m_lifeState = G::Netvars.GetOffset("DT_BasePlayer", "m_lifeState");

		m_hMyWeapons = G::Netvars.GetOffset("DT_BasePlayer", "m_hActiveWeapon") - 0x100;
		m_flNextPrimaryAttack = G::Netvars.GetOffset("DT_BaseCombatWeapon", "LocalActiveWeaponData", "m_flNextPrimaryAttack");
		m_iClip1 = G::Netvars.GetOffset("DT_BaseCombatWeapon", "m_iClip1");

		m_flFlashDuration = G::Netvars.GetOffset("DT_CSPlayer", "m_flFlashDuration");
		m_viewPunchAngle = G::Netvars.GetOffset("DT_BasePlayer", "localdata", "m_Local", "m_viewPunchAngle");

		m_flDuckAmount = G::Netvars.GetOffset("DT_BasePlayer", "m_flDuckAmount");
		m_flNextAttack = G::Netvars.GetOffset("DT_BaseCombatCharacter", "bcc_localdata", "m_flNextAttack");

		m_iShotsFired =G::Netvars.GetOffset("DT_CSPlayer", "cslocaldata", "m_iShotsFired");

		invalidateBoneCache = U::FindSignature("client.dll", "80 3D ? ? ? ? ? 74 16 A1 ? ? ? ? 48 C7 81");
		linegoesthrusmoke   = U::FindSignature("client.dll", "55 8B EC 83 EC 08 8B 15 ? ? ? ? 0F 57 C0");

		isBreakableEntity   = U::FindSignature("client.dll", "55 8B EC 51 56 8B F1 85 F6 74 68 83 BE");
		initKeyValues       = U::FindSignature("client.dll", "55 8B EC 51 33 C0 C7 45");
		loadFromBuffer      = U::FindSignature("client.dll", "55 8B EC 83 EC 44 53 56 57 8B F9 89 7D F8 FF 15 ? ? ? ? 8B F0 B9");
		getSequenceActivity = U::FindSignature("client.dll", "55 8B EC 53 8B 5D 08 56 8B F1 83");
		serverRankRevealAll = U::FindSignature("client.dll", "55 8B EC 8B 0D ? ? ? ? 85 C9 75 ? A1 ? ? ? ? 68 ? ? ? ? 8B 08 8B 01 FF 50 ? 85 C0 74 ? 8B C8 E8 ? ? ? ? 8B C8 EB ? 33 C9 89 0D ? ? ? ? 8B 45 ? FF 70 ? E8 ? ? ? ? B0 ? 5D");
		
		sendClantagChanged  = U::FindSignature("engine.dll", "53 56 57 8B DA 8B F9 FF");

		DrawSetColor_ScopeArc  = U::FindSignature("client.dll", "6A 00 FF 50 3C 8B 0D ? ? ? ? FF B7 ? ? ? ?") + 5;
		DrawSetColor_ScopeLens = U::FindSignature("client.dll", "FF 50 3C 8B 4C 24 20") + 3;

		deviceStructureAddress = **(DWORD**)(U::FindSignature("shaderapidx9.dll", "A1 ?? ?? ?? ?? 50 8B 08 FF 51 0C") + 1);

		m_bDormant = 0xED;
		animstate = 0x3900;
		m_dwBoneMatrix = 0x26A8;
		m_nMoveType = 0x25C;
		animlayer = 0x2980;

		//nPredictionRandomSeed = *(int**)U::FindSignature("client.dll", "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04") + 0x2;
	}
}