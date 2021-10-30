#include "..\hooks.h"
#include "..\globals.h"

void __fastcall hooks::Hooked_EmitSound(IEngineSound* enginesound, void* edx, IRecipientFilter& filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample,
	float flVolume, float flAttenuation, int iFlags, int iPitch, int iSpecialDSP,
	const Vector* pOrigin, const Vector* pDirection, CUtlVector<Vector>* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, int unk)
{
	hooks::oEmitSound(enginesound, filter, iEntIndex, iChannel, pSoundEntry, nSoundEntryHash, pSample,
		flVolume, flAttenuation, iFlags, iPitch, iSpecialDSP, pOrigin,
		pDirection, pUtlVecOrigins, bUpdatePositions, soundtime, speakerentity, unk);
}
