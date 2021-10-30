#include "..\hooks.h"
#include "..\globals.h"

HRESULT __stdcall hooks::Hooked_Reset(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* pPresentParameters)
{
	if (!g_renderer_init)
		return hooks::oReset(device, pPresentParameters);

	g_fonts.OnLostDevice();
	g_stateblock->Release();

	HRESULT ret_value = hooks::oReset(device, pPresentParameters);

	HDC hdc = _GetDC(g_window);
	{
		g_dpi.x = static_cast<float>(_GetDeviceCaps(hdc, LOGPIXELSX));
		g_dpi.y = static_cast<float>(_GetDeviceCaps(hdc, LOGPIXELSY));
	}
	_ReleaseDC(g_window, hdc);

	device->GetViewport(&g_viewport);
	device->CreateStateBlock(D3DSBT_PIXELSTATE, &g_stateblock);

	g_fonts.OnResetDevice();
	g_menu.Scale();

	return ret_value;
}
