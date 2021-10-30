#include "Interfaces.h"
#include "Globals.h"

namespace I
{
	IClient* Client;
	CInput* Input;
	CPanel* Panel;
	ISurface* Surface;
	IEngine* Engine;
	CClientEntityList* EntityList;
	CGlobalVars* Globals;
	IVDebugOverlay* DebugOverlay;
	CTrace* Trace;
	CModelInfo* ModelInfo;
	ICVar* Cvar;
	CInputSystem* InputSystem;
	CModelRender* ModelRender;
	CRenderView* RenderView;
	IMaterialSystem* MaterialSystem;
	IGameEventManager* GameEventManager;
	IViewRenderBeams* ViewRenderBeams;
	CGlowObjectManager* GlowObjManager;
	CPrediction* Prediction;
	IGameMovement* GameMovement;
	IMoveHelper* MoveHelper;
	IClientModeShared* ClientMode;
	CNetworkStringTableContainer* NetStrTableContainer; //custommodels
	IPhysicsSurfaceProps* PhysSurface;

	HMODULE GetModuleHandleSafeA(const char* library)
	{
		HMODULE handle = nullptr;

		while (!(handle = GetModuleHandleA(library)))
			Sleep(1);

		return handle;
	}

	template <typename T> T* CaptureInterface(const char* library, const char* partialName)
	{
		DWORD dwCreateInterface = (DWORD)GetProcAddress(GetModuleHandleSafeA(library), "CreateInterface");

		DWORD jmp_displacement_addr = dwCreateInterface + 0x5;
		DWORD dwCreateInterfaceInternal = *reinterpret_cast<long*>(jmp_displacement_addr) + jmp_displacement_addr + sizeof(long);

		DWORD interface_list = *reinterpret_cast<DWORD*>(dwCreateInterfaceInternal + 0x6);

		InterfaceReg* pCur = *reinterpret_cast<InterfaceReg**>(interface_list);

		while (pCur)
		{
			if (std::string(pCur->m_pName).find(partialName) != std::string::npos)
			{
				return reinterpret_cast<T*>(pCur->m_CreateFn());
			}

			pCur = pCur->m_pNext;
		}
		return nullptr;
	}

	void SetupInterfaces()
	{
		Client = CaptureInterface<IClient>("client.dll", "VClient0");
		Panel = CaptureInterface<CPanel>("vgui2.dll", "VGUI_Panel0");
		Surface = CaptureInterface<ISurface>("vguimatsurface", "VGUI_Surface0");
		Engine = CaptureInterface<IEngine>("engine.dll", "VEngineClient0");
		EntityList = CaptureInterface<CClientEntityList>("client.dll", "VClientEntityList0");
		DebugOverlay = CaptureInterface<IVDebugOverlay>("engine.dll", "VDebugOverlay0");
		Trace = CaptureInterface<CTrace>("engine.dll", "EngineTraceClient0");
		ModelInfo = CaptureInterface<CModelInfo>("engine.dll", "VModelInfoClient0");
		Cvar = CaptureInterface<ICVar>("vstdlib.dll", "VEngineCvar0");
		InputSystem = CaptureInterface<CInputSystem>("inputsystem.dll", "InputSystemVersion0");
		ModelRender = CaptureInterface<CModelRender>("engine.dll", "VEngineModel0");
		RenderView = CaptureInterface<CRenderView>("engine.dll", "VEngineRenderView0");
		MaterialSystem = CaptureInterface<IMaterialSystem>("materialsystem.dll", "VMaterialSystem0");
		PhysSurface = CaptureInterface<IPhysicsSurfaceProps>("vphysics.dll", "VPhysicsSurfaceProps0");
		GameEventManager = CaptureInterface<IGameEventManager>("engine.dll", "GAMEEVENTSMANAGER002");
		Prediction = CaptureInterface<CPrediction>("client.dll", "VClientPrediction001");
		GameMovement = CaptureInterface<IGameMovement>("client.dll", "GameMovement001");
		PhysSurface = CaptureInterface<IPhysicsSurfaceProps>("vphysics.dll", "VPhysicsSurfaceProps001");
		NetStrTableContainer = CaptureInterface<CNetworkStringTableContainer>("engine.dll", "VEngineClientStringTable001");

		Input = *(CInput**)(U::FindSignature("client.dll", "B9 ? ? ? ? F3 0F 11 04 24 FF 50 10") + 0x1);
		//ViewRender = **(IViewRender***)(U::FindSignature("client.dll", "FF 50 4C 8B 06 8D 4D F4") - 0x6);
		ViewRenderBeams = *(IViewRenderBeams**)(U::FindSignature("client.dll", "A1 ? ? ? ? 56 8B F1 B9 ? ? ? ? FF 50 08") + 0x1);
		GlowObjManager = *(CGlowObjectManager**)(U::FindSignature("client.dll", "0F 11 05 ? ? ? ? 83 C8 01") + 0x3);
		MoveHelper = **(IMoveHelper***)(U::FindSignature("client.dll", "8B 0D ? ? ? ? 8B 46 08 68") + 0x2);

		Globals = **reinterpret_cast<CGlobalVars***>((*reinterpret_cast<uintptr_t**>(Client))[11] + 10);
		ClientMode = **(IClientModeShared***)((*(DWORD**)Client)[10] + 0x5);

		Engine->GetScreenSize(G::width, G::height);

		G::x88Font = Surface->CreateSourceFont();
		Surface->SetFont(G::x88Font, "Tahoma", 12, 500, 0, 0, FONTFLAG_OUTLINE);

		//MemAlloc = *(IMemAlloc**)(GetProcAddress(GetModuleHandle("tier0.dll"), "g_pMemAlloc");
		//Enginevgui = CaptureInterface<EngineVGui>("engine.dll", "VEngineVGui001");
	}
}