#pragma once
#include <Windows.h>
#include <d3d9.h>
#include "sdk.h"

using SceneEndFn            = void(__thiscall*)(IVRenderView* renderview);
using TraceRayFn            = void(__thiscall*)(IEngineTrace* enginetrace, const Ray_t& ray, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace);
using LockCursorFn          = void(__thiscall*)(ISurface* surface);
using CreateMoveFn          = bool(__thiscall*)(IClientMode* clientmode, float sample_input_frametime, CUserCmd* cmd);
using DrawSetColorFn        = void(__thiscall*)(ISurface* surface, int r, int g, int b, int a);
using OverrideViewFn        = void(__thiscall*)(IClientMode* clientmode, CViewSetup* viewsetup);
using FireGameEventFn       = bool(__thiscall*)(IGameEventManager2* gameeventmanager2, IGameEvent* gameevent);
using PaintTraverseFn       = void(__thiscall*)(IPanel* panel, unsigned int vguiPanel, bool forceRepaint, bool allowForce);
using FrameStageNotifyFn    = void(__thiscall*)(IBaseClientDLL* client, int stage);
using DrawModelExecuteFn    = void(__thiscall*)(IVModelRender* modelrender, IMatRenderContext* context, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* pCustomBoneToWorld);
using ListLeavesInBoxFn     = int (__thiscall*)(void* bsptreequery, Vector& mins, Vector& maxs, unsigned short* pList, int listMax);
using GetViewmodelFovFn     = float(__thiscall*)(IClientMode* clientmode);
using ConVar_GetIntFn       = int (__thiscall*)(ConVar* convar);
using ClientCmdFn           = void(__thiscall*)(IVEngineClient* engine, const char* cmdstring);
using DispatchUserMessageFn = bool(__thiscall*)(IBaseClientDLL* client, UINT msg_type, UINT pad1, UINT bytes, const void* msg_data);

using EmitSoundFn = void(__thiscall*)(IEngineSound* enginesound, IRecipientFilter& filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char *pSample,
		float flVolume, float flAttenuation, int iFlags, int iPitch, int iSpecialDSP, 
		const Vector* pOrigin, const Vector* pDirection, CUtlVector<Vector>* pUtlVecOrigins, bool bUpdatePositions, float soundtime, int speakerentity, int unk);

using ResetFn   = HRESULT(__stdcall*)(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* PresentParameters);
using PresentFn = HRESULT(__stdcall*)(IDirect3DDevice9* device, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion);

namespace hooks
{
	extern void  __fastcall Hooked_SceneEnd(IVRenderView* renderview);
	extern void  __fastcall Hooked_TraceRay(IEngineTrace* enginetrace, void* edx, const Ray_t& ray, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace);
	extern void  __fastcall Hooked_LockCursor(ISurface* surface);
	extern bool  __fastcall Hooked_CreateMove(IClientMode* clientmode, void* edx, float sample_input_frametime, CUserCmd* cmd);
	extern void  __fastcall Hooked_DrawSetColor(ISurface* surface, void* edx, int r, int g, int b, int a);
	extern void  __fastcall Hooked_OverrideView(IClientMode* clientmode, void* edx, CViewSetup* viewsetup);
	extern bool  __fastcall Hooked_FireGameEvent(IGameEventManager2* gameeventmanager2, void* edx, IGameEvent* gameevent);
	extern void  __fastcall Hooked_PaintTraverse(IPanel* panel, void* edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce);
	extern void  __fastcall Hooked_FrameStageNotify(IBaseClientDLL* client, void* edx, int stage);
	extern void  __fastcall Hooked_DrawModelExecute(IVModelRender* modelrender, void* edx, IMatRenderContext* context, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* pCustomBoneToWorld);
	extern int   __fastcall Hooked_ListLeavesInBox(void* bsptreequery, void* edx, Vector& mins, Vector& maxs, unsigned short* pList, int listMax);
	extern float __fastcall Hooked_GetViewmodelFov(IClientMode* clientmode);
	extern bool  __fastcall Hooked_SvCheats_GetInt(ConVar* sv_cheats);
	extern void  __fastcall Hooked_ClientCmd(IVEngineClient* engine, void* edx, const char* szCmdString);
	extern bool  __fastcall Hooked_DispatchUserMessage (IBaseClientDLL* client, void* edx, UINT msg_type, UINT pad1, UINT bytes, const void* msg_data);

	extern void  __fastcall Hooked_EmitSound(IEngineSound* enginesound, void* edx, IRecipientFilter& filter, int iEntIndex, int iChannel, const char* pSoundEntry, unsigned int nSoundEntryHash, const char* pSample,
		float flVolume, float flAttenuation, int iFlags = 0, int iPitch = PITCH_NORM, int iSpecialDSP = 0,
		const Vector* pOrigin = NULL, const Vector* pDirection = NULL, CUtlVector<Vector>* pUtlVecOrigins = NULL, bool bUpdatePositions = true, float soundtime = 0.0f, int speakerentity = -1, int unk = 0);

	extern HRESULT __stdcall Hooked_Reset  (IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentParameters);
	extern HRESULT __stdcall Hooked_Present(IDirect3DDevice9* device, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion);

	LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
}
