#pragma once

#include "getvfunc.h"

#include "..\studio.h"
#include "..\QAngle.h"
#include "..\VMatrix.h"

#include "..\..\Offsets.h"

#include "..\ModelInfo.h"
#include "..\RecvData.h"
#include "..\classid.h"

class CAnimationLayer
{
public:
	char  pad_0000[20];
	unsigned int m_nOrder; //0x0014
	unsigned int m_nSequence; //0x0018
	float m_flPrevCycle; //0x001C
	float m_flWeight; //0x0020
	float m_flWeightDeltaRate; //0x0024
	float m_flPlaybackRate; //0x0028
	float m_flCycle; //0x002C
	void *m_pOwner; //0x0030 // player's thisptr
	char  pad_0038[4]; //0x0034
}; //Size: 0x0038

class CCSPlayerAnimState;
class CBaseHandle;

enum DataUpdateType_t
{
	DATA_UPDATE_CREATED = 0,
	DATA_UPDATE_DATATABLE_CHANGED = 1,
};

class bf_read;
class IClientUnknown;

class IClientNetworkable
{
public:
	virtual IClientUnknown*            GetIClientUnknown() = 0;
	virtual void                       Release() = 0;
	virtual ClientClass*               GetClientClass() = 0;
	virtual void                       NotifyShouldTransmit(int state) = 0;
	virtual void                       OnPreDataChanged(int updateType) = 0;
	virtual void                       OnDataChanged(int updateType) = 0;
	virtual void                       PreDataUpdate(int updateType) = 0;
	virtual void                       PostDataUpdate(int updateType) = 0;
	virtual void                       Unknown() = 0;
	virtual bool                       IsDormant() = 0;
	virtual int                        EntIndex() const = 0;
	virtual void                       ReceiveMessage(int classID, bf_read& msg) = 0;
	virtual void*                      GetDataTableBasePtr() = 0;
	virtual void                       SetDestroyedOnRecreateEntities() = 0;
};

class IHandleEntity;
enum SolidType_t;
class matrix3x4;
struct Ray_t;
struct trace_t;
class ICollideable
{
public:
	virtual IHandleEntity*      GetEntityHandle() = 0;
	virtual Vector&				OBBMins() const = 0;
	virtual Vector&				OBBMaxs() const = 0;
	virtual void                WorldSpaceTriggerBounds(Vector *pVecWorldMins, Vector *pVecWorldMaxs) const = 0;
	virtual bool                TestCollision(const Ray_t &ray, unsigned int fContentsMask, trace_t& tr) = 0;
	virtual bool                TestHitboxes(const Ray_t &ray, unsigned int fContentsMask, trace_t& tr) = 0;
	virtual int                 GetCollisionModelIndex() = 0;
	virtual const model_t*      GetCollisionModel() = 0;
	virtual Vector&				GetCollisionOrigin() const = 0;
	virtual Vector&		        GetCollisionAngles() const = 0;
	virtual const matrix3x4&  CollisionToWorldTransform() const = 0;
	virtual SolidType_t         GetSolid() const = 0;
	virtual int                 GetSolidFlags() const = 0;
	virtual IClientUnknown*     GetIClientUnknown() = 0;
	virtual int                 GetCollisionGroup() const = 0;
	virtual void                WorldSpaceSurroundingBounds(Vector *pVecMins, Vector *pVecMaxs) = 0;
	virtual bool                ShouldTouchTrigger(int triggerSolidFlags) const = 0;
	virtual const matrix3x4*  GetRootParentToWorldTransform() const = 0;
};

struct SpatializationInfo_t;

class IClientNetworkable;
class IClientRenderable;
class IClientModelRenderable;
class IClientEntity;
class C_CSPlayer;
class IClientThinkable;
class IClientAlphaProperty;

class CBaseHandle;

class IHandleEntity
{
public:
	virtual ~IHandleEntity() {}
	virtual void SetRefEHandle(const CBaseHandle &handle) = 0;
	virtual const CBaseHandle& GetRefEHandle() const = 0;
};

class IClientUnknown : public IHandleEntity
{
public:
	virtual ICollideable*              GetCollideable() = 0;
	virtual IClientNetworkable*        GetClientNetworkable() = 0;
	virtual IClientRenderable*         GetClientRenderable() = 0;
	virtual IClientEntity*             GetIClientEntity() = 0;
	virtual C_CSPlayer*              GetBaseEntity() = 0;
	virtual IClientThinkable*          GetClientThinkable() = 0;
	virtual IClientModelRenderable*	 GetClientModelRenderable() = 0;
	virtual IClientAlphaProperty*      GetClientAlphaProperty() = 0;
};

typedef unsigned short ClientShadowHandle_t;
typedef unsigned short ClientRenderHandle_t;
typedef unsigned short ModelInstanceHandle_t;
typedef unsigned char uint8;

class IClientUnknown;
struct model_t;

