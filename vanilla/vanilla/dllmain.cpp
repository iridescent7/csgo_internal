#include <Windows.h>
#include <detours.h>
#include "globals.h"

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "detours.lib")

constexpr size_t cx_strlen(const char* str)
{
	size_t length = 0;
	for (; *str; ++str, ++length) {}
	return length;
}

#define xassert(x, m)   if(!x) { throw charenc(m); }
#define partial_hash(s) hash(s), cx_strlen(s)

HMODULE g_dll    = nullptr;
HWND    g_window = nullptr;

IVEngineClient*       g_engine        = nullptr;
IBaseClientDLL*       g_client        = nullptr;
IClientEntityList*    g_entitylist    = nullptr;
IEngineTrace*         g_enginetrace   = nullptr;
ICvar*                g_cvar          = nullptr;
IPanel*               g_panel         = nullptr;
IVDebugOverlay*       g_debugoverlay  = nullptr;
ISurface*             g_surface       = nullptr;
IVModelInfoClient*    g_modelinfo     = nullptr;
IVModelRender*        g_modelrender   = nullptr;
IVRenderView*         g_renderview    = nullptr;
IMaterialSystem*      g_materialsys   = nullptr;
IGameEventManager2*   g_gameeventmgr2 = nullptr;
IMDLCache*            g_mdlcache      = nullptr;
IPrediction*          g_prediction    = nullptr;
CGameMovement*        g_gamemovement  = nullptr;
IEngineSound*         g_enginesound   = nullptr;
IPhysicsSurfaceProps* g_physsurface   = nullptr;
IInputSystem*         g_inputsystem   = nullptr;

IClientMode*          g_clientmode        = nullptr;
CGlobalVarsBase*      g_globalvars        = nullptr;
CGlowObjectManager*   g_glowobjectmanager = nullptr;
CInput*               g_input             = nullptr;
IViewRender*          g_viewrender        = nullptr;
CClientState*         g_clientstate       = nullptr;
IMoveHelper*          g_movehelper        = nullptr;
CHud*                 g_hud               = nullptr;
CHudChat*             g_hudchat           = nullptr;

namespace hooks
{
	SceneEndFn            oSceneEnd            = nullptr;
	TraceRayFn            oTraceRay            = nullptr;
	LockCursorFn          oLockCursor          = nullptr;
	CreateMoveFn          oCreateMove          = nullptr;
	DrawSetColorFn        oDrawSetColor        = nullptr;
	OverrideViewFn        oOverrideView        = nullptr;
	FireGameEventFn       oFireGameEvent       = nullptr;
	ListLeavesInBoxFn     oListLeavesInBox     = nullptr;
	PaintTraverseFn       oPaintTraverse       = nullptr;
	FrameStageNotifyFn    oFrameStageNotify    = nullptr;
	DrawModelExecuteFn    oDrawModelExecute    = nullptr;
	GetViewmodelFovFn     oGetViewmodelFov     = nullptr;
	ConVar_GetIntFn       oSvCheats_GetInt     = nullptr;
	EmitSoundFn           oEmitSound           = nullptr;
	ClientCmdFn           oClientCmd	       = nullptr;
	DispatchUserMessageFn oDispatchUserMessage = nullptr;

	ResetFn   oReset   = nullptr;
	PresentFn oPresent = nullptr;

	WNDPROC oWndProc = nullptr;
}

C_LocalPlayer  g_localplayer;

D3DX9Renderer* g_renderer      = nullptr;
bool           g_renderer_init = false;

D3DVIEWPORT9   g_viewport = { 0, 0, 0, 0, 0.f, 0.f };
dpi_t          g_dpi      = { 96.f, 96.f };

IDirect3DStateBlock9* g_stateblock = nullptr;

CheatFonts     g_fonts;
CheatColors    g_colors;
CheatMenu      g_menu;
CheatVariables g_vars;
CheatHotkeys   g_hotkeys;

Control* g_focus = nullptr;

__forceinline void ExitCheat()
{
	if (g_dll)
		_FreeLibraryAndExitThread(g_dll, 0);
	else
		_ExitThread(0);
}

