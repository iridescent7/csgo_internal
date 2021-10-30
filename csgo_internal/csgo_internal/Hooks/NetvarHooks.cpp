#include "Hooks.h"
#include "..\SDK\SDK.h"
#include "..\Globals.h"

#include <string>

#define RandomSequence(nMin, nMax) (rand() % (nMax - nMin + 1) + nMin);

#define SEQUENCE_DEFAULT_DRAW 0
#define SEQUENCE_DEFAULT_IDLE1 1
#define SEQUENCE_DEFAULT_IDLE2 2
#define SEQUENCE_DEFAULT_LIGHT_MISS1 3
#define SEQUENCE_DEFAULT_LIGHT_MISS2 4
#define SEQUENCE_DEFAULT_HEAVY_MISS1 9
#define SEQUENCE_DEFAULT_HEAVY_HIT1 10
#define SEQUENCE_DEFAULT_HEAVY_BACKSTAB 11
#define SEQUENCE_DEFAULT_LOOKAT01 12

#define SEQUENCE_BUTTERFLY_DRAW 0
#define SEQUENCE_BUTTERFLY_DRAW2 1
#define SEQUENCE_BUTTERFLY_LOOKAT01 13
#define SEQUENCE_BUTTERFLY_LOOKAT03 15

#define SEQUENCE_FALCHION_IDLE1 1
#define SEQUENCE_FALCHION_HEAVY_MISS1 8
#define SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP 9
#define SEQUENCE_FALCHION_LOOKAT01 12
#define SEQUENCE_FALCHION_LOOKAT02 13

#define SEQUENCE_DAGGERS_IDLE1 1
#define SEQUENCE_DAGGERS_LIGHT_MISS1 2
#define SEQUENCE_DAGGERS_LIGHT_MISS5 6
#define SEQUENCE_DAGGERS_HEAVY_MISS2 11
#define SEQUENCE_DAGGERS_HEAVY_MISS1 12

#define SEQUENCE_BOWIE_IDLE1 1

namespace H
{
	void Hooked_RecvModelIndex(CRecvProxyData* pData, void *pStruct, void *pOut)
	{
		static int default_t = I::ModelInfo->GetModelIndex("models/weapons/v_knife_default_t.mdl");
		static int default_ct = I::ModelInfo->GetModelIndex("models/weapons/v_knife_default_ct.mdl");
		static int iBayonet = I::ModelInfo->GetModelIndex("models/weapons/v_knife_bayonet.mdl");
		static int iButterfly = I::ModelInfo->GetModelIndex("models/weapons/v_knife_butterfly.mdl");
		static int iFlip = I::ModelInfo->GetModelIndex("models/weapons/v_knife_flip.mdl");
		static int iGut = I::ModelInfo->GetModelIndex("models/weapons/v_knife_gut.mdl");
		static int iKarambit = I::ModelInfo->GetModelIndex("models/weapons/v_knife_karam.mdl");
		static int iM9Bayonet = I::ModelInfo->GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
		static int iHuntsman = I::ModelInfo->GetModelIndex("models/weapons/v_knife_tactical.mdl");
		static int iFalchion = I::ModelInfo->GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
		static int iDagger = I::ModelInfo->GetModelIndex("models/weapons/v_knife_push.mdl");
		static int iBowie = I::ModelInfo->GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");
		static int iGunGame = I::ModelInfo->GetModelIndex("models/weapons/v_knife_gg.mdl");

		static int Navaja = I::ModelInfo->GetModelIndex("models/weapons/v_knife_gypsy_jackknife.mdl");
		static int Stiletto = I::ModelInfo->GetModelIndex("models/weapons/v_knife_stiletto.mdl");
		static int Ursus = I::ModelInfo->GetModelIndex("models/weapons/v_knife_ursus.mdl");
		static int Talon = I::ModelInfo->GetModelIndex("models/weapons/v_knife_widowmaker.mdl");

		// Get local player (just to stop replacing spectators knifes)
		C_CSPlayer* pLocal = I::EntityList->GetClientEntity(I::Engine->GetLocalPlayer());

		// Carry on to the original proxy
		oRecvModelIndex(pData, pStruct, pOut);
	}

