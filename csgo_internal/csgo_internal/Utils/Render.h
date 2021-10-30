#pragma once

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")

#include <d3d9.h>
#include <d3dx9.h>

#include "..\SDK\SDK.h"

// meme
#define TL2DVERT(x, y, color) { x, y, 0.0f, 1.0f, color }

struct D3DTLVERTEX
{
	float x, y, z, rhw;
	DWORD color;
};

namespace F
{
	extern ID3DXFont* Title;
	extern ID3DXFont* TabText;

	extern ID3DXFont* Text;
	extern ID3DXFont* GroupboxText;
	extern ID3DXFont* SliderText;

	extern ID3DXFont* EspText;
	extern ID3DXFont* EspHealth;
}

namespace R
{
	void Initialize(IDirect3DDevice9* pDevice);
	void OnLostDevice();
	void OnResetDevice();

	bool WorldToScreen(const Vector& origin, Vector2D* screen);

	void GetTextSize(LPD3DXFONT pFont, const char* text, float* width, float* height);
	void GetTextSizeW(LPD3DXFONT pFont, const wchar_t* text, float* width, float* height);

	void Text(LPD3DXFONT pFont, long x, long y, D3DCOLOR color, const char* text);
	void TextW(LPD3DXFONT pFont, long x, long y, D3DCOLOR color, const wchar_t* text);

	void TextF(LPD3DXFONT pFont, long x, long y, D3DCOLOR color, const char* format, ...);
	void TextWF(LPD3DXFONT pFont, long x, long y, D3DCOLOR color, const wchar_t* format, ...);

	void Line(float x0, float y0, float x1, float y1, D3DCOLOR color);

	void Rect(float x, float y, float width, float height, D3DCOLOR color);
	void FilledRect(float x, float y, float width, float height, D3DCOLOR color);

	void Circle(float x, float y, float radius, size_t numSides, D3DCOLOR color);
	void FilledCircle(float x, float y, float radius, size_t numSides, D3DCOLOR color);

	void GradientDown(float x, float y, float width, float height, D3DCOLOR topColor, D3DCOLOR bottomColor);
	void GradientSideways(float x, float y, float width, float height, D3DCOLOR leftColor, D3DCOLOR rightColor);

	extern IDirect3DDevice9* Device;
	extern D3DVIEWPORT9 ViewPort;
	extern matrix3x4_t WorldViewMatrix;

}
