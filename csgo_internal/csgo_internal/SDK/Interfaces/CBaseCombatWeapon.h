#pragma once
#include "getvfunc.h"

#include "..\..\offsets.h"
#include "..\..\interfaces.h"

#include "..\Vector.h"

class CBaseHandle;
class CSWeaponInfo;

class CBaseCombatWeapon
{
public:
	PNETVAR_GET(int, m_nFallbackPaintKit)
	PNETVAR_GET(CBaseHandle, m_hWeaponWorldModel)
	PNETVAR_GET(int, m_iEntityQuality)
	PNETVAR_GET(int, m_OriginalOwnerXuidLow)
	PNETVAR_GET(float, m_flFallbackWear)
	PNETVAR_GET(int, m_iItemIDHigh)
	PNETVAR_GET(int, m_OriginalOwnerXuidHigh)
	PNETVAR_GET(int, m_nModelIndex)
	PNETVAR_GET(int, m_iViewModelIndex)

	NETVAR_GETSET(Vector, m_vecOrigin)
	NETVAR_GETSET(short, m_iItemDefinitionIndex)
	NETVAR_GETSET(float, m_flNextPrimaryAttack)
	NETVAR_GETSET(float, m_flPostponeFireReadyTime)
	NETVAR_GETSET(int, m_iClip1)

	float GetSpread()
	{
		typedef float(__thiscall* Fn)(void*);
		return getvfunc<Fn>(this, 452)(this);
	}
	float GetInaccuracy()
	{
		typedef float(__thiscall* Fn)(void*);
		return getvfunc<Fn>(this, 482)(this); //479
	}
	HANDLE GetOwner()
	{
		return *(HANDLE*)((DWORD)this + 0x031D0);
	}
	void UpdateAccuracyPenalty()
	{
		typedef void(__thiscall* Fn)(void*);
		return getvfunc<Fn>(this, 479)(this); //477
	}
	CSWeaponInfo* GetCSWeaponData()
	{
		typedef CSWeaponInfo *(__thiscall* Fn)(void*);
		return getvfunc<Fn>(this, 460)(this); //457
	}

	bool IsRevolver()
	{
		return (this->m_iItemDefinitionIndex() == WEAPON_REVOLVER);
	}
	bool IsKnife()
	{
		int id = this->m_iItemDefinitionIndex();

		return (id == WEAPON_KNIFE_CT
			|| id == WEAPON_KNIFE_T
			|| id == WEAPON_BAYONET
			|| id == WEAPON_KNIFE_FLIP
			|| id == WEAPON_KNIFE_GUT
			|| id == WEAPON_KNIFE_KARAMBIT
			|| id == WEAPON_KNIFE_M9_BAYONET
			|| id == WEAPON_KNIFE_TACTICAL
			|| id == WEAPON_KNIFE_FALCHION
			|| id == WEAPON_KNIFE_BUTTERFLY
			|| id == WEAPON_KNIFE_PUSH);
	}
	bool IsNade()
	{
		int id = this->m_iItemDefinitionIndex();

		return (id == WEAPON_FLASHBANG
			|| id == WEAPON_HEGRENADE
			|| id == WEAPON_SMOKEGRENADE
			|| id == WEAPON_MOLOTOV
			|| id == WEAPON_DECOY
			|| id == WEAPON_INCGRENADE);
	}
	bool IsBomb()
	{
		return (this->m_iItemDefinitionIndex() == WEAPON_C4);
	}
};

class CSBomb
{
public:
	CBaseHandle* GetOwner()
	{
		return (CBaseHandle*)((DWORD)this + 0x31D0);
	}

	float GetBlowTime()
	{
		return *(float*)((DWORD)this + 0x297C);
	}

	float GetDefuseCountDown()
	{
		return *(float*)((DWORD)this + 0x2994);
	}

	int GetBombDefuser()
	{
		return *(int*)((DWORD)this + 0x17B8);
	}

	bool IsBombDefused()
	{
		return *(bool*)((DWORD)this + 0x2998);
	}
};

class CBaseCSGrenade : CBaseCombatWeapon
{
public:
	float IsPinPulled()
	{
		return *(float*)((DWORD)this + O::m_bPinPulled);
	}

	float GetThrowTime()
	{
		return *(float*)((DWORD)this + O::m_fThrowTime);
	}
};