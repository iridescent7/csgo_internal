#pragma once
#include "..\SDK\SDK.h"

enum
{
	GLOW_TEAM,
	GLOW_ENEMY
};

enum
{
	GLOW_REGULAR,
	GLOW_PULSING,
	GLOW_OUTLINE
};

class CGlow
{
public:
	void SceneEnd();

public:
	bool GlowTeams[2] = { false };
	int  GlowStyle = 0;
};