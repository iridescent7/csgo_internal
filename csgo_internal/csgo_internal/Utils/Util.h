#pragma once
#include <Windows.h>
#include "..\SDK\SDK.h"

namespace U
{
	inline int ConvertANSIToUnicode(const char* ansi, wchar_t* unicode, int unicodeBufferSize)
	{
		return ::MultiByteToWideChar(CP_UTF8, 0, ansi, -1, unicode, unicodeBufferSize);
	}

	DWORD  FindSignature(const char* moduleName, const char* signature);
	void   TraceLine(const Vector& start, const Vector& end, unsigned int mask, ITraceFilter* filter, trace_t* tr);

	IMaterial* CreateMaterial(const char* type, const char* vmt);

	int GetSequenceActivity(C_CSPlayer* entity, int sequence);
	float GetMaxDesyncDelta(CCSPlayerAnimState* anim_state);

	void InitKeyValues(KeyValues* kv, const char* name);
	void LoadFromBuffer(KeyValues* kv, const char* name, const char* buffer);

	void ServerRankRevealAll();
	void SendClantagChanged(const char* tag, const char* name);
}