class IClientRenderable
{
public:
	virtual IClientUnknown*            GetIClientUnknown() = 0;
	virtual Vector const&              GetRenderOrigin(void) = 0;
	virtual QAngle const&              GetRenderAngles(void) = 0;
	virtual bool                       ShouldDraw(void) = 0;
	virtual int                        GetRenderFlags(void) = 0; // ERENDERFLAGS_xxx
	virtual void                       Unused(void) const {}
	virtual ClientShadowHandle_t       GetShadowHandle() const = 0;
	virtual ClientRenderHandle_t&      RenderHandle() = 0;
	virtual const model_t*             GetModel() const = 0;
	virtual int                        DrawModel(int flags, unsigned char alpha/*RenderableInstance_t &instance*/) = 0; //const int instead of uint8_t
	virtual int                        GetBody() = 0;
	virtual void                       GetColorModulation(float* color) = 0;
	virtual bool                       LODTest() = 0;
	virtual bool                       oSetupBones(matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime) = 0;
	virtual void                       SetupWeights(const matrix3x4_t *pBoneToWorld, int nFlexWeightCount, float *pFlexWeights, float *pFlexDelayedWeights) = 0;
	virtual void                       DoAnimationEvents(void) = 0;
	virtual void* /*IPVSNotify*/       GetPVSNotifyInterface() = 0;
	virtual void                       GetRenderBounds(Vector& mins, Vector& maxs) = 0;
	virtual void                       GetRenderBoundsWorldspace(Vector& mins, Vector& maxs) = 0;
	virtual void                       GetShadowRenderBounds(Vector &mins, Vector &maxs, int /*ShadowType_t*/ shadowType) = 0;
	virtual bool                       ShouldReceiveProjectedTextures(int flags) = 0;
	virtual bool                       GetShadowCastDistance(float *pDist, int /*ShadowType_t*/ shadowType) const = 0;
	virtual bool                       GetShadowCastDirection(Vector *pDirection, int /*ShadowType_t*/ shadowType) const = 0;
	virtual bool                       IsShadowDirty() = 0;
	virtual void                       MarkShadowDirty(bool bDirty) = 0;
	virtual IClientRenderable*         GetShadowParent() = 0;
	virtual IClientRenderable*         FirstShadowChild() = 0;
	virtual IClientRenderable*         NextShadowPeer() = 0;
	virtual int /*ShadowType_t*/       ShadowCastType() = 0;
	virtual void                       CreateModelInstance() = 0;
	virtual ModelInstanceHandle_t      GetModelInstance() = 0;
	virtual const matrix3x4_t&         RenderableToWorldTransform() = 0;
	virtual int                        LookupAttachment(const char *pAttachmentName) = 0;
	virtual bool				       GetAttachment(int number, Vector &origin, QAngle &angles) = 0;
	virtual bool                       GetAttachment(int number, matrix3x4_t &matrix) = 0;
	virtual float*                     GetRenderClipPlane(void) = 0;
	virtual int                        GetSkin() = 0;
	virtual void                       OnThreadedDrawSetup() = 0;
	virtual bool                       UsesFlexDelayedWeights() = 0;
	virtual void                       RecordToolMessage() = 0;
	virtual bool                       ShouldDrawForSplitScreenUser(int nSlot) = 0;
	virtual uint8                      OverrideAlphaModulation(uint8 nAlpha) = 0;
	virtual uint8                      OverrideShadowAlphaModulation(uint8 nAlpha) = 0;
};

class IClientUnknown;
class CClientThinkHandlePtr;
typedef CClientThinkHandlePtr* ClientThinkHandle_t;

class IClientThinkable
{
public:
	virtual IClientUnknown*		GetIClientUnknown() = 0;
	virtual void				ClientThink() = 0;
	virtual ClientThinkHandle_t	GetThinkHandle() = 0;
	virtual void				SetThinkHandle(ClientThinkHandle_t hThink) = 0;
	virtual void				Release() = 0;
};

class IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
	virtual void             Release(void) = 0;
	virtual const Vector&    oGetAbsOrigin(void) const = 0;
	virtual const QAngle&    GetAbsAngles(void) const = 0;
	virtual void*            GetMouth(void) = 0;
	virtual bool             GetSoundSpatialization(SpatializationInfo_t info) = 0;
	virtual bool             IsBlurred(void) = 0;

public:
	bool IsPlayer()
	{
		return ( this->GetClientClass()->m_ClassID == ClassId_CCSPlayer );
	}
};

class CBaseAnimating
{
public:
	float* m_flPoseParameter()
	{
		return (float*)((DWORD)this + O::m_flPoseParameter);
	}
	model_t* GetModel()
	{
		void* pRenderable = (void*)((DWORD)this + 0x4);
		typedef model_t* (__thiscall* fnGetModel)(void*);

		return getvfunc<fnGetModel>(pRenderable, 8)(pRenderable);
	}
};

