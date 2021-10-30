#include "Util.h"
#include "WinAPI.h"
#include <Psapi.h>
#include <algorithm>
#include <string>

namespace U
{
#define INRANGE(x,a,b)  (x >= a && x <= b)
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

	DWORD FindSignature(const char* moduleName, const char* signature)
	{
		MODULEINFO modInfo;
		K32GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(moduleName), &modInfo, sizeof(MODULEINFO));

		const char* sig = signature;

		DWORD begin = (DWORD)modInfo.lpBaseOfDll;
		DWORD end = begin + modInfo.SizeOfImage;

		DWORD firstMatch = 0;
		for (DWORD pCur = begin; pCur < end; ++pCur)
		{
			if (!*sig)
				return firstMatch;

			if (*(PBYTE)sig == '\?' || *(BYTE*)pCur == getByte(sig))
			{
				if (!firstMatch) firstMatch = pCur;
				if (!sig[2]) return firstMatch;
				if (*(PWORD)sig == '\?\?' || *(PBYTE)sig != '\?') sig += 3;
				else sig += 2;
			}
			else
			{
				sig = signature;
				firstMatch = 0;
			}
		}
		return NULL;
	}

	void TraceLine(const Vector& start, const Vector& end, unsigned int mask, ITraceFilter* filter, trace_t* tr)
	{
		Ray_t ray;
		ray.Init(start, end);

		I::Trace->TraceRay(ray, mask, filter, tr);
	}

	IMaterial* CreateMaterial(const char* type, const char* vmt)
	{
		static size_t created = 0;

		std::string name = "vanity_n" + std::to_string(created);
		KeyValues*  kv = new KeyValues;

		U::InitKeyValues(kv, type);
		U::LoadFromBuffer(kv, name.c_str(), vmt);

		IMaterial* pMaterial = I::MaterialSystem->CreateMaterial(name.c_str(), kv);

		pMaterial->IncrementReferenceCount();
		++created;

		return pMaterial;
	}

	int GetSequenceActivity(C_CSPlayer* entity, int sequence)
	{
		auto hdr = I::ModelInfo->GetStudioModel(entity->GetModel());

		if (!hdr)
			return -1;

		using GetSequenceActivityFn = int(__fastcall*)(void*, studiohdr_t*, int);

		return ((GetSequenceActivityFn)O::getSequenceActivity)(entity, hdr, sequence);
	}

	float GetMaxDesyncDelta(CCSPlayerAnimState* anim_state)
	{
		const auto speed_factor   = max(0.f, min(1, anim_state->m_flSpeedFactor));
		const auto speed_fraction = max(0.f, min(anim_state->m_flSpeedFraction, 1.f));

		const auto unk1 = (anim_state->m_flLandingRatio * -0.30000001 - 0.19999999) * speed_fraction;
		float unk2 = unk1 + 1.f;

		if (anim_state->m_flDuckAmount > 0)
			unk2 += anim_state->m_flDuckAmount * speed_factor * (0.5f - unk2);

		return anim_state->m_flMaxBodyYawDegrees * unk2;
	}

	void InitKeyValues(KeyValues* kv, const char* name)
	{
		using InitKeyValuesFn = void(__thiscall*)(void* thisptr, const char* name);

		((InitKeyValuesFn)O::initKeyValues)(kv, name);
	}

	void LoadFromBuffer(KeyValues* kv, const char* name, const char* buffer)
	{
		using LoadFromBufferFn = void(__thiscall*)(
			void* thisptr, const char* resourceName,
			const char* pBuffer, void* pFileSystem,
			const char* pPathID, void* pfnEvaluateSymbolProc, void* a1);

		((LoadFromBufferFn)O::loadFromBuffer)(kv, name, buffer, nullptr, nullptr, nullptr, nullptr);
	}

	void ServerRankRevealAll()
	{
		using ServerRankRevealAllFn = bool(__cdecl*)(float*);

		float fArray[3] = { 0.f, 0.f, 0.f };
		((ServerRankRevealAllFn)O::serverRankRevealAll)(fArray);
	}

	void SendClantagChanged(const char* tag, const char* name)
	{
		using SendClantagChangedFn = void(__fastcall*)(const char*, const char*);

		((SendClantagChangedFn)O::sendClantagChanged)(tag, name);
	}
}
