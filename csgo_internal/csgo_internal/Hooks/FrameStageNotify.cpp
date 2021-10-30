#include "Hooks.h"
#include "..\SDK\SDK.h"
#include "..\Globals.h"

namespace H
{
	void __fastcall Hooked_FrameStageNotify(void* thisptr, void* edx, int stage)
	{
		if (stage == FRAME_RENDER_START && I::Engine->IsInGame())
		{
			int local_idx = I::Engine->GetLocalPlayer();

			for (int idx = 1; idx != MAX_PLAYERS; idx++)
			{
				if (idx == local_idx)
					continue;

				auto entity = I::EntityList->GetClientEntity(idx);

				if (!entity
					|| entity->IsDormant()
					|| !entity->IsPlayer())
					continue;

				// pelvis meme
				*(int*)((DWORD)entity + 0xA30) = I::Globals->framecount; // force into latest framecount to skip checks
				*(int*)((DWORD)entity + 0xA28) = 0; // clear flags
			}

			if (I::Input->m_fCameraInThirdPerson)
			{
				auto local_player = I::EntityList->GetClientEntity(local_idx);

				I::Prediction->SetLocalViewAngles(C::LastAngle);

				// already called for every player pre renderview
				//local_player->UpdateClientSideAnimation();
			}
		}

		C::NightMode.FrameStageNotify(stage);
		C::VisualExtras.FrameStageNotify(stage);

		oFrameStageNotify(thisptr, stage);
	}
}