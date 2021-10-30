#include "Hooks.h"
#include "..\SDK\SDK.h"
#include "..\Globals.h"

namespace H
{
	void __fastcall Hooked_ClientCmd(IEngine* ecx, void* edx, const char* szCmdString)
	{
		if (I::Engine->IsInGame() && cdk::strcheck(szCmdString, "say"))
		{
			const char* _szCmdString = szCmdString;

			_szCmdString += _szCmdString[3] == '_' ? 10 : 5;

			if (cdk::strcheck(_szCmdString, "!clantag"))
			{
				_szCmdString += sizeof("!clantag");

				char buffer[33] = { '\0' };

				int idx = 0;
				for (; idx < 32; idx++)
				{
					if (!_szCmdString[idx])
						break;

					buffer[idx] = _szCmdString[idx];
				}

				//strcpy_s(buffer, 17, _szCmdString);
				buffer[idx - 1] = '\0';

				U::SendClantagChanged(buffer, buffer);

				return;
			}
		}

		H::oClientCmd(ecx, szCmdString);
	}
}