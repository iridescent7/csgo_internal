#pragma once
#include "..\SDK\SDK.h"
#include <string>
#include <vector>

class MaterialCache
{
public:
	IMaterial* material;
	char       texture_group;

	float color_mod[3];
	float alpha_mod;

	MaterialCache()	{ }
	MaterialCache(IMaterial* pMaterial, char texGroup)
	{
		material      = pMaterial;
		texture_group = texGroup;

		pMaterial->GetColorModulation(&color_mod[0], &color_mod[1], &color_mod[2]);
		alpha_mod = pMaterial->GetAlphaModulation();
	}

	void RestoreColorMod()
	{
		material->ColorModulate(color_mod[0], color_mod[1], color_mod[2]);
	}

	void RestoreAlphaMod()
	{
		material->AlphaModulate(alpha_mod);
	}
};

class CNightMode
{
private:
	bool first_load = true;
	bool nightmode_applied = false;

	std::string skyname_backup;
	std::vector<MaterialCache> cached_materials;

	bool  asus_applied;
	float last_asus_amount;

public:
	void FrameStageNotify(int stage);

public:
	bool  NightModeEnabled = true;
	bool  AsusPropsEnabled = false;
	float AsusPropsAmount = false;
};