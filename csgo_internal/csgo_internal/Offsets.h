#pragma once
#include <Windows.h>
#include "SDK\ConVar.h"

namespace O
{
	extern DWORD m_iHealth;
	extern DWORD m_fFlags;
	extern DWORD m_vecVelocity;
	extern DWORD m_flLowerBodyYawTarget;
	extern DWORD deadflag;
	extern DWORD m_vecOrigin;
	extern DWORD m_iTeamNum;
	extern DWORD m_nTickBase;
	extern DWORD m_bDormant;
	extern DWORD animstate;
	extern DWORD m_Collision;
	extern DWORD m_angEyeAngles;
	extern DWORD m_flSimulationTime;
	extern DWORD m_vecViewOffset;
	extern DWORD m_dwBoneMatrix;
	extern DWORD m_aimPunchAngle;
	extern DWORD m_bGunGameImmunity;
	extern DWORD m_nForceBone;
	extern DWORD m_flPoseParameter;
	extern DWORD m_flNextPrimaryAttack;
	extern DWORD m_flNextAttack;
	extern DWORD m_hActiveWeapon;
	extern DWORD m_ArmorValue;
	extern DWORD m_bHasHelmet;
	extern DWORD m_iObserverMode;
	extern DWORD m_bIsScoped;
	extern DWORD m_iAccount;
	extern DWORD m_iPlayerC4;
	extern DWORD m_iItemDefinitionIndex;
	extern DWORD m_lifeState;
	extern DWORD m_flPostponeFireReadyTime;
	extern DWORD m_fThrowTime;
	extern DWORD m_bPinPulled;
	extern DWORD m_nMoveType;
	extern DWORD m_viewPunchAngle;
	extern DWORD m_iShotsFired;
	extern DWORD m_bClientSideAnimation;
	extern DWORD m_iItemIDHigh;
	extern DWORD m_iEntityQuality;
	extern DWORD m_iAccountID;
	extern DWORD m_flFallbackWear;
	extern DWORD m_nFallbackSeed;
	extern DWORD m_nFallbackStatTrak;
	extern DWORD m_nFallbackPaintKit;
	extern DWORD m_hMyWearables;
	extern DWORD m_nModelIndex;
	extern DWORD m_hOwner;
	extern DWORD m_iViewModelIndex;
	extern DWORD m_iWorldModelIndex;
	extern DWORD m_hWeaponWorldModel;
	extern DWORD m_Item;
	extern DWORD m_OriginalOwnerXuidLow;
	extern DWORD m_OriginalOwnerXuidHigh;
	extern DWORD m_iClip1;
	extern DWORD m_hMyWeapons;
	extern DWORD animlayer;
	extern DWORD m_flFlashDuration;
	extern DWORD m_viewPunchAngle;
	extern DWORD m_flDuckAmount;
	extern DWORD m_hObserverTarget;
	extern DWORD m_iShotsFired;

	extern DWORD invalidateBoneCache;
	extern DWORD linegoesthrusmoke;

	extern DWORD isBreakableEntity;
	extern DWORD initKeyValues;
	extern DWORD loadFromBuffer;
	extern DWORD getSequenceActivity;
	extern DWORD serverRankRevealAll;
	extern DWORD sendClantagChanged;

	extern DWORD DrawSetColor_ScopeArc;
	extern DWORD DrawSetColor_ScopeLens;

	extern DWORD deviceStructureAddress;

	extern int* nPredictionRandomSeed;

	void SetupOffsets();
}