	void Hooked_RecvSequence(CRecvProxyData* pData, CBaseViewModel* pViewModel, void* pOut)
	{
		if (pViewModel)
		{
			DWORD ownerId = pViewModel->GetOwner() & 0xFFF;
			C_CSPlayer* pOwner = I::EntityList->GetClientEntity(ownerId);

			if (pOwner && ownerId == I::Engine->GetLocalPlayer())
			{
				// Get the filename of the current view model.
				std::string szModel = I::ModelInfo->GetModelName(I::ModelInfo->GetModel(pViewModel->GetModelIndex()));

				// Store the current sequence.
				int m_nSequence = pData->m_Value.m_Int;
				if (szModel == "models/weapons/v_knife_butterfly.mdl")
				{
					// Fix animations for the Butterfly Knife.
					switch (m_nSequence)
					{
					case SEQUENCE_DEFAULT_DRAW:
						m_nSequence = RandomSequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
						break;
					case SEQUENCE_DEFAULT_LOOKAT01:
						m_nSequence = RandomSequence(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
						break;
					default:
						++m_nSequence;
					}
				}
				else if (szModel == "models/weapons/v_knife_falchion_advanced.mdl")
				{
					// Fix animations for the Falchion Knife.
					switch (m_nSequence)
					{
					case SEQUENCE_DEFAULT_IDLE2:
						m_nSequence = SEQUENCE_FALCHION_IDLE1;
						break;
					case SEQUENCE_DEFAULT_HEAVY_MISS1:
						m_nSequence = RandomSequence(SEQUENCE_FALCHION_HEAVY_MISS1, SEQUENCE_FALCHION_HEAVY_MISS1_NOFLIP);
						break;
					case SEQUENCE_DEFAULT_LOOKAT01:
						m_nSequence = RandomSequence(SEQUENCE_FALCHION_LOOKAT01, SEQUENCE_FALCHION_LOOKAT02);
						break;
					case SEQUENCE_DEFAULT_DRAW:
					case SEQUENCE_DEFAULT_IDLE1:
						break;
					default:
						--m_nSequence;
					}
				}
				else if (szModel == "models/weapons/v_knife_push.mdl")
				{
					// Fix animations for the Shadow Daggers.
					switch (m_nSequence)
					{
					case SEQUENCE_DEFAULT_IDLE2:
						m_nSequence = SEQUENCE_DAGGERS_IDLE1; break;
					case SEQUENCE_DEFAULT_LIGHT_MISS1:
					case SEQUENCE_DEFAULT_LIGHT_MISS2:
						m_nSequence = RandomSequence(SEQUENCE_DAGGERS_LIGHT_MISS1, SEQUENCE_DAGGERS_LIGHT_MISS5);
						break;
					case SEQUENCE_DEFAULT_HEAVY_MISS1:
						m_nSequence = RandomSequence(SEQUENCE_DAGGERS_HEAVY_MISS2, SEQUENCE_DAGGERS_HEAVY_MISS1);
						break;
					case SEQUENCE_DEFAULT_HEAVY_HIT1:
					case SEQUENCE_DEFAULT_HEAVY_BACKSTAB:
					case SEQUENCE_DEFAULT_LOOKAT01:
						m_nSequence += 3; break;
					case SEQUENCE_DEFAULT_DRAW:
					case SEQUENCE_DEFAULT_IDLE1:
						break;
					default:
						m_nSequence += 2;
					}
				}
				else if (szModel == "models/weapons/v_knife_survival_bowie.mdl")
				{
					// Fix animations for the Bowie Knife.
					switch (m_nSequence)
					{
					case SEQUENCE_DEFAULT_DRAW:
					case SEQUENCE_DEFAULT_IDLE1:
						break;
					case SEQUENCE_DEFAULT_IDLE2:
						m_nSequence = SEQUENCE_BOWIE_IDLE1;
						break;
					default:
						--m_nSequence;
					}
				}
				else if (szModel == "models/weapons/v_knife_ursus.mdl")
				{
					// Fix animations for the Bowie Knife.
					switch (m_nSequence)
					{
					case SEQUENCE_DEFAULT_DRAW:
						m_nSequence = RandomSequence(SEQUENCE_BUTTERFLY_DRAW, SEQUENCE_BUTTERFLY_DRAW2);
						break;
					case SEQUENCE_DEFAULT_LOOKAT01:
						m_nSequence = RandomSequence(SEQUENCE_BUTTERFLY_LOOKAT01, SEQUENCE_BUTTERFLY_LOOKAT03);
						break;
					default:
						++m_nSequence;
						break;
					}
				}
				else if (szModel == "models/weapons/v_knife_stiletto.mdl")
				{
					// Fix animations for the Bowie Knife.
					switch (m_nSequence)
					{
					case SEQUENCE_DEFAULT_LOOKAT01:
						m_nSequence = RandomSequence(12, 13);
						break;
					}
				}
				else if (szModel == "models/weapons/v_knife_widowmaker.mdl")
				{
					// Fix animations for the Bowie Knife.
					switch (m_nSequence)
					{
					case SEQUENCE_DEFAULT_LOOKAT01:
						m_nSequence = RandomSequence(14, 15);
						break;
					}
				}
				pData->m_Value.m_Int = m_nSequence;
			}
		}
		oRecvSequence(pData, pViewModel, pOut);
	}
}