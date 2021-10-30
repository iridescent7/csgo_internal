#include "renderer.h"
#include "..\globals.h"

void D3DX9Renderer::SetScissorTest(long x, long y)
{
	RECT scissor =
	{
		x,
		y,
		static_cast<LONG>(g_viewport.Width),
		static_cast<LONG>(g_viewport.Height)
	};

	this->SetScissorRect(&scissor);
}

void D3DX9Renderer::SetScissorTest(long x, long y, long x1, long y1)
{
	RECT scissor = { x, y, x1, y1 };
	this->SetScissorRect(&scissor);
}

void D3DX9Renderer::ResetScissorTest()
{
	RECT noScissor =
	{
		0,
		0,
		static_cast<LONG>(g_viewport.Width),
		static_cast<LONG>(g_viewport.Height)
	};

	this->SetScissorRect(&noScissor);
}

void D3DX9Renderer::GetTextSizeA(ID3DXFont* font, const char* text, float* width, float* height)
{
	RECT rect = { 0 };
	font->DrawTextA(NULL, text, -1, &rect, DT_CALCRECT, 0);

	if (width)
		*width = (float)(rect.right - rect.left);

	if (height)
		*height = (float)(rect.bottom - rect.top);
}

void D3DX9Renderer::DrawTextA(ID3DXFont* font, const char* text, float x, float y, D3DCOLOR color)
{
	RECT rect = { (long)x, (long)y, (long)x, (long)y };
	font->DrawTextA(NULL, text, -1, &rect, DT_NOCLIP, color);
}

void D3DX9Renderer::DrawOutlinedText(ID3DXFont* font, const char* text, float x, float y, D3DCOLOR color, D3DCOLOR outlineColor)
{
	this->DrawTextA(font, text, x + 1, y + 1, outlineColor);
	this->DrawTextA(font, text, x - 1, y + 1, outlineColor);
	this->DrawTextA(font, text, x + 1, y - 1, outlineColor);
	this->DrawTextA(font, text, x - 1, y - 1, outlineColor);

	this->DrawTextA(font, text, x, y, color);
}

void D3DX9Renderer::GetTextSizeA(ID3DXFont* font, enc_string text, float* width, float* height)
{
	this->GetTextSizeA(font, encstr_tostr(text).c_str(), width, height);
}

void D3DX9Renderer::DrawTextA(ID3DXFont* font, enc_string text, float x, float y, D3DCOLOR color)
{
	this->DrawTextA(font, encstr_tostr(text).c_str(), x, y, color);
}

void D3DX9Renderer::DrawOutlinedText(ID3DXFont* font, enc_string text, float x, float y, D3DCOLOR color, D3DCOLOR outlineColor)
{
	this->DrawOutlinedText(font, encstr_tostr(text).c_str(), x, y, color, outlineColor);
}

void D3DX9Renderer::DrawFilledRect(float x, float y, float width, float height, D3DCOLOR color)
{
	TLVertex v_rect[] =
	{
		{ x,         y + height, 0.f, 1.f, color },
		{ x,         y,          0.f, 1.f, color },
		{ x + width, y + height, 0.f, 1.f, color },
		{ x + width, y,          0.f, 1.f, color }
	};

	this->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v_rect, sizeof(TLVertex));
}

void D3DX9Renderer::DrawSoftFilledRect(float x, float y, float width, float height, D3DCOLOR color)
{
	float x_corner_radius = DYN_X(2);
	float y_corner_radius = DYN_Y(2);

	TLVertex v_buffer[] =
	{
		// starting point
		{ x + x_corner_radius, y + y_corner_radius, 0.f, 1.f, color },

		{ x + x_corner_radius,         y,                            0.f, 1.f, color },
		{ x + width - x_corner_radius, y,                            0.f, 1.f, color },
		{ x + width,                   y + y_corner_radius,          0.f, 1.f, color },
		{ x + width,                   y + height - y_corner_radius, 0.f, 1.f, color },
		{ x + width - x_corner_radius, y + height,                   0.f, 1.f, color },
		{ x + x_corner_radius,         y + height,                   0.f, 1.f, color },
		{ x,                           y + height - y_corner_radius, 0.f, 1.f, color },
		{ x,                           y + y_corner_radius,          0.f, 1.f, color },
		{ x + x_corner_radius,         y,                            0.f, 1.f, color }
	};

	this->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, 8, v_buffer, sizeof(TLVertex));
}

void D3DX9Renderer::DrawFilledCircle(float x, float y, float radius, size_t numSides, D3DCOLOR color)
{
	if (radius < 1.f || numSides < 3)
		return;

	float step = D3DX_PI * 2.0f / (float)numSides;

	TLVertex* pVertices = new TLVertex[numSides + 2];
	size_t lastVert     = numSides + 1;

	// Load starting & ending points

	pVertices[0] = { x, y, 0.f, 1.f, color };
	pVertices[1] = pVertices[lastVert] =
	{
		radius * cosf(step) + x,
		radius * sinf(step) + y,
		0.f, 1.f,
		color
	};

	float current = step * 2;

	for (size_t i = 2; i < lastVert; ++i)
	{
		float x0 = radius * cosf(current) + x;
		float y0 = radius * sinf(current) + y;

		pVertices[i] = { x0, y0, 0.f, 1.f, color };

		current += step;
	}

	this->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, numSides, pVertices, sizeof(TLVertex));
	delete[] pVertices;
}