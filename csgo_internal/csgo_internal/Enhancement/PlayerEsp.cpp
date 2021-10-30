#include "PlayerEsp.h"
#include "..\SDK\SDK.h"
#include "..\Globals.h"
#include "../Utils/Util.h"

void CPlayerEsp::CreateMove(CUserCmd* cmd, bool* send_packet)
{
	int  local_idx    = I::Engine->GetLocalPlayer();
	auto local_player = I::EntityList->GetClientEntity(local_idx);

	if (!local_player)
		return;

	if (!local_player->IsAlive())
	{
		local_observe_target = local_player->m_hObserverTarget()->index & 0xFFF;
	}
	else
	{
		local_observe_target = 0;
	}

	int local_team = local_player->m_iTeamNum();
	
	float fade_in  = (1.f / 0.2f) * I::Globals->frametime;
	float fade_out = (1.f / 2.f)  * I::Globals->frametime;

	for (int idx = 1; idx != MAX_PLAYERS; ++idx)
	{
		auto& ve = visual_entities[idx];

		C_CSPlayer* entity;
		if (idx == local_idx
			|| !(entity = I::EntityList->GetClientEntity(idx))
			|| !entity->IsPlayer()
			|| !entity->IsAlive())
		{
			ve.valid = false;
			esp_alpha[idx] = 0.f;
			continue;
		}

		ve.dormant = entity->IsDormant();

		if (!ve.dormant)
			esp_alpha[idx] += fade_in;
		else
			esp_alpha[idx] -= fade_out;

		Clamp(esp_alpha[idx], 0.f, 1.f);

		if (esp_alpha[idx] == 0.f)
		{
			ve.valid = false;
			continue;
		}

		player_info_t player_info;

		if (I::Engine->GetPlayerInfo(idx, &player_info))
		{
			ve.name[24] = L'\0';

			U::ConvertANSIToUnicode(player_info.name, ve.name, 25);

			if (ve.name[24] != L'\0')
				wcsncpy(&ve.name[20], L" ...\0", 5);
		}

		ve.health  = min(entity->m_iHealth(), 100);
		ve.hp_text = std::to_string(ve.health);

		bool enemy = local_team != entity->m_iTeamNum();

		if ( (!enemy && !EspTeams[ESP_TEAM])
		  || ( enemy && !EspTeams[ESP_ENEMY] )
		   )
		{
			ve.valid = false;
			esp_alpha[idx] = 0.f;
			continue;
		}

		ve.origin = entity->GetAbsOrigin();
		ve.jumping = !(entity->m_fFlags() & FL_ONGROUND) && entity->m_nMoveType() != MOVETYPE_LADDER;
		ve.duck_amount = entity->m_flDuckAmount();
		ve.angles = entity->m_angEyeAngles();
		ve.eyepos = entity->GetEyePosition();
		C::LegitAimbot.GetHitboxPoint(entity, HITBOX_HEAD, &ve.headpos);

		if (!ve.dormant)
		{
			auto weapon = (CBaseCombatWeapon*)I::EntityList->GetClientEntityFromHandle(entity->m_hActiveWeapon());

			ve.valid_weapon = (weapon != nullptr);

			if (ve.valid_weapon)
			{
				ve.weapon_ammo = weapon->m_iClip1();
				ve.weapon_ammo_text = std::to_string(ve.weapon_ammo);

				auto weapon_data = weapon->GetCSWeaponData();

				if (weapon_data)
				{
					ve.weapon_shootable = (weapon_data->basic_weapon_type > BASICWEAPONTYPE_KNIFE);
					ve.weapon_maxclip = weapon_data->max_clip;

					strncpy(ve.weapon_name, weapon_data->weapon_name + 7, 24);
					ve.weapon_name[24] = '\0';

					for (size_t i = 0; i < strlen(ve.weapon_name); ++i)
						ve.weapon_name[i] = toupper(ve.weapon_name[i]);

					if (entity->m_bIsScoped()) strcat(ve.weapon_name, " (Scoped)");
				}

			}

			auto animlayer = entity->GetAnimLayer()[1];

			if (animlayer.m_pOwner)
			{
				int activity = U::GetSequenceActivity(entity, animlayer.m_nSequence);

				ve.weapon_reloading = (activity == 967 && animlayer.m_flWeight != 0.0f);
				ve.weapon_reload_cycle = ve.weapon_reloading ? animlayer.m_flCycle : 1.f;
			}
		}
		else
		{
			ve.valid_weapon = false;

			ve.weapon_reloading    = false;
			ve.weapon_reload_cycle = 1.f;
		}

		ve.valid = true;
	}
}

void CPlayerEsp::EndScene()
{
	if (!I::Engine->IsInGame())
	{
		if (!ve_list_cleared)
		{
			for (size_t idx = 1; idx != MAX_PLAYERS; ++idx)
				visual_entities[idx].valid = false;

			ve_list_cleared = true;
		}
		return;
	}

	int local_idx = I::Engine->GetLocalPlayer();
	auto local_player = I::EntityList->GetClientEntity(local_idx);

	for (size_t idx = 1; idx != MAX_PLAYERS; ++idx)
	{
		auto& ve = visual_entities[idx];

		if (!ve.valid)
			continue;

		if (local_observe_target == idx)
			continue;

		ScreenBBox bbox;

		if (GetPlayerBoundingBox(ve, &bbox) && (!C::VisualExtras.DeadVisuals || (C::VisualExtras.DeadVisuals && !local_player->IsAlive())) )
		{
			byte alpha = 255 * esp_alpha[idx];

			if (EspFeatures[ESP_NAME])
				DrawPlayerName(ve, bbox, alpha);

			if (EspFeatures[ESP_WEAPON])
				DrawPlayerWeapon(ve, bbox, alpha);

			if (EspFeatures[ESP_BOX])
				DrawPlayerBox(ve, bbox, alpha);

			if (EspFeatures[ESP_HEALTH])
				DrawPlayerHealth(ve, bbox, alpha);

			if (EspFeatures[ESP_AMMOBAR])
				DrawPlayerAmmoBar(ve, bbox, alpha);

			if (EspFeatures[ESP_BARREL])
				DrawBarrel(ve, bbox, alpha);
		}
	}
}

bool CPlayerEsp::GetPlayerBoundingBox(const VisualEntity& ve, ScreenBBox* bbox)
{
	// todo: store origin and stuff, and calculate screenpos in endscene instead
	Vector top, down;

	down = ve.origin;

	if (ve.jumping)
		down.z += 10.f;

	top = down;
	top.z += 72.f + (-18.f * ve.duck_amount);

	Vector2D top_screen, down_screen;

	if (R::WorldToScreen(top, &top_screen) && R::WorldToScreen(down, &down_screen))
	{
		float delta = down_screen.y - top_screen.y;

		bbox->h = fabs(delta);
		bbox->w = bbox->h / 2.f;

		bbox->x = top_screen.x - (bbox->w / 2.f);
		bbox->y = top_screen.y;

		return true;
	}

	return false;
}

void CPlayerEsp::DrawPlayerBox(const VisualEntity& ve, const ScreenBBox& bbox, byte alpha)
{
	byte outline_alpha = alpha * 0.6f;

	D3DXCOLOR color = ve.dormant ? D3DCOLOR_ARGB(alpha, 130, 130, 130) : D3DCOLOR_ARGB(alpha, 0, 255, 0);

	R::Rect(bbox.x - 1, bbox.y - 1, bbox.w + 2, bbox.h + 2, D3DCOLOR_ARGB(outline_alpha, 0, 0, 0));
	R::Rect(bbox.x, bbox.y, bbox.w, bbox.h, color);
	R::Rect(bbox.x + 1, bbox.y + 1, bbox.w - 2, bbox.h - 2, D3DCOLOR_ARGB(outline_alpha, 0, 0, 0));
}

void CPlayerEsp::DrawBarrel(const VisualEntity& ve, const ScreenBBox& bbox, byte alpha)
{

	Vector2D p1, p2;

	Vector p2_(
				50.0f * cosf(DEG2RAD(ve.angles.pitch)) * cosf(DEG2RAD(ve.angles.yaw)),
				50.0f * cosf(DEG2RAD(ve.angles.pitch)) * sinf(DEG2RAD(ve.angles.yaw)),
				50.0f * -sinf(DEG2RAD(ve.angles.pitch))
			  );

	p2_ += ve.headpos;

	if (R::WorldToScreen(ve.headpos, &p1) && R::WorldToScreen(p2_, &p2))
	{
		R::Line(p1.x, p1.y, p2.x, p2.y, D3DCOLOR_XRGB(255, 255, 255));
	}
}

void CPlayerEsp::DrawPlayerHealth(const VisualEntity& ve, const ScreenBBox& bbox, byte alpha)
{
	D3DCOLOR text_color, color;

	byte outline_alpha = alpha * 0.6f,
		inner_back_alpha = alpha * 0.6f;

	ScreenBBox new_bb =
	{
		bbox.x - 5,
		bbox.y + 1,
		2,
		bbox.h - 2
	};

	if (ve.dormant)
	{
		text_color = D3DCOLOR_ARGB(alpha, 130, 130, 130);
		color = D3DCOLOR_ARGB(alpha, 130, 130, 130);
	}
	else
	{
		text_color = D3DCOLOR_ARGB(alpha, 255, 255, 255);
		color = D3DCOLOR_ARGB(alpha, 153, min(255, ve.health * 225 / 100), 0);
	}

	int	bar_height = ve.health * new_bb.h / 100.0f;
	int offset = new_bb.h - bar_height;

	float height = new_bb.h / 10.f;

	R::Rect(bbox.x - 6, bbox.y, 4, bbox.h, D3DCOLOR_ARGB(outline_alpha, 0, 0, 0));

	R::FilledRect(new_bb.x, new_bb.y, 2, new_bb.h, D3DCOLOR_ARGB(inner_back_alpha, 0, 0, 0));
	R::FilledRect(new_bb.x, new_bb.y + offset, 2, bar_height, color);

	for (size_t i = 0; i < 10; ++i)
		R::Line(new_bb.x, new_bb.y + i * height, new_bb.x + 2, new_bb.y + i * height, D3DCOLOR_ARGB(inner_back_alpha, 0, 0, 0));

	if (ve.health < 100)
	{
		float w, h;
		R::GetTextSize(F::EspHealth, ve.hp_text.c_str(), &w, &h);

		R::Text(F::EspHealth, new_bb.x + 1 - w / 2, new_bb.y + offset - h / 2, text_color, ve.hp_text.c_str());
	}
}

void CPlayerEsp::DrawPlayerName(const VisualEntity& ve, const ScreenBBox& bbox, byte alpha)
{
	D3DCOLOR color = ve.dormant ? D3DCOLOR_ARGB(130, 130, 130, 130) : D3DCOLOR_ARGB(alpha, 255, 255, 255);

	float w, h;
	R::GetTextSizeW(F::EspText, ve.name, &w, &h);

	R::TextW(F::EspText, bbox.x + (bbox.w / 2) - w / 2, bbox.y - 16, color, ve.name);
}

void CPlayerEsp::DrawPlayerWeapon(const VisualEntity& ve, const ScreenBBox& bbox, byte alpha)
{
	if (!ve.valid_weapon)
		return;

	D3DCOLOR color = ve.dormant ? D3DCOLOR_ARGB(130, 130, 130, 130) : D3DCOLOR_ARGB(alpha, 255, 255, 255);

	bool drawing_ammo_bar = EspFeatures[ESP_AMMOBAR] && ve.weapon_shootable;

	float w, h;
	R::GetTextSize(F::EspText, ve.weapon_name, &w, &h);

	R::Text(F::EspText, bbox.x + (bbox.w / 2) - w / 2, bbox.y + bbox.h + (drawing_ammo_bar ? 7 : 2), color, ve.weapon_name);
}

void CPlayerEsp::DrawPlayerAmmoBar(const VisualEntity& ve, const ScreenBBox& bbox, byte alpha)
{
	if (!ve.valid_weapon || !ve.weapon_shootable)
		return;

	D3DCOLOR text_color, color;

	byte
		outline_alpha = alpha * 0.6f,
		inner_back_alpha = alpha * 0.6f;

	if (ve.dormant)
	{
		text_color = D3DCOLOR_ARGB(alpha, 130, 130, 130);
		color = D3DCOLOR_ARGB(alpha, 130, 130, 130);
	}
	else
	{
		text_color = D3DCOLOR_ARGB(alpha, 255, 255, 255);
		color = D3DCOLOR_ARGB(alpha, 167, 67, 227);
	}

	ScreenBBox new_bb = {
		bbox.x + 1,
		bbox.y + bbox.h + 3,
		bbox.w - 1,
		2
	};

	float bar_width = ve.weapon_ammo * bbox.w / ve.weapon_maxclip;

	if (ve.weapon_reloading && ve.weapon_reload_cycle < 0.99f)
	{
		bar_width = (ve.weapon_reload_cycle * bbox.w) / 1.f;
	}

	R::FilledRect(new_bb.x - 1, new_bb.y - 1, bbox.w + 2, 4, D3DCOLOR_ARGB(inner_back_alpha, 0, 0, 0));
	R::FilledRect(new_bb.x, new_bb.y, bar_width, 2, color);

	if (ve.weapon_ammo != ve.weapon_maxclip)
	{
		float w, h;
		R::GetTextSize(F::Text, ve.weapon_ammo_text.c_str(), &w, &h);

		R::Text(F::Text, new_bb.x + bar_width - w / 2, new_bb.y + 1 - h / 2, text_color, ve.weapon_ammo_text.c_str());
	}
}
