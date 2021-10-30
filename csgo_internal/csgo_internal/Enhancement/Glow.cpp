#include "Glow.h"

void CGlow::SceneEnd()
{
	if (!GlowTeams[GLOW_TEAM] && !GlowTeams[GLOW_ENEMY])
		return;

	auto   local_player = I::EntityList->GetClientEntity(I::Engine->GetLocalPlayer());
	size_t size         = I::GlowObjManager->GetSize();

	for (size_t i = 0; i < size; ++i)
	{
		auto& glowObj = I::GlowObjManager->m_GlowObjectDefinitions[i];

		if (!glowObj.m_pEntity || glowObj.IsUnused())
			continue;

		int idx = glowObj.m_pEntity->GetIndex();

		// todo: weapon / nade glow
		if (idx < 1 || idx >= MAX_PLAYERS)
			continue;

		bool enemy = (local_player->m_iTeamNum() != glowObj.m_pEntity->m_iTeamNum());

		if (!enemy && !GlowTeams[GLOW_TEAM]
		  || enemy && !GlowTeams[GLOW_ENEMY])
		{
			// todo: reset to original values / colors
			glowObj.m_bRenderWhenOccluded = false;
			glowObj.m_bRenderWhenUnoccluded = false;
			continue;
		}

		glowObj.m_nGlowStyle = GlowStyle;

		glowObj.m_flRed = 144 / 255.f;
		glowObj.m_flGreen = 238 / 255.f;
		glowObj.m_flBlue = 144 / 255.f;
		glowObj.m_flAlpha = 1.f;

		glowObj.m_bRenderWhenOccluded = true;
		glowObj.m_bRenderWhenUnoccluded = false;
	}
}