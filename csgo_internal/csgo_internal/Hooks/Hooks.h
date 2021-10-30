#pragma once

#include <d3d9.h>
#include <d3dx9.h>
#include "..\SDK\SDK.h"
#include "..\detours.h"

using CreateMoveFn = bool(__stdcall*)(float, CUserCmd*);
using PaintTraverseFn =  void(__thiscall*)(void*, unsigned int, bool, bool);

using FrameStageNotifyFn = void(__thiscall*)(void*, int);
using FireGameEventFn = bool(__thiscall*)(void*, IGameEvent*);
using OverrideViewFn = void(__thiscall*)(void* ecx, CViewSetup* pSetup);

using SceneEndFn = void(__thiscall*)(void*);
using DrawModelExecuteFn = void(__thiscall*)(void*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);

using ResetFn = HRESULT(__stdcall*)(D3DPRESENT_PARAMETERS* pPresentParameters);
using EndSceneFn = HRESULT(__stdcall*)(IDirect3DDevice9* pDevice);

using TraceRayFn = void(__thiscall*)(void*, const Ray_t &ray, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace);
using GetViewmodelFovFn = float(__thiscall*)();
using LockCursorFn = void(__thiscall*)(void*);

using DrawSetColorFn = void(__thiscall*)(void* thisptr, int r, int g, int b, int a);
using ListLeavesInBoxFn = int(__thiscall*)(void*, const Vector&, const Vector&, unsigned short*, int);

using ClientCMDFn = void(__thiscall*)(IEngine* ecx, const char* szCmdString);

namespace H
{
	extern bool D3DInit;

	extern CreateMoveFn oCreateMove;
	extern PaintTraverseFn oPaintTraverse;

	extern FrameStageNotifyFn oFrameStageNotify;
	extern FireGameEventFn oFireGameEvent;
	extern OverrideViewFn oOverrideView;

	extern SceneEndFn oSceneEnd;
	extern DrawModelExecuteFn oDrawModelExecute;

	extern ResetFn oReset;
	extern EndSceneFn oEndScene;

	extern TraceRayFn oTraceRay;
	extern GetViewmodelFovFn oGetViewmodelFov;
	extern LockCursorFn oLockCursor;

	extern DrawSetColorFn oDrawSetColor;
	extern ListLeavesInBoxFn oListLeavesInBox;

	extern ClientCMDFn oClientCmd;

	extern WNDPROC oWndProc;

	void SetupHooks();

	extern bool __stdcall   Hooked_CreateMove(float sample_input_frametime, CUserCmd* cmd);
	extern void __stdcall   Hooked_PaintTraverse(unsigned int vguiPanel, bool forceRepaint, bool allowForce);

	extern void __fastcall  Hooked_FrameStageNotify(void* thisptr, void* edx, int stage);
	extern bool __fastcall  Hooked_FireGameEvent(void* thisptr, void* edx, IGameEvent* pEvent);
	extern void __fastcall  Hooked_OverrideView(void* thisptr, void* edx, CViewSetup* pSetup);

	extern void __fastcall  Hooked_SceneEnd(void* thisptr, void* edx);
	extern void __fastcall  Hooked_DrawModelExecute(void* thisptr, void* edx, IMatRenderContext* context, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* pCustomBoneToWorld);

	extern HRESULT __stdcall Hooked_Reset(D3DPRESENT_PARAMETERS* pPresentParameters);
	extern HRESULT __stdcall Hooked_EndScene(IDirect3DDevice9* pDevice);

	extern void __fastcall  Hooked_TraceRay(void* thisptr, void* edx, const Ray_t& ray, unsigned int fMask, ITraceFilter *pTraceFilter, trace_t *pTrace);
	extern float __fastcall Hooked_GetViewmodelFov();
	extern void __fastcall  Hooked_LockCursor(void* thisptr, void* edx);

	extern void __fastcall  Hooked_DrawSetColor(void* thisptr, void* edx, int r, int g, int b, int a);
	extern int __fastcall   Hooked_ListLeavesInBox(void* bsp, void* edx, Vector& mins, Vector& maxs, unsigned short* pList, int listMax);

	extern void __fastcall Hooked_ClientCmd(IEngine* ecx, void* edx, const char* szCmdString);

	extern LRESULT __stdcall Hooked_WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
}
