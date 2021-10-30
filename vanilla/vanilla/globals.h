#pragma once
#include <Windows.h>
#include <windowsx.h>
#include <d3dx9.h>
#include "sdk.h"
#include "winapi.h"
#include "hooks.h"
#include "vars.h"
#include "menu.h"
#include "utils.h"

extern HMODULE g_dll;
extern HWND    g_window;

extern IVEngineClient*       g_engine;
extern IBaseClientDLL*       g_client;
extern IClientEntityList*    g_entitylist;
extern IEngineTrace*         g_enginetrace;
extern ICvar*                g_cvar;
extern IPanel*               g_panel;
extern IVDebugOverlay*       g_debugoverlay;
extern ISurface*             g_surface;
extern IVModelInfoClient*    g_modelinfo;
extern IVModelRender*        g_modelrender;
extern IVRenderView*         g_renderview;
extern IMaterialSystem*      g_materialsys;
extern IGameEventManager2*   g_gameeventmgr2;
extern IMDLCache*            g_mdlcache;
extern IPrediction*          g_prediction;
extern CGameMovement*        g_gamemovement;
extern IEngineSound*         g_enginesound;
extern IPhysicsSurfaceProps* g_physsurface;
extern IInputSystem*         g_inputsystem;

extern IClientMode*          g_clientmode;
extern CGlobalVarsBase*      g_globalvars;
extern CGlowObjectManager*   g_glowobjectmanager;
extern CInput*               g_input;
extern IViewRender*          g_viewrender;
extern CClientState*         g_clientstate;
extern IMoveHelper*          g_movehelper;
extern CHud*                 g_hud;
extern CHudChat*             g_hudchat;

namespace hooks
{
	extern SceneEndFn            oSceneEnd;
	extern TraceRayFn            oTraceRay;
	extern LockCursorFn          oLockCursor;
	extern CreateMoveFn          oCreateMove;
	extern DrawSetColorFn        oDrawSetColor;
	extern OverrideViewFn        oOverrideView;
	extern FireGameEventFn       oFireGameEvent;
	extern ListLeavesInBoxFn     oListLeavesInBox;
	extern PaintTraverseFn       oPaintTraverse;
	extern FrameStageNotifyFn    oFrameStageNotify;
	extern DrawModelExecuteFn    oDrawModelExecute;
	extern GetViewmodelFovFn     oGetViewmodelFov;
	extern ConVar_GetIntFn       oSvCheats_GetInt;
	extern EmitSoundFn           oEmitSound;
	extern ClientCmdFn           oClientCmd;
	extern DispatchUserMessageFn oDispatchUserMessage;

	extern ResetFn   oReset;
	extern PresentFn oPresent;

	extern WNDPROC oWndProc;
}

class C_BasePlayer;
class C_LocalPlayer
{
public:
	C_LocalPlayer() : m_local(nullptr) {}

	operator bool() const { return *m_local != nullptr; }
	operator C_BasePlayer* () const { return *m_local; }

	C_BasePlayer* operator->() { return *m_local; }
	bool operator==(void* ptr) { return *m_local == ptr; }

private:
	C_BasePlayer** m_local;
};

struct dpi_t
{
	float x, y;
};

extern C_LocalPlayer  g_localplayer;

extern D3DX9Renderer* g_renderer;
extern bool           g_renderer_init;

extern D3DVIEWPORT9   g_viewport;
extern dpi_t          g_dpi;

extern IDirect3DStateBlock9* g_stateblock;

extern CheatFonts     g_fonts;
extern CheatColors    g_colors;
extern CheatMenu      g_menu;
extern CheatVariables g_vars;
extern CheatHotkeys   g_hotkeys;

extern Control* g_focus;