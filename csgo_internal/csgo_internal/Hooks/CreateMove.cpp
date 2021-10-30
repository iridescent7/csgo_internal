#include "Hooks.h"
#include "..\SDK\SDK.h"
#include "..\Globals.h"

namespace H
{
	bool __stdcall Hooked_CreateMove(float sample_input_frametime, CUserCmd* cmd)
	{
		if (!cmd || cmd->command_number == 0)
			return false;

		//if (C::sv_cheats.GetInt() == 0)
		//	C::sv_cheats.SetInt(1);

		bool* send_packet;
		
		__asm mov send_packet, ebp;
		send_packet = *(bool**)send_packet - 0x1C;

		// uhhh fix the pasted engine prediction (og: stackhack)
		//C::PredictionSystem.RunPrediction(cmd, local_player);

		

		C::VisualExtras.CreateMove(cmd, send_packet);
		C::PlayerEsp.CreateMove(cmd, send_packet);

		//C::Aimbot.CreateMove(cmd, send_packet);
		C::LegitAimbot.CreateMove(cmd, send_packet);
		C::LegitTriggerbot.CreateMove(cmd, send_packet);

		C::LegitBacktrack.CreateMove(cmd, send_packet);
		C::LegitMisc.CreateMove(cmd, send_packet);

		C::Miscellaneous.CreateMove(cmd, send_packet);
		C::AntiAim.CreateMove(cmd, send_packet);

		C::LAA.CreateMove(cmd, send_packet);

		//C::PredictionSystem.EndPrediction(local_player);

		// todo
		C::LegitAimbot.last_attack = C::LegitMisc.last_attack = (cmd->buttons & IN_ATTACK) || GetAsyncKeyState(VK_MENU);

		if (!GetAsyncKeyState(VK_MENU))
			C::LegitAimbot.locking = false;

		C::LastAngle.pitch = cmd->viewangles.pitch;
		C::LastAngle.yaw   = cmd->viewangles.yaw;
		C::LastAngle.roll  = 0.f;

		return false;
	}
}