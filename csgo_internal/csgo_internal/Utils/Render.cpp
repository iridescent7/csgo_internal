#include "Render.h"
#include <stdio.h>

namespace F
{
	ID3DXFont* Title = nullptr;
	ID3DXFont* TabText = nullptr;

	ID3DXFont* Text = nullptr;
	ID3DXFont* GroupboxText = nullptr;
	ID3DXFont* SliderText = nullptr;

	ID3DXFont* EspText = nullptr;
	ID3DXFont* EspHealth = nullptr;
}

namespace R
{
	IDirect3DDevice9* Device = nullptr;
	D3DVIEWPORT9 ViewPort;
	matrix3x4_t WorldViewMatrix = { 0 };

	void Initialize(IDirect3DDevice9* pDevice)
	{
		Device = pDevice;
		pDevice->GetViewport(&ViewPort);

		D3DXCreateFontA(pDevice, 28, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_SWISS, "Helvetica", &F::Title);
		D3DXCreateFontA(pDevice, 18, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_SWISS, "Helvetica", &F::TabText);

		D3DXCreateFontA(pDevice, 18, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Calibri", &F::Text);
		D3DXCreateFontA(pDevice, 22, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Calibri", &F::GroupboxText);
		D3DXCreateFontA(pDevice, 14, 0, FW_THIN, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Verdana", &F::SliderText);

		D3DXCreateFontA(pDevice, 14, 0, FW_NORMAL, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Calibri", &F::EspText);
		D3DXCreateFontA(pDevice, 12, 0, FW_THIN, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Verdana", &F::EspHealth);

		//D3DXCreateTextureFromFileInMemoryEx(Device, cursor_png, cursor_png_size,25,39, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &T::Cursor);
		//D3DXCreateSprite(Device, &Sprite);
	}

	void OnLostDevice()
	{
		F::Title->OnLostDevice();
		F::TabText->OnLostDevice();

		F::Text->OnLostDevice();
		F::GroupboxText->OnLostDevice();
		F::SliderText->OnLostDevice();

		F::EspText->OnLostDevice();
		F::EspHealth->OnLostDevice();
	}

	void OnResetDevice()
	{
		F::Title->OnResetDevice();
		F::TabText->OnResetDevice();

		F::Text->OnResetDevice();
		F::GroupboxText->OnResetDevice();
		F::SliderText->OnResetDevice();

		F::EspText->OnResetDevice();
		F::EspHealth->OnResetDevice();
	}
	
	bool WorldToScreen(const Vector& origin, Vector2D* screen)
	{
		float w = WorldViewMatrix[3][0] * origin.x + WorldViewMatrix[3][1] * origin.y + WorldViewMatrix[3][2] * origin.z + WorldViewMatrix[3][3];

		if (w > 0.01f)
		{
			float inverseW = 1.f / w;
			float screenW = (float)ViewPort.Width;
			float screenH = (float)ViewPort.Height;

			screen->x = (float)((screenW / 2.f) + (0.5f * ((WorldViewMatrix[0][0] * origin.x + WorldViewMatrix[0][1] * origin.y + WorldViewMatrix[0][2] * origin.z + WorldViewMatrix[0][3]) * inverseW) * screenW + 0.5f));
			screen->y = (float)((screenH / 2.f) - (0.5f * ((WorldViewMatrix[1][0] * origin.x + WorldViewMatrix[1][1] * origin.y + WorldViewMatrix[1][2] * origin.z + WorldViewMatrix[1][3]) * inverseW) * screenH + 0.5f));

			return true;
		}

		return false;
	}

	void GetTextSize(LPD3DXFONT pFont, const char* text, float* width, float* height)
	{
		RECT rect = { 0,0,0,0 };

		pFont->DrawTextA(NULL, text, -1, &rect, DT_CALCRECT, D3DCOLOR_XRGB(0, 0, 0));

		if (width)
			*width = (float)(rect.right - rect.left);

		if (height)
			*height = (float)(rect.bottom - rect.top);
	}

	void GetTextSizeW(LPD3DXFONT pFont, const wchar_t* text, float* width, float* height)
	{
		RECT rect = { 0,0,0,0 };

		pFont->DrawTextW(NULL, text, -1, &rect, DT_CALCRECT, D3DCOLOR_XRGB(0, 0, 0));

		if (width)
			*width = (float)(rect.right - rect.left);

		if (height)
			*height = (float)(rect.bottom - rect.top);
	}

	void Text(LPD3DXFONT pFont, long x, long y, D3DCOLOR color, const char* text)
	{
		RECT rect = { x, y, x, y };
		pFont->DrawTextA(NULL, text, -1, &rect, DT_NOCLIP, color);
	}

	void TextW(LPD3DXFONT pFont, long x, long y, D3DCOLOR color, const wchar_t* text)
	{
		RECT rect = { x, y, x, y };
		pFont->DrawTextW(NULL, text, -1, &rect, DT_NOCLIP, color);
	}

	void TextF(LPD3DXFONT pFont, long x, long y, D3DCOLOR color, const char* format, ...)
	{
		va_list args;
		va_start(args, format);

		int len = _vscprintf(format, args);
		if (len == -1)
			return;

		size_t size = (size_t)len + 1;
		char*  str  = new char[size];
		if (!str)
			return;

		int r = vsprintf_s(str, size, format, args);
		va_end(args);

		if (r == -1)
		{
			delete[] str;
			return;
		}

		RECT rect = { x, y, x, y };

		pFont->DrawTextA(NULL, str, -1, &rect, DT_NOCLIP, color);
		delete[] str;
	}

	void TextWF(LPD3DXFONT pFont, long x, long y, D3DCOLOR color, const wchar_t* format, ...)
	{
		va_list args;
		va_start(args, format);

		int len = _vscwprintf(format, args);
		if (len == -1)
			return;

		size_t   size = (size_t)len + 1;
		wchar_t* str  = new wchar_t[size];
		if (!str)
			return;

		int r = vswprintf_s(str, size, format, args);
		va_end(args);

		if (r == -1)
		{
			delete[] str;
			return;
		}

		RECT rect = { x, y, x, y };

		pFont->DrawTextW(NULL, str, -1, &rect, DT_NOCLIP, color);
		delete[] str;
	}

	void Line(float x, float y, float x1, float y1, D3DCOLOR color)
	{
		D3DTLVERTEX vertices[2] =
		{
			TL2DVERT(x, y,  color),
			TL2DVERT(x1, y1, color)
		};

		Device->DrawPrimitiveUP(D3DPT_LINESTRIP, 1, vertices, sizeof(D3DTLVERTEX));
	}

	void Rect(float x, float y, float width, float height, D3DCOLOR color)
	{
		float xw = x + width;
		float yh = y + height;

		D3DTLVERTEX vertices[] =
		{
			TL2DVERT(x,  y,  color),
			TL2DVERT(xw, y,  color),
			TL2DVERT(xw, yh, color),
			TL2DVERT(x,  yh, color),
			TL2DVERT(x,  y,  color)
		};

		Device->DrawPrimitiveUP(D3DPT_LINESTRIP, 4, vertices, sizeof(D3DTLVERTEX));
	}

	void FilledRect(float x, float y, float width, float height, D3DCOLOR color)
	{
		float xw = x + width;
		float yh = y + height;

		D3DTLVERTEX vertices[] =
		{
			TL2DVERT(x,  y,  color),
			TL2DVERT(xw, y,  color),
			TL2DVERT(x,  yh, color),
			TL2DVERT(xw, yh, color)
		};

		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(D3DTLVERTEX));
	}

	void Circle(float x, float y, float radius, size_t numSides, D3DCOLOR color)
	{
		if (radius == 0.0f || numSides < 3)
			return;

		float step = D3DX_PI * 2.0f / (float)numSides;

		D3DTLVERTEX* pVertices = new D3DTLVERTEX[numSides + 1];

		// Load starting & ending points
		pVertices[0] = pVertices[numSides] =
			TL2DVERT(
				radius * cosf(step) + x,
				radius * sinf(step) + y,
				color
			);

		float current = step * 2;

		for (size_t i = 1; i < numSides; ++i)
		{
			float x0 = radius * cosf(current) + x;
			float y0 = radius * sinf(current) + y;

			pVertices[i] = TL2DVERT(x0, y0, color);

			current += step;
		}

		Device->DrawPrimitiveUP(D3DPT_LINESTRIP, numSides, pVertices, sizeof(D3DTLVERTEX));
		delete[] pVertices;
	}

	void FilledCircle(float x, float y, float radius, size_t numSides, D3DCOLOR color)
	{
		if (radius == 0.0f || numSides < 3)
			return;

		float step = D3DX_PI * 2.0f / (float)numSides;

		D3DTLVERTEX* pVertices = new D3DTLVERTEX[numSides + 2];

		size_t lastVert = numSides + 1;

		// Load starting & ending points

		pVertices[0] = TL2DVERT(x, y, color);
		pVertices[1] = pVertices[lastVert] =
			TL2DVERT(
				radius * cosf(step) + x,
				radius * sinf(step) + y,
				color
			);

		float current = step * 2;

		for (size_t i = 2; i < lastVert; ++i)
		{
			float x0 = radius * cosf(current) + x;
			float y0 = radius * sinf(current) + y;

			pVertices[i] = TL2DVERT(x0, y0, color);

			current += step;
		}

		Device->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, numSides, pVertices, sizeof(D3DTLVERTEX));
		delete[] pVertices;
	}

	void GradientDown(float x, float y, float width, float height, D3DCOLOR topColor, D3DCOLOR bottomColor)
	{
		if (width <= 0.0f || height <= 0.0f)
			return;

		float xw = x + width;
		float yh = y + height;

		D3DTLVERTEX vertices[] =
		{
			TL2DVERT(x,  y,  topColor),
			TL2DVERT(xw, y,  topColor),
			TL2DVERT(x,  yh, bottomColor),
			TL2DVERT(xw, yh, bottomColor)
		};

		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(D3DTLVERTEX));
	}

	void GradientSideways(float x, float y, float width, float height, D3DCOLOR leftColor, D3DCOLOR rightColor)
	{
		if (width <= 0.0f || height <= 0.0f)
			return;

		float xw = x + width;
		float yh = y + height;

		D3DTLVERTEX vertices[] =
		{
			TL2DVERT(x,  yh, leftColor),
			TL2DVERT(x,  y,  leftColor),
			TL2DVERT(xw, yh, rightColor),
			TL2DVERT(xw, y,  rightColor)
		};

		Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, vertices, sizeof(D3DTLVERTEX));
	}
}