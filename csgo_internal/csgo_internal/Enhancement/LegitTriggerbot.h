#pragma once

#include "../SDK/SDK.h"
#include "../Utils/Util.h"

class CLegitTriggerbot
{
	public:
		void CreateMove(CUserCmd*, bool*);
		void EndScene();

	public:
		bool  Enabled = false;
		float delayAmount;
		bool  autoStop;
		int   key = 0x4;

		int   hitchance = 0;
		int   mindamage = 10;

	private:
		float  lastTime {0.0f};
		Vector dst;
};