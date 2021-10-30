#pragma once

#include <d3dx9.h>
#include <limits>
#include <string>
#include "cx_strenc.h"

#define GET_X(x) ( (float) ( x ) / 96.f )
#define GET_Y(y) ( (float) ( y ) / 96.f )

#define SC_X(_x) ( g_dpi.x * ( _x ) )
#define SC_Y(_y) ( g_dpi.y * ( _y ) )

#define DYN_X(x) SC_X( GET_X( x ) )
#define DYN_Y(y) SC_Y( GET_Y( y ) )

struct TLVertex
{
	float x, y, z, rhw;
	D3DCOLOR color;
};

class D3DX9Renderer : public IDirect3DDevice9
{
public:
	__forceinline IDirect3DDevice9* GetDevice()
	{
		return reinterpret_cast<IDirect3DDevice9*>(this);
	}

	void SetScissorTest(long x, long y);
	void SetScissorTest(long x, long y, long x1, long y1);
	void ResetScissorTest();

	void GetTextSizeA    (ID3DXFont* font, const char* text, float* width, float* height);
	void DrawTextA       (ID3DXFont* font, const char* text, float x, float y, D3DCOLOR color);
	void DrawOutlinedText(ID3DXFont* font, const char* text, float x, float y, D3DCOLOR color, D3DCOLOR outlineColor);

	void GetTextSizeA    (ID3DXFont* font, enc_string text, float* width, float* height);
	void DrawTextA       (ID3DXFont* font, enc_string text, float x, float y, D3DCOLOR color);
	void DrawOutlinedText(ID3DXFont* font, enc_string text, float x, float y, D3DCOLOR color, D3DCOLOR outlineColor);

	void DrawFilledRect(float x, float y, float width, float height, D3DCOLOR color);
	void DrawSoftFilledRect(float x, float y, float width, float height, D3DCOLOR color);

	void DrawFilledCircle(float x, float y, float radius, size_t numSides, D3DCOLOR color);
};
