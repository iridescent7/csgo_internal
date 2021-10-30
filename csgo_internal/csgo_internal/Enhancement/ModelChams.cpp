#include "ModelChams.h"
#include "..\Globals.h"
#include "..\Hooks\Hooks.h"

void CModelChams::DrawModelExecute(void* thisptr, IMatRenderContext* context, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* pCustomBoneToWorld)
{
	bool isInGame = I::Engine->IsInGame();

	static IMaterial* glass_material = nullptr;
	static bool hasLoaded = false;
	
	if (isInGame && !hasLoaded)
	{
		glass_material = I::MaterialSystem->FindMaterial("models/inventory_items/cologne_prediction/cologne_prediction_glass", nullptr, true, nullptr);
		hasLoaded = true;
	}
	else if (!isInGame && hasLoaded)
	{
		hasLoaded = false;
	}

	if (isInGame && info.pModel)
	{
		auto local_player = I::EntityList->GetClientEntity(I::Engine->GetLocalPlayer());
		int  local_team = local_player->m_iTeamNum();

		const char* model_name = I::ModelInfo->GetModelName(info.pModel);

		if (cdk::strcheck(model_name, "models/player"))
		{
			auto entity = I::EntityList->GetClientEntity(info.entity_index);
			
			if (entity && entity->IsPlayer() && entity->IsAlive())
			{
				bool enemy = local_team != entity->m_iTeamNum();

				if ( (!enemy && ChamsTeams[CHAMS_TEAM])
			      || ( enemy && ChamsTeams[CHAMS_ENEMY])
				   )
				{
					Color color_hidden = enemy ? Color(0, 255, 255) : Color(72, 219, 75);
					Color color_visible = enemy ? Color(255, 0, 255) : Color(84, 167, 255);

					bool already_rendered = false;

					if (C::LegitBacktrack.Enabled && C::LegitBacktrack.DrawLagTicks)
					{
						static Color greeeeen(0, 255, 0);
						glass_material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, !C::VisualExtras.DeadVisuals || (C::VisualExtras.DeadVisuals && !local_player->IsAlive()));
						glass_material->ColorModulate(greeeeen);

						I::ModelRender->ForcedMaterialOverride(glass_material);

						for (int idx = 0; idx <= MAX_LAGTICKS; idx++)
						{
							if (C::LegitBacktrack.records[info.entity_index][idx].simtime == -1)
								continue;

							if (!entity || entity->IsDormant() || !entity->IsAlive() || entity->m_iTeamNum() == local_player->m_iTeamNum() || entity->GetEyePosition().DistTo(C::LegitBacktrack.records[info.entity_index][idx].world_position) < 5.f)
								continue;

							H::oDrawModelExecute(thisptr, context, state, info, C::LegitBacktrack.records[info.entity_index][idx].bones);
						}
					}

					if ( (ChamsMode[CHAMS_INVISIBLE] && (!C::VisualExtras.DeadVisuals || !enemy)) || ( ChamsMode[CHAMS_INVISIBLE] && C::VisualExtras.DeadVisuals && enemy && !local_player->IsAlive()) )
					{
						if (InvisibleChamsType == CHAMS_ORIGINAL)
						{
							I::ModelRender->ForcedMaterialOverride(nullptr);
						}
						else
						{
							IMaterial* chams_material_p = (InvisibleChamsType == CHAMS_FLAT) ? C::Flat : C::Regular;

							chams_material_p->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, true);
							chams_material_p->ColorModulate(color_hidden);

							I::ModelRender->ForcedMaterialOverride(chams_material_p);
						}

						H::oDrawModelExecute(thisptr, context, state, info, pCustomBoneToWorld);
						already_rendered = true;
					}

					if (ChamsMode[CHAMS_VISIBLE])
					{
						auto chams_material_p = (VisibleChamsType == CHAMS_FLAT) ? C::Flat : C::Regular;

						if (VisibleChamsType == CHAMS_ORIGINAL)
						{
							I::ModelRender->ForcedMaterialOverride(nullptr);
						}
						else
						{
							
							chams_material_p->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);
							chams_material_p->ColorModulate(color_visible);

							I::ModelRender->ForcedMaterialOverride(chams_material_p);
						}

						H::oDrawModelExecute(thisptr, context, state, info, pCustomBoneToWorld);

						already_rendered = true;
					}

					if (already_rendered)
					{
						I::ModelRender->ForcedMaterialOverride(nullptr);
						return;
					}
				}
			}
		}
		else if (WeaponChams && cdk::strcheck(model_name, "models/weapons/v_") && !local_player->m_bIsScoped())
		{
			//static auto chams_material = I::MaterialSystem->FindMaterial("models/inventory_items/cologne_prediction/cologne_prediction_glass", nullptr, true, nullptr);

			glass_material->SetMaterialVarFlag(MATERIAL_VAR_IGNOREZ, false);

			glass_material->ColorModulate(cdk::strcheck(model_name, "models/weapons/v_models") ? Color(0, 255, 255) : Color(255, 0, 255));

			glass_material->AlphaModulate(30.f);

			I::ModelRender->ForcedMaterialOverride(glass_material);
			H::oDrawModelExecute(thisptr, context, state, info, pCustomBoneToWorld);
			I::ModelRender->ForcedMaterialOverride(nullptr);
			return;
		}
		
		H::oDrawModelExecute(thisptr, context, state, info, pCustomBoneToWorld);
	}
}