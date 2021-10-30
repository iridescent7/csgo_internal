#pragma once

typedef void* (*InstantiateInterfaceFn)();
class InterfaceReg
{
public:
	InstantiateInterfaceFn	     m_CreateFn;
	const char				    *m_pName;

	InterfaceReg		    	*m_pNext;
};

class IClient;
class CInput;
class CPanel;
class ISurface;
class IEngine;
class CClientEntityList;
class CGlobalVars;
class IVDebugOverlay;
class CTrace;
class CModelInfo;
class ICVar;
class CInputSystem;
class CModelRender;
class CRenderView;
class IMaterialSystem;
class IGameEventManager;
class IViewRenderBeams;
class CGlowObjectManager;
class CPrediction;
class IGameMovement;
class IMoveHelper;
class IClientModeShared;
class IPhysicsSurfaceProps;
class CNetworkStringTableContainer;

namespace I
{
	extern IClient* Client;
	extern CInput* Input;
	extern CPanel* Panel;
	extern ISurface* Surface;
	extern IEngine* Engine;
	extern CClientEntityList* EntityList;
	extern CGlobalVars* Globals;
	extern IVDebugOverlay* DebugOverlay;
	extern CTrace* Trace;
	extern CModelInfo* ModelInfo;
	extern ICVar* Cvar;
	extern CInputSystem* InputSystem;
	extern CModelRender* ModelRender;
	extern CRenderView* RenderView;
	extern IMaterialSystem* MaterialSystem;
	extern IGameEventManager* GameEventManager;
	extern IViewRenderBeams* ViewRenderBeams;
	extern CGlowObjectManager* GlowObjManager;
	extern CPrediction* Prediction;
	extern IGameMovement* GameMovement;
	extern IMoveHelper* MoveHelper;
	extern IClientModeShared* ClientMode;
	extern IPhysicsSurfaceProps*  PhysSurface;
	extern CNetworkStringTableContainer* NetStrTableContainer;

	void SetupInterfaces();
}