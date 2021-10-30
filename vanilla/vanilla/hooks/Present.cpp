#include "..\hooks.h"
#include "..\globals.h"

/*
static const D3DMATRIX Identity =
{
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0
};
*/

HRESULT __stdcall hooks::Hooked_Present(IDirect3DDevice9* device, const RECT* pSourceRect, const RECT* pDestRect, HWND hDestWindowOverride, const RGNDATA* pDirtyRegion)
{
	IDirect3DVertexDeclaration9* oldVertDec     = nullptr;
	//IDirect3DVertexShader9*      oldVertShader  = nullptr;
	//IDirect3DPixelShader9*       oldPixelShader = nullptr;

	DWORD dwOld_COLORWRITEENABLE = NULL;

	if (!g_renderer_init)
	{
		// initialize our DPI scaling variables
		HDC hdc = _GetDC(g_window);
		{
			g_dpi.x = static_cast<float>(_GetDeviceCaps(hdc, LOGPIXELSX));
			g_dpi.y = static_cast<float>(_GetDeviceCaps(hdc, LOGPIXELSY));
		}
		_ReleaseDC(g_window, hdc);

		// initialize our d3d stuff
		g_renderer->GetViewport(&g_viewport);
		g_renderer->CreateStateBlock(D3DSBT_PIXELSTATE, &g_stateblock);

		// setup fonts
		g_fonts.SetupFonts();

		// scale menu
		g_menu.Scale();

		g_renderer_init = true;
	}

	g_stateblock->Capture();

	device->GetVertexDeclaration(&oldVertDec);
	//Device->GetVertexShader(&oldVertShader);
	//Device->GetPixelShader(&oldPixelShader);

	device->GetRenderState(D3DRS_COLORWRITEENABLE, &dwOld_COLORWRITEENABLE);

	// setup
	RECT noScissor =
	{
		0,
		0,
		static_cast<LONG>(g_viewport.Width),
		static_cast<LONG>(g_viewport.Height)
	};

	device->SetScissorRect(&noScissor);

	device->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1);

	device->SetVertexShader(nullptr);
	device->SetPixelShader(nullptr);

	device->SetRenderState(D3DRS_LIGHTING, FALSE);
	device->SetRenderState(D3DRS_FOGENABLE, FALSE);
	device->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
	device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	device->SetRenderState(D3DRS_SCISSORTESTENABLE, TRUE);
	device->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
	device->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, FALSE);

	device->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	device->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	device->SetSamplerState(0, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
	device->SetSamplerState(0, D3DSAMP_SRGBTEXTURE, 0);

	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);

	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
	device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);

	device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	device->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);

	device->SetRenderState(D3DRS_COLORWRITEENABLE, 0xFFFFFFFF);
	device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_SRGBWRITEENABLE, FALSE);

	device->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, TRUE);
	device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	device->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_INVDESTALPHA);
	device->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ONE);

	//Device->SetTransform(D3DTS_VIEW, &Identity);

	//Device->SetTexture(0, NULL);

	// Draw our menu
	g_menu.Paint();

	// restore
	g_stateblock->Apply();

	device->SetRenderState(D3DRS_COLORWRITEENABLE, dwOld_COLORWRITEENABLE);
	device->SetRenderState(D3DRS_SRGBWRITEENABLE, TRUE);

	device->SetVertexDeclaration(oldVertDec);
	//Device->SetVertexShader(oldVertShader);
	//Device->SetPixelShader(oldPixelShader);

	return hooks::oPresent(device, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}