// todo: refactorize, CBaseEntity, CbaseCombatPlayer, CCSPlayer
class C_CSPlayer : public IClientEntity // todo: clean up this mess later
{
public:
	NETVAR_GETSET(int, m_iHealth)
	NETVAR_GETSET(int, m_fFlags)
	NETVAR_GETSET(int, m_iTeamNum)
	NETVAR_GETSET(int, m_iObserverMode)
	NETVAR_GETSET(bool, m_bIsScoped)
	NETVAR_GETSET(int, m_iAccount)
	NETVAR_GETSET(int, m_lifeState)
	NETVAR_GETSET(float, m_flLowerBodyYawTarget)
	NETVAR_GETSET(int, m_nMoveType)
	NETVAR_GETSET(QAngle, m_angEyeAngles)
	NETVAR_GETSET(Vector, m_vecVelocity)
	NETVAR_GETSET(Vector, m_vecOrigin)
	NETVAR_GETSET(int, m_nTickBase)
	NETVAR_GETSET(float, m_flSimulationTime)
	NETVAR_GETSET(Vector, m_vecViewOffset)
	NETVAR_GETSET(QAngle, m_aimPunchAngle)
	NETVAR_GETSET(QAngle, m_viewPunchAngle)
	NETVAR_GETSET(bool, m_bGunGameImmunity)
	NETVAR_GETSET(float, m_flFlashDuration)
	NETVAR_GETSET(float, m_flNextAttack)
	NETVAR_GETSET(float, m_flDuckAmount)
	NETVAR_GETSET(int, m_ArmorValue)
	NETVAR_GETSET(int, m_iShotsFired)
	NETVAR_GETSET(bool, m_bHasHelmet)

	PNETVAR_GET(CBaseHandle, m_hObserverTarget)
	PNETVAR_GET(CBaseHandle, m_hActiveWeapon)
	PNETVAR_GET(ICollideable, m_Collision)

	CCSPlayerAnimState* GetAnimState()
	{
		return *(CCSPlayerAnimState**)((DWORD)this + O::animstate);
	}
	CAnimationLayer* GetAnimLayer()
	{
		return *(CAnimationLayer**)((DWORD)this + O::animlayer);
	}
	bool IsAlive()
	{
		return this->m_lifeState() == 0;
	}
	int GetIndex()
	{
		return *(int*)((DWORD)this + 0x64);
	}
/*
	bool HasC4Bomb()
	{
		int iBombIndex = *(int*)(*(DWORD*)(O::dwPlayerResource) + O::m_iPlayerC4);
		if (iBombIndex == this->GetIndex())
			return true;
		else
			return false;
	}
*/
	Vector& GetAbsOrigin()
	{
		typedef Vector& (__thiscall* OriginalFn)(void*);
		return getvfunc<OriginalFn>(this, 10)(this);
	}
	Vector GetEyePosition()
	{
		return m_vecOrigin() + m_vecViewOffset();
	}
	void GetRenderBounds(Vector& mins, Vector& maxs)
	{
		void* pRenderable = (void*)(this + 0x4);
		typedef void(__thiscall* Fn)(void*, Vector&, Vector&);
		getvfunc<Fn>(pRenderable, 17)(pRenderable, mins, maxs);
	}
	void SetModelIndex(int index)
	{
		typedef void(__thiscall* OriginalFn)(PVOID, int);
		return getvfunc<OriginalFn>(this, 75)(this, index);
	}
	IClientNetworkable* GetClientNetworkable()
	{
		return (IClientNetworkable*)((DWORD)(this) + 0x8);
	}
	void PreDataUpdate(DataUpdateType_t updateType)
	{
		PVOID pNetworkable = (PVOID)((DWORD)(this) + 0x8);
		typedef void(__thiscall* OriginalFn)(PVOID, int);
		return getvfunc<OriginalFn>(pNetworkable, 6)(pNetworkable, updateType);
	}
	CBaseHandle* GetWeapons()
	{
		return (CBaseHandle*)((DWORD)this + O::m_hMyWeapons);
	}
	bool SetupBones(matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime)
	{
		__asm
		{
			mov edi, this
			lea ecx, dword ptr ds : [edi + 0x4]
			mov edx, dword ptr ds : [ecx]
			push currentTime
			push boneMask
			push nMaxBones
			push pBoneToWorldOut
			call dword ptr ds : [edx + 0x34]
		}
	}
	void UpdateClientSideAnimation()
	{
		typedef void(__thiscall* Fn)(void*);
		getvfunc<Fn>(this, 221)(this);
	}
	Vector& GetAbsAngles()
	{
		typedef Vector& (__thiscall* OriginalFn)(void*);
		return ((OriginalFn)getvfunc<OriginalFn>(this, 11))(this);
	}
	float* GetPoseParamaters()
	{
		return (float*)((DWORD)this + O::m_flPoseParameter);
	}
};

class CBaseViewModel : public CModelInfo
{
public:
	inline DWORD GetOwner()
	{
		return *(PDWORD)((DWORD)this + O::m_hOwner);
	}

	inline int GetModelIndex()
	{
		return *(int*)((DWORD)this + O::m_nModelIndex);
	}
};
