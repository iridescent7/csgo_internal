#pragma once
#include "..\interfaces\vfunc.h"

class CHudChat
{
public:
	enum ChatFilters
	{
		CHAT_FILTER_NONE = 0,
		CHAT_FILTER_JOINLEAVE = 0x000001,
		CHAT_FILTER_NAMECHANGE = 0x000002,
		CHAT_FILTER_PUBLICCHAT = 0x000004,
		CHAT_FILTER_SERVERMSG = 0x000008,
		CHAT_FILTER_TEAMCHANGE = 0x000010,
		//=============================================================================
		// HPE_BEGIN:
		// [tj]Added a filter for achievement announce
		//=============================================================================
 
		CHAT_FILTER_ACHIEVEMENT = 0x000020,
 
		//=============================================================================
		// HPE_END
		//=============================================================================
	};
 
	template <typename ...Args>
	void ChatPrintfA(int iPlayerIndex, int iFilter, const char* format, Args... arg)
	{
		GetVFunc<void(__cdecl*)(void* hudchat, int iPlayerIndex, int iFilter, const char* format, Args... arg)>(this, 27)(this, iPlayerIndex, iFilter, format, arg...);
	}

	template <typename ...Args>
	void ChatPrintfW(int iPlayerIndex, int iFilter, const wchar_t* format, Args... arg)
	{
		GetVFunc<void(__thiscall*)(void* hudchat, int iPlayerIndex, int iFilter, const wchar_t* format, Args... arg)>(this, 28)(this, iPlayerIndex, iFilter, format, arg...);
	}
};