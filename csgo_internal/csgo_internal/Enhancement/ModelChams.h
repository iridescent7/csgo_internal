#pragma once
#include "..\SDK\SDK.h"

enum
{
	CHAMS_TEAM,
	CHAMS_ENEMY
};

enum
{
	CHAMS_VISIBLE,
	CHAMS_INVISIBLE
};

enum
{
	CHAMS_FLAT,
	CHAMS_REGULAR,
	CHAMS_ORIGINAL
};

class CModelChams
{
public:
	void DrawModelExecute(void* thisptr, IMatRenderContext* context, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* pCustomBoneToWorld);

public:
	bool ChamsTeams[2] = { false, true };
	bool ChamsMode[3] = { true, true };

	bool WeaponChams = false;
	bool C4Chams = true;

	int VisibleChamsType = 1;
	int InvisibleChamsType = 1;
};