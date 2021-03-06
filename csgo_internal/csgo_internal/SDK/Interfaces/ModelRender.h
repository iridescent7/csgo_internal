#pragma once
#include "getvfunc.h"
#include "..\Vector.h"
#include "..\VMatrix.h"

typedef unsigned short ModelInstanceHandle_t;

class IMaterial;
class IMatRenderContext;
class IClientRenderable;
struct model_t;
struct studiohdr_t;
struct studiohwdata_t;
struct StudioDecalHandle_t { int unused; };

struct DrawModelState_t
{
	studiohdr_t*			m_pStudioHdr;
	studiohwdata_t*			m_pStudioHWData;
	IClientRenderable*		m_pRenderable;
	const matrix3x4_t		*m_pModelToWorld;
	StudioDecalHandle_t		m_decals;
	int						m_drawFlags;
	int						m_lod;
};

enum OverrideType_t
{
	OVERRIDE_NORMAL = 0,
	OVERRIDE_BUILD_SHADOWS,
	OVERRIDE_DEPTH_WRITE,
};

//--- 3/30/18 ---//
struct ModelRenderInfo_t
{
	Vector origin;
	Vector angles;
	char pad[0x4];
	void *pRenderable;
	const model_t *pModel;
	const matrix3x4_t *pModelToWorld;
	const matrix3x4_t *pLightingOffset;
	const Vector *pLightingOrigin;
	int flags;
	int entity_index;
	int skin;
	int body;
	int hitboxset;
	unsigned short instance;

	ModelRenderInfo_t()
	{
		pModelToWorld = NULL;
		pLightingOffset = NULL;
		pLightingOrigin = NULL;
	}
};

class CModelRender
{
public:
	void DrawModelExecute(IMatRenderContext* ctx, const DrawModelState_t &state, const ModelRenderInfo_t &pInfo, matrix3x4_t *pCustomBoneToWorld)
	{
		typedef void(__thiscall* DrawModelExecuteFn)(void*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t*);
		getvfunc<DrawModelExecuteFn>(this, 21)(this, ctx, state, pInfo, pCustomBoneToWorld);
	}
	void ForcedMaterialOverride(IMaterial *newMaterial, OverrideType_t nOverrideType = OVERRIDE_NORMAL)
	{
		typedef void(__thiscall* OriginalFn)(void*, IMaterial*, OverrideType_t, int);
		getvfunc<OriginalFn>(this, 1)(this, newMaterial, nOverrideType, NULL);
	}
};
