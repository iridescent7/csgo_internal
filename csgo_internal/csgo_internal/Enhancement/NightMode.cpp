#include "NightMode.h"
#include "..\Globals.h"

void CNightMode::FrameStageNotify(int stage)
{
	if (stage == FRAME_RENDER_START)
	{
		if (!I::Engine->IsInGame())
		{
			first_load = true;
			return;
		}

		if (first_load)
		{
			cached_materials.clear();

			for (MaterialHandle_t h = I::MaterialSystem->FirstMaterial(); h != I::MaterialSystem->InvalidMaterial(); h = I::MaterialSystem->NextMaterial(h))
			{
				IMaterial* pMaterial = I::MaterialSystem->GetMaterial(h);

				if (!pMaterial || pMaterial->IsErrorMaterial())
					continue;

				if (pMaterial->GetReferenceCount() <= 0)
					continue;

				const char*   texGroupName = pMaterial->GetTextureGroupName();
				MaterialCache mat_cache;

				if (strstr(texGroupName, "World"))
				{
					mat_cache = MaterialCache(pMaterial, 'W');
				}
				else if (strstr(texGroupName, "StaticProp"))
				{
					mat_cache = MaterialCache(pMaterial, 'P');
				}
				else
					continue; // dont store unneeded materials

				cached_materials.emplace_back(mat_cache);
			}

			nightmode_applied = false;
			asus_applied = false;

			first_load = false;
		}

		if (NightModeEnabled)
		{
			if (!nightmode_applied)
			{
				skyname_backup = C::sv_skyname.GetString();

				C::sv_skyname.SetString("sky_csgo_night02");

				for (auto& cache : cached_materials)
				{
					if (cache.texture_group == 'W')
					{
						cache.material->ColorModulate(1.f, 220.f / 255.f, 1.f);
					}
					else // StaticProps
					{
						cache.material->ColorModulate(0.4f, 0.4f, 0.4f);
					}
				}

				nightmode_applied = true;
			}
		}
		else
		{
			if (nightmode_applied)
			{
				C::sv_skyname.SetString(skyname_backup.c_str());

				for (auto& cache : cached_materials)
					cache.RestoreColorMod();

				nightmode_applied = false;
			}
		}

		if (AsusPropsEnabled)
		{
			if (!asus_applied || AsusPropsAmount != last_asus_amount)
			{
				for (auto& cache : cached_materials)
				{
					if (cache.texture_group == 'P')
						cache.material->AlphaModulate(AsusPropsAmount);
				}

				asus_applied     = true;
				last_asus_amount = AsusPropsAmount;
			}
		}
		else
		{
			if (asus_applied)
			{
				for (auto& cache : cached_materials)
					cache.RestoreAlphaMod();

				asus_applied = false;
			}
		}
	}
}