DWORD __stdcall InitializeCheat(LPVOID lpThreadParameter)
{
	char filename[MAX_PATH];
	_GetModuleFileNameA(NULL, filename, MAX_PATH);

	if (std::string(filename).find(strenc("\\Steam\\steamapps\\common\\Counter-Strike Global Offensive\\csgo.exe")) == std::string::npos)
	{
		ExitCheat();
	}

	g_window = _FindWindowA(charenc("Valve001"), charenc("Counter-Strike: Global Offensive"));

	if (!g_window)
	{
		ExitCheat();
	}

	long max_wait_count = 1200; // 120 seconds

	while (!_GetModuleHandleA(charenc("serverbrowser.dll")))
	{
		if (max_wait_count < 1)
		{
			ExitCheat();
		}

		--max_wait_count;
		_Sleep(100);
	}

	_Sleep(100);

	try
	{
		auto clientEntry    = utils::GetModuleEntry(hash("client_panorama.dll"));
		auto engineEntry    = utils::GetModuleEntry(hash("engine.dll"));
		auto valveStdEntry  = utils::GetModuleEntry(hash("vstdlib.dll"));
		auto vgui2Entry     = utils::GetModuleEntry(hash("vgui2.dll"));
		auto vguiEntry      = utils::GetModuleEntry(hash("vguimatsurface.dll"));
		auto matSysEntry    = utils::GetModuleEntry(hash("materialsystem.dll"));
		auto dataCacheEntry = utils::GetModuleEntry(hash("datacache.dll"));
		auto vphysicsEntry  = utils::GetModuleEntry(hash("vphysics.dll"));
		auto inputSysEntry  = utils::GetModuleEntry(hash("inputsystem.dll"));
		auto tier0Entry	    = utils::GetModuleEntry(hash("tier0.dll"));
		auto dx9apiEntry    = utils::GetModuleEntry(hash("shaderapidx9.dll"));

		xassert(clientEntry,    "client_panorama.dll is missing")
		xassert(engineEntry,             "engine.dll is missing")
		xassert(valveStdEntry,          "vstdlib.dll is missing")
		xassert(vgui2Entry,               "vgui2.dll is missing")
		xassert(vguiEntry,       "vguimatsurface.dll is missing")
		xassert(matSysEntry,     "materialsystem.dll is missing")
		xassert(dataCacheEntry,       "datacache.dll is missing")
		xassert(vphysicsEntry,         "vphysics.dll is missing")
		xassert(inputSysEntry,      "inputsystem.dll is missing")
		xassert(tier0Entry,               "tier0.dll is missing")
		xassert(dx9apiEntry,       "shaderapidx9.dll is missing")

		// setup 2 tier0 exported functions we're gonna use
		//ConMsg      = (ConMsgFn)     utils::GetLdrEntryExportFunctionByPartialNameHash(tier0Entry, partial_hash("?ConMsg@@"));
		//ConColorMsg = (ConColorMsgFn)utils::GetLdrEntryExportFunctionByPartialNameHash(tier0Entry, partial_hash("?ConColorMsg@@"));

		// setup interfaces
		// get interface list from respective modules
		InterfaceFactory clientFactory    = utils::GetInterfaceFactory(clientEntry);
		InterfaceFactory engineFactory    = utils::GetInterfaceFactory(engineEntry);
		InterfaceFactory valveStdFactory  = utils::GetInterfaceFactory(valveStdEntry);
		InterfaceFactory vgui2Factory	  = utils::GetInterfaceFactory(vgui2Entry);
		InterfaceFactory vguiFactory	  = utils::GetInterfaceFactory(vguiEntry);
		InterfaceFactory matSysFactory	  = utils::GetInterfaceFactory(matSysEntry);
		InterfaceFactory dataCacheFactory = utils::GetInterfaceFactory(dataCacheEntry);
		InterfaceFactory vphysicsFactory  = utils::GetInterfaceFactory(vphysicsEntry);
		InterfaceFactory inputSysFactory  = utils::GetInterfaceFactory(inputSysEntry);

		// initialize interfaces
		g_client        = (IBaseClientDLL*)	   clientFactory.GetInterface(partial_hash("VClient0"));
		g_entitylist    = (IClientEntityList*) clientFactory.GetInterface(partial_hash("VClientEntityList0"));
		g_prediction    = (IPrediction*)	   clientFactory.GetInterface(partial_hash("VClientPrediction001"));
		g_gamemovement  = (CGameMovement*)     clientFactory.GetInterface(partial_hash("GameMovement001"));

		g_mdlcache      = (IMDLCache*) dataCacheFactory.GetInterface(partial_hash("MDLCache0"));

		g_engine        = (IVEngineClient*)	    engineFactory.GetInterface(partial_hash("VEngineClient0"));
		g_modelinfo     = (IVModelInfoClient*)  engineFactory.GetInterface(partial_hash("VModelInfoClient0"));
		g_modelrender   = (IVModelRender*)	    engineFactory.GetInterface(partial_hash("VEngineModel0"));
		g_renderview    = (IVRenderView*)	    engineFactory.GetInterface(partial_hash("VEngineRenderView0"));
		g_enginetrace   = (IEngineTrace*)	    engineFactory.GetInterface(partial_hash("EngineTraceClient0"));
		g_debugoverlay  = (IVDebugOverlay*)     engineFactory.GetInterface(partial_hash("VDebugOverlay0"));
		g_gameeventmgr2 = (IGameEventManager2*) engineFactory.GetInterface(partial_hash("GAMEEVENTSMANAGER002"));
		g_enginesound   = (IEngineSound*)	    engineFactory.GetInterface(partial_hash("IEngineSoundClient0"));

		g_materialsys   = (IMaterialSystem*)	  matSysFactory  .GetInterface(partial_hash("VMaterialSystem0"));
		g_cvar          = (ICvar*)				  valveStdFactory.GetInterface(partial_hash("VEngineCvar0"));
		g_panel         = (IPanel*)				  vgui2Factory   .GetInterface(partial_hash("VGUI_Panel0"));
		g_surface       = (ISurface*)			  vguiFactory    .GetInterface(partial_hash("VGUI_Surface0"));
		g_physsurface   = (IPhysicsSurfaceProps*) vphysicsFactory.GetInterface(partial_hash("VPhysicsSurfaceProps001"));
		g_inputsystem   = (IInputSystem*)		  inputSysFactory.GetInterface(partial_hash("InputSystemVersion0"));

		// setup cs global variables
		// scan signatures for offsets
		DWORD dwInput       = utils::ModulePatternScan(clientEntry, charenc("B9 ? ? ? ? F3 0F 11 04 24 FF 50 10"));
		DWORD dwMoveHelper  = utils::ModulePatternScan(clientEntry, charenc("8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01"));
		DWORD dwViewRender  = utils::ModulePatternScan(clientEntry, charenc("A1 ? ? ? ? B9 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? FF 10"));
		DWORD dwLocalPlayer = utils::ModulePatternScan(clientEntry, charenc("8B 0D ? ? ? ? 83 FF FF 74 07"));
		DWORD dwHUD         = utils::ModulePatternScan(clientEntry, charenc("B9 ? ? ? ? E8 ? ? ? ? 85 C0 0F 84 ? ? ? ? 83 C0 EC 0F 84 ? ? ? ? 80 B8"));

		DWORD dwGlowObjectManager = utils::ModulePatternScan(clientEntry, charenc("0F 11 05 ? ? ? ? 83 C8 01"));

		DWORD dwClientState = utils::ModulePatternScan(engineEntry, charenc("A1 ? ? ? ? 8B 80 ? ? ? ? C3"));
		DWORD dwD3DDevice   = utils::ModulePatternScan(dx9apiEntry, charenc("A1 ? ? ? ? 50 8B 08 FF 51 0C"));

		// sanity checks
		//xassert(ConMsg,           "?ConMsg@@ is missing from tier0.dll")
		//xassert(ConColorMsg, "?ConColorMsg@@ is missing from tier0.dll")

		// interfaces
		xassert(g_client,          "IBaseClientDll interface not found")
		xassert(g_entitylist,   "IClientEntityList interface not found")
		xassert(g_prediction,         "IPrediction interface not found")
		xassert(g_gamemovement,     "CGameMovement interface not found")

		xassert(g_mdlcache,             "IMDLCache interface not found")

		xassert(g_engine,            "IVEngineClient interface not found")
		xassert(g_modelinfo,      "IVModelInfoClient interface not found")
		xassert(g_modelrender,        "IVModelRender interface not found")
		xassert(g_renderview,          "IVRenderView interface not found")
		xassert(g_enginetrace,         "IEngineTrace interface not found")
		xassert(g_debugoverlay,      "IVDebugOverlay interface not found")
		xassert(g_gameeventmgr2, "IGameEventManager2 interface not found")
		xassert(g_enginesound,         "IEngineSound interface not found")

		xassert(g_materialsys,      "IMaterialSystem interface not found")
		xassert(g_cvar,                       "ICvar interface not found")
		xassert(g_panel,                     "IPanel interface not found")
		xassert(g_surface,                 "ISurface interface not found")
		xassert(g_physsurface, "IPhysicsSurfaceProps interface not found")
		xassert(g_inputsystem,         "IInputSystem interface not found")
			
		// cs globals
		xassert(dwInput,             "dwInput signature doesnt match")
		xassert(dwMoveHelper,   "dwMoveHelper signature doesnt match")
		xassert(dwViewRender,   "dwViewRender signature doesnt match")
		xassert(dwClientState, "dwClientState signature doesnt match")
		xassert(dwLocalPlayer, "dwLocalPlayer signature doesnt match")
		xassert(dwD3DDevice,     "dwD3DDevice signature doesnt match")

		xassert(dwGlowObjectManager, "dwGlowObjectManager signature doesnt match")

		// initialize our csgo exclusive global variables
		try
		{
			g_globalvars	  = **(CGlobalVarsBase***)(GetVFunc<DWORD>(g_client, 0)  + 0x1B);
			g_clientmode	  = **(IClientMode***)    (GetVFunc<DWORD>(g_client, 10) + 0x5);

			g_input           =  *(CInput**)       (dwInput + 1);
			g_movehelper      = **(IMoveHelper***) (dwMoveHelper + 2);
			g_viewrender      =  *(IViewRender**)  (dwViewRender + 1);
			g_localplayer	  =  *(C_LocalPlayer*) (dwLocalPlayer + 2);
			g_hud             =  *(CHud**)         (dwHUD + 1);

			g_glowobjectmanager  =  *(CGlowObjectManager**) (dwGlowObjectManager + 3);

			g_clientstate     = **(CClientState***)  (dwClientState + 1);
			g_renderer        = **(D3DX9Renderer***) (dwD3DDevice + 1);
		}
		catch (...)
		{
			throw charenc("error: bad global signatures");
		}

		// Initialize the chat hud from the CHud pointer by obtaining the Hud element
		g_hudchat = (CHudChat*) g_hud->FindElement(charenc("CHudChat"));

		xassert(g_hudchat, "CHudChat not found");
		
		// get original virtual functions
		hooks::oSceneEnd            = GetVFunc<SceneEndFn>            (g_renderview,    9);
		hooks::oTraceRay            = GetVFunc<TraceRayFn>            (g_enginetrace,   5);
		hooks::oLockCursor          = GetVFunc<LockCursorFn>          (g_surface,      67);
		hooks::oCreateMove          = GetVFunc<CreateMoveFn>          (g_clientmode,   24);
		hooks::oDrawSetColor        = GetVFunc<DrawSetColorFn>        (g_surface,      15);
		hooks::oOverrideView        = GetVFunc<OverrideViewFn>        (g_clientmode,   18);
		hooks::oFireGameEvent       = GetVFunc<FireGameEventFn>       (g_gameeventmgr2, 9);
		hooks::oPaintTraverse       = GetVFunc<PaintTraverseFn>       (g_panel,        41);
		hooks::oFrameStageNotify    = GetVFunc<FrameStageNotifyFn>    (g_client,       37);
		hooks::oDrawModelExecute    = GetVFunc<DrawModelExecuteFn>    (g_modelrender,  21);
		hooks::oGetViewmodelFov     = GetVFunc<GetViewmodelFovFn>     (g_clientmode,   35);
		hooks::oEmitSound           = GetVFunc<EmitSoundFn>           (g_enginesound,   5);
		hooks::oClientCmd           = GetVFunc<ClientCmdFn>           (g_engine,        7);
		hooks::oDispatchUserMessage = GetVFunc<DispatchUserMessageFn> (g_client,       38);

		auto sv_cheats          = g_cvar->FindVar(charenc("sv_cheats"));
		auto bsptreequery       = g_engine->GetBSPTreeQuery();

		hooks::oSvCheats_GetInt = GetVFunc<ConVar_GetIntFn>   (sv_cheats,   13);
		hooks::oListLeavesInBox = GetVFunc<ListLeavesInBoxFn> (bsptreequery, 6);

		hooks::oReset   = GetVFunc<ResetFn>   (g_renderer, 16);
		hooks::oPresent = GetVFunc<PresentFn> (g_renderer, 17);

		// initialize hooks
		LONG DetourBegin_ErrorCode = DetourTransactionBegin();

		if (DetourBegin_ErrorCode)
			throw (strenc("DetourTransactionBegin: ") + std::to_string(DetourBegin_ErrorCode)).c_str();
		
		DetourAttach((PVOID*)& hooks::oSceneEnd,            hooks::Hooked_SceneEnd);
		DetourAttach((PVOID*)& hooks::oTraceRay,            hooks::Hooked_TraceRay);
		DetourAttach((PVOID*)& hooks::oLockCursor,          hooks::Hooked_LockCursor);
		DetourAttach((PVOID*)& hooks::oCreateMove,          hooks::Hooked_CreateMove);
		DetourAttach((PVOID*)& hooks::oDrawSetColor,        hooks::Hooked_DrawSetColor);
		DetourAttach((PVOID*)& hooks::oOverrideView,        hooks::Hooked_OverrideView);
		DetourAttach((PVOID*)& hooks::oFireGameEvent,       hooks::Hooked_FireGameEvent);
		DetourAttach((PVOID*)& hooks::oPaintTraverse,       hooks::Hooked_PaintTraverse);
		DetourAttach((PVOID*)& hooks::oFrameStageNotify,    hooks::Hooked_FrameStageNotify);
		DetourAttach((PVOID*)& hooks::oDrawModelExecute,    hooks::Hooked_DrawModelExecute);
		DetourAttach((PVOID*)& hooks::oGetViewmodelFov,     hooks::Hooked_GetViewmodelFov);
		DetourAttach((PVOID*)& hooks::oEmitSound,           hooks::Hooked_EmitSound);
		DetourAttach((PVOID*)& hooks::oClientCmd,           hooks::Hooked_ClientCmd);
		DetourAttach((PVOID*)& hooks::oDispatchUserMessage, hooks::Hooked_DispatchUserMessage);
		DetourAttach((PVOID*)& hooks::oSvCheats_GetInt,     hooks::Hooked_SvCheats_GetInt);
		DetourAttach((PVOID*)& hooks::oListLeavesInBox,     hooks::Hooked_ListLeavesInBox);
		DetourAttach((PVOID*)& hooks::oReset,               hooks::Hooked_Reset);
		DetourAttach((PVOID*)& hooks::oPresent,             hooks::Hooked_Present);

		LONG DetourCommit_ErrorCode = DetourTransactionCommit();

		if (DetourCommit_ErrorCode)
			throw (strenc("DetourTransactionCommit: ") + std::to_string(DetourCommit_ErrorCode)).c_str();

		hooks::oWndProc = (WNDPROC)_SetWindowLongA(g_window, GWLP_WNDPROC, (LONG_PTR)hooks::WndProc);
	}
	catch (const char* err)
	{
		_MessageBoxA(g_window, err, charenc("Initialization failed"), MB_ICONERROR | MB_OK);
		ExitCheat();
	}

	return NULL;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		if (hinstDLL)
		{
			_DisableThreadLibraryCalls(hinstDLL);
			g_dll = hinstDLL;
		}
		_CreateThread(NULL, 0, InitializeCheat, nullptr, 0, NULL);
	}

	return TRUE;
}
