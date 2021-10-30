#include "Hooks.h"
#include "..\Globals.h"

#include <intrin.h>

#include "..\GUI\Colors.h"

namespace H
{
	constexpr D3DRENDERSTATETYPE StatesToBackup[] =
	{
		D3DRS_COLORWRITEENABLE,
		D3DRS_SRGBWRITEENABLE,
		D3DRS_ALPHABLENDENABLE,
		D3DRS_SRCBLEND,
		D3DRS_DESTBLEND,
		D3DRS_BLENDOP,
		D3DRS_FOGENABLE 
	};

	long __stdcall Hooked_EndScene(IDirect3DDevice9* pDevice)
	{
		// prevents from being called twice per frame
		static auto first_addr = _ReturnAddress();

		if (_ReturnAddress() != first_addr)
			return oEndScene(pDevice);

		if (!D3DInit)
		{
			R::Initialize(pDevice);
			D3DInit = true;
		}

		DWORD oldFVF, oldStates[ARRAYSIZE(StatesToBackup)];

		IDirect3DVertexDeclaration9* oldVertDec;
		IDirect3DVertexShader9* oldVertShader;

		// backup FVF and render states
		pDevice->GetFVF(&oldFVF);
		pDevice->GetVertexDeclaration(&oldVertDec);
		pDevice->GetVertexShader(&oldVertShader);

		for (size_t i = 0; i < ARRAYSIZE(StatesToBackup); ++i)
			pDevice->GetRenderState(StatesToBackup[i], &oldStates[i]);

		// set our FVF and render states
		pDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);
		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0xFFFFFFFF);
		pDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);

		pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		
		C::PlayerEsp.EndScene();
		//C::LegitBacktrack.EndScene();
		//C::LegitTriggerbot.EndScene(); //debug

		if (I::Engine->IsInGame())
		{
			//R::Text(F::EspText, 5, 5, D3DCOLOR_ARGB(255, 255, 255, 0), ("Choked Packets: " + std::to_string(I::Engine->GetNetChannel()->m_nChokedPackets)).c_str());
			C::LegitMisc.EndScene();
			//C::AntiAim.EndScene();
		}

		

		G::Menu.EndScene();

		// restore FVF and render states
		pDevice->SetFVF(oldFVF);
		pDevice->SetVertexDeclaration(oldVertDec);
		pDevice->SetVertexShader(oldVertShader);

		for (size_t i = 0; i < ARRAYSIZE(StatesToBackup); ++i)
			pDevice->SetRenderState(StatesToBackup[i], oldStates[i]);

		return oEndScene(pDevice);
	}
}