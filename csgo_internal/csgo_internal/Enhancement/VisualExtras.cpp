#include "VisualExtras.h"
#include "..\Globals.h"

const char* smoke_and_effects[] =
{
	"particle/vistasmokev1/vistasmokev1_smokegrenade",
	"particle/vistasmokev1/vistasmokev1_fire",
	"particle/vistasmokev1/vistasmokev1_emods",
	"particle/vistasmokev1/vistasmokev1_emods_impactdust",
};

void CVisualExtras::CreateMove(CUserCmd* cmd, bool* send_packet)
{
	bool no_recoil_applied =
	   (C::view_recoil_tracking.GetFloat()           == 0.f
	 && C::weapon_recoil_view_punch_extra.GetFloat() == 0.f);

	if (VisualRemovals[REMOVALS_RECOIL] != no_recoil_applied)
	{
		if (VisualRemovals[REMOVALS_RECOIL])
		{
			C::view_recoil_tracking.SetFloat(0.f);
			C::weapon_recoil_view_punch_extra.SetFloat(0.f);
		}
		else
		{
			C::view_recoil_tracking.ResetFloat();
			C::weapon_recoil_view_punch_extra.ResetFloat();
		}
	}

	bool no_zoom_sens_applied =
		(C::zoom_sensitivity_ratio_mouse.GetFloat()    == 0.f
	  && C::zoom_sensitivity_ratio_joystick.GetFloat() == 0.f);

	if (VisualRemovals[REMOVALS_ZOOM] != no_zoom_sens_applied)
	{
		if (VisualRemovals[REMOVALS_ZOOM])
		{
			C::zoom_sensitivity_ratio_mouse.SetFloat(0.f);
			C::zoom_sensitivity_ratio_joystick.SetFloat(0.f);
		}
		else
		{
			C::zoom_sensitivity_ratio_mouse.ResetFloat();
			C::zoom_sensitivity_ratio_joystick.ResetFloat();
		}
	}
}

void CVisualExtras::FrameStageNotify(int stage)
{
	if (stage == FRAME_RENDER_START)
	{
		if (!I::Engine->IsInGame())
		{
			first_load = true;
			return;
		}

		if(first_load)
		{
			scope_blur_mat = I::MaterialSystem->FindMaterial("dev/scope_bluroverlay", NULL);

			smoke_materials.clear();

			for (auto& mat_name : smoke_and_effects)
			{
				IMaterial* pMaterial = I::MaterialSystem->FindMaterial(mat_name, TEXTURE_GROUP_OTHER);

				smoke_materials.emplace_back(pMaterial);
			}

			first_load = false;
		}

		auto local_player = I::EntityList->GetClientEntity(I::Engine->GetLocalPlayer());

		if (local_player->IsAlive() && Thirdperson)
		{
			if (GetAsyncKeyState(ThirdpersonKey) && GetTickCount() > next_tp_update)
			{
				I::Input->m_fCameraInThirdPerson = !I::Input->m_fCameraInThirdPerson;
				next_tp_update = GetTickCount() + 400;
			}

			if (I::Input->m_fCameraInThirdPerson)
			{
				constexpr int ideal_thirdperson_distance = 120;

				QAngle viewAngle;
				I::Engine->GetViewAngles(viewAngle);

				QAngle angleInverse = QAngle(viewAngle.pitch * -1.f, viewAngle.yaw + 180.f, 0.f);
				Vector direction;
				M::AngleVectors(angleInverse, &direction);

				CTraceWorldOnly filter;
				trace_t trace;

				Vector eyePosition = local_player->GetEyePosition();
				U::TraceLine(eyePosition, eyePosition + direction * (ideal_thirdperson_distance + 5.f), MASK_ALL, &filter, &trace);

				float distance = ideal_thirdperson_distance * trace.fraction;

				I::Input->m_vecCameraOffset = Vector(viewAngle.pitch, viewAngle.yaw, distance);
			}
		}
		else
			I::Input->m_fCameraInThirdPerson = false;

		if (VisualRemovals[REMOVALS_FLASH])
			local_player->m_flFlashDuration() = 0.f;

		if (scope_blur_mat)
		{
			scope_blur_mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, VisualRemovals[REMOVALS_SCOPE_BLUR]);
		}

		if (VisualRemovals[REMOVALS_SMOKE])
		{
			for (auto& mat : smoke_materials)
				mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, true);

			int* smoke_count = *(int**)(O::linegoesthrusmoke + 0x8);
			*smoke_count = 0;
		}
		else
		{
			for (auto& mat : smoke_materials)
				mat->SetMaterialVarFlag(MATERIAL_VAR_NO_DRAW, false);
		}
	}
}

void CVisualExtras::OverrideView(CViewSetup* pSetup)
{
	if (I::Engine->IsInGame())
	{
		float fov_value = I::Input->m_fCameraInThirdPerson ? (float)ThirdpersonFov : (float)Fov;

		if (VisualRemovals[REMOVALS_ZOOM] || OverrideFov)
		{
			pSetup->fov = OverrideFov ? fov_value : 90.f;
		}
	}
}
