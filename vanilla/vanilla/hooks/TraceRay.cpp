#include "..\hooks.h"
#include "..\globals.h"

void __fastcall hooks::Hooked_TraceRay(IEngineTrace* enginetrace, void* edx, const Ray_t& ray, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace)
{
	return hooks::oTraceRay(enginetrace, ray, fMask, pTraceFilter, pTrace);
}
