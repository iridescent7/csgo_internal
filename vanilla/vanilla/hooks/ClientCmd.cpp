#include "..\hooks.h"
#include "..\globals.h"

#define chat_msg(s) charenc(" \x04[\x01Vanilla\x04]\x01 " s)

void __fastcall hooks::Hooked_ClientCmd(IVEngineClient* engine, void* edx, const char* cmdstring)
{
	/*
	std::string cmdstr (cmdstring);

	auto szCmdParse = [&, cmdstr](const char* match, int pos) -> bool
	{
		if (!cmdstr.substr(pos, strlen(match)).compare(match)) return true;
		return false;
	};

	// Check client for sending a chat message
	if (szCmdParse(charenc("say"), 0))
	{                              // lol
		UINT offset = cmdstring[3] == charenc('_') ? 5 : 0; // Add offset to char pointer if it's a team chat

		// Captures Vanilla chat commands with the prefix !vm <command> <parameter>. statement checks for "!vm ".
		if (szCmdParse(charenc("\"!vm "), 4 + offset))
		{
			if (szCmdParse(charenc("trans"), 9 + offset))
			{
				g_hudchat->ChatPrintfA(0, 0, chat_msg("\x04Translating..."));
			}
			else
				g_hudchat->ChatPrintfA(0, 0, chat_msg("Unknown command!"));
		}
	}
	*/

	return hooks::oClientCmd(engine, cmdstring);
}