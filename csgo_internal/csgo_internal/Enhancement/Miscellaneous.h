#pragma once
#include <Windows.h>
#include "..\SDK\SDK.h"

class CMiscellaneous
{
private:
	DWORD next_anitag = 0;
	size_t current_tag_index = 0;

public:
	void CreateMove(CUserCmd* cmd, bool* send_packet);

public:
	bool RankRevealer = true;

	int ClantagStyle = 0;
	bool AnimatedClantag = false;

	bool BunnyHop = false;
	bool UnlimitedDuck = false;
	bool Slidewalk = false;

	bool BlockBot = false;
	bool OnTopBlockbot = false;
};
