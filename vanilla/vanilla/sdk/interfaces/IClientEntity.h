#pragma once 

#include <stdint.h>
#include <math.h>

#include "IClientNetworkable.h"
#include "IClientRenderable.h"
#include "IClientUnknown.h"
#include "IClientThinkable.h"

class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
	virtual void Release(void) = 0;
};

struct CCSWeaponInfo;

class IWeaponSystem
{
	virtual void unused0() = 0;
	virtual void unused1() = 0;
public:
	virtual const CCSWeaponInfo* GetWpnData(unsigned ItemDefinitionIndex) = 0;
};