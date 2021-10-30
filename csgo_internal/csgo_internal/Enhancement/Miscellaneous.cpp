#include "Miscellaneous.h"
#include "..\SDK\SDK.h"
#include "..\Globals.h"

const char* nex_clantag_strings[] =
{
	u8"\u2606\u2606\u2606\u2606\u2606",
	u8"\u2605\u2606\u2606\u2606\u2606",
	u8"\u2605\u2605\u2606\u2606\u2606",
	u8"\u2605\u2605\u2605\u2606\u2606",
	u8"\u2605\u2605\u2605\u2605\u2606",
	u8"\u2605\u2605\u2605\u2605\u2605"
};

// aaagggggh
const char* gaysense[] =
{
	"                 g",
	"                ga",
	"               gay",
	"              gayy",
	"             gayys",
	"            gayyse",
	"           gayysen",
	"          gayysens",
	"         gayysense",
	"        gayysense ",
	"       gayysense  ",
	"      gayysense   ",
	"     gayysense    ",
	"    gayysense     ",
	"   gayysense      ",
	"  gayysense       ",
	" gayysense        ",
	"gayysense         ",
	"ayysense          ",
	"yysense           ",
	"ysense            ",
	"sense             ",
	"sens              ",
	"sen               ",
	"se                ",
	"s                 ",
	"                  "
};

const char* blendS[] =
{
	"SMILE",
	"SWEET",
	"SISTER",
	"SADISTIC",
	"SUPRISE",
	"SERVICE"
};

const char* clantags[] =
{
	u8"Nanahira\u2665",
	"CSJAPAN",
	u8"\u30DC\u30C3\u30C8", // BOT
	u8"\u30CD\u30BF\u30DF\u30CB\u30C6\u30A3", // netaminity
	u8"\u30DC\u30C3\u30C8 [\u273F] ", // BOT flower,
	"suika.xyz",
	"x88cheats",
	u8"\u3067\u3059", // desu
	"closethvh",
	"UWUWARE.net",
	"WatchTheDemo",
	"acc-suicide",
	u8"[ \u273F ]", // Black Florette
	u8"[ \u2740 ]", // White Florette
	"awoo",
	"ZettaiRyoiki",
	"DraculaPremium",
	"too.lewd.se",
	"cute",
	"hentaiware.moe"
};

void CMiscellaneous::CreateMove(CUserCmd* cmd, bool* send_packet)
{
	if (RankRevealer && (cmd->buttons & IN_SCORE))
		I::Client->DispatchUserMessage(50, 0, 0, nullptr);

	if (AnimatedClantag && GetTickCount() > next_anitag)
	{
		constexpr size_t size_clantags = ARRAYSIZE(clantags);

		const char* clantagbuff;

		if (ClantagStyle < size_clantags)
			clantagbuff = clantags[ClantagStyle];
		else
		{
			switch (ClantagStyle)
			{
				case size_clantags:
				{
					if (current_tag_index >= ARRAYSIZE(nex_clantag_strings))
						current_tag_index = 0;

					clantagbuff = nex_clantag_strings[current_tag_index++];
					break;
				}

				case size_clantags + 1:
				{
					if (current_tag_index >= ARRAYSIZE(gaysense))
						current_tag_index = 0;

					clantagbuff = gaysense[current_tag_index++];
					break;
				}

				case size_clantags + 2:
				{
					if (current_tag_index >= ARRAYSIZE(blendS))
						current_tag_index = 0;

					clantagbuff = blendS[current_tag_index++];
					break;
				}

				default:
					clantagbuff = "";
			}
		}

		next_anitag = GetTickCount() + 800;
		U::SendClantagChanged(clantagbuff, clantagbuff);
	}

	auto local_player = I::EntityList->GetClientEntity(I::Engine->GetLocalPlayer());

	if ( local_player && local_player->IsAlive() && local_player->m_nMoveType() != 9) // b1g ladder f1x
	{
		if (BunnyHop && (cmd->buttons & IN_JUMP))
		{
			if (!(local_player->m_fFlags() & FL_ONGROUND))
				cmd->buttons &= ~IN_JUMP;
		}

		if (UnlimitedDuck || C::LegitAimbot.chvh)
			cmd->buttons |= IN_BULLRUSH;

		if (Slidewalk)
		{
			cmd->buttons &= ~(IN_FORWARD | IN_BACK | IN_MOVELEFT | IN_MOVERIGHT);

			// both are in opposite direction
			// to trigger the meme walk animation
			if (cmd->forwardmove != 0.f)
			{
				cmd->buttons |= (cmd->forwardmove > 0.f) ? IN_BACK : IN_FORWARD;
			}

			if (cmd->sidemove != 0.f)
			{
				cmd->buttons |= (cmd->sidemove > 0.f) ? IN_MOVELEFT : IN_MOVERIGHT;
			}
		}

		if (BlockBot)
		{
			C_CSPlayer* target = nullptr;
			float       bestdist = 250.f;

			for (int i = 1; i < I::Engine->GetMaxClients(); i++)
			{
				C_CSPlayer* entity = I::EntityList->GetClientEntity(i);

				if (!entity)
					continue;

				if (!entity->IsAlive()
					|| entity->IsDormant()
					|| entity == local_player)
					continue;

				float dist = local_player->m_vecOrigin().DistTo(entity->m_vecOrigin());

				if (dist < bestdist)
				{
					bestdist = dist;
					target = entity;
				}
			}

			if (!target)
				return;

			float yaw_diff = M::CalcAngle(local_player->m_vecOrigin(), target->m_vecOrigin()).yaw;
			
			yaw_diff -= local_player->m_angEyeAngles().yaw;
			M::NormalizeYaw(&yaw_diff);

			if (yaw_diff != 0.0f)
				cmd->sidemove = (yaw_diff > 0.0f) ? -450.f : 450.f;
		}

		// will do this later
		/*
		if (OnTopBlockbot)
		{
			for (int i = 1; i < I::Engine->GetMaxClients(); i++)
			{
				C_CSPlayer* entity = I::EntityList->GetClientEntity(i);

				if (!entity)
					continue;

				if (!entity->IsAlive()
					|| entity->IsDormant()
					|| entity == local_player)
					continue;

				float dist = local_player->m_vecOrigin().DistTo(entity->m_vecOrigin());

				if (dist > 32.f)
				{
					continue;
				}

				Vector top = entity->m_vecOrigin();
				bool   jumping = !(entity->m_fFlags() & FL_ONGROUND) && entity->m_nMoveType() != MOVETYPE_LADDER;

				if (jumping)
					top.z += 10.f;

				top.z += 72.f + (-18.f * entity->m_flDuckAmount());

				if (local_player->m_vecOrigin().z < top.z)
				{
					continue;
				}

				// we only need to do this once, though
				// todo: follow y movement

				break;
			}
		}
		*/
	}
}
