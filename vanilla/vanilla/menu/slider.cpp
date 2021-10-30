#include "slider.h"
#include "..\globals.h"

void Slider::Scale()
{
	g_renderer->GetTextSizeA(g_fonts.text, text, &_w, &_h);

	_bar_pad_y = _h + DYN_Y(2);

	width  = DYN_X(200);
	height = _bar_pad_y + DYN_Y(24);

	_bar_height = DYN_Y(12);
}

void Slider::Paint()
{
	auto& DrawSlider = [](float x, float y, float width, float height, D3DCOLOR color)
	{
		size_t numSides = 30;

		float step   = D3DX_PI * 2.0f / (float)numSides;
		float radius = height / 2;

		TLVertex* pVertices = new TLVertex[numSides + 2];
		size_t    lastVert  = numSides + 1;

		// Load starting point
		pVertices[0] = { x + radius, y + radius, 0.f, 1.f, color };

		float current = step * 2;

		for (size_t i = 1; i <= lastVert; ++i)
		{
			float cx;
			size_t p = i;

			// within Quadrant IV & I
			if ((p <= numSides * 1/4) || (p > numSides * 3/4))
				cx = x + width;
			else
				cx = x;

			float x0 = radius * cosf(current) + cx;
			float y0 = radius * sinf(current) + y + radius;

			pVertices[i] = { x0, y0, 0.f, 1.f, color };

			current += step;
		}

		g_renderer->DrawPrimitiveUP(D3DPT_TRIANGLEFAN, numSides, pVertices, sizeof(TLVertex));
		delete[] pVertices;
	};

	g_renderer->DrawOutlinedText(g_fonts.text, text, x, y, MENU_COLOR(g_colors.background), MENU_COLOR(g_colors.outline));

	DrawSlider(x,     y + _bar_pad_y,     width,     _bar_height,     MENU_COLOR(g_colors.outline));
	DrawSlider(x + 1, y + _bar_pad_y + 1, width - 1, _bar_height - 2, MENU_COLOR(g_colors.background));

	if (_value)
	{
		float val_width = ((float)(*_value - _min) / (float)(_max - _min)) * (width - 1);
		DrawSlider(x + 1, y + _bar_pad_y + 1, val_width, _bar_height - 2, MENU_COLOR(g_colors.foreground));

		std::string val_str = std::to_string(*_value);

		float tw, th;
		g_renderer->GetTextSizeA(g_fonts.text, val_str.c_str(), &tw, &th);
		g_renderer->DrawOutlinedText(g_fonts.text, val_str.c_str(), x + val_width - tw / 2, y + _bar_pad_y + _bar_height, MENU_COLOR(g_colors.background), MENU_COLOR(g_colors.outline));
	}
}

bool Slider::Update(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (!_value)
		return false;

	if (msg == WM_MOUSEMOVE)
	{
		if (_clicked)
		{
			float mx = (float)GET_X_LPARAM(lParam),
				  my = (float)GET_Y_LPARAM(lParam);

			float amt = (mx - x);
			utils::Clamp(&amt, 0.f, width);

			*_value = (int)((amt / width) * (_max - _min) + _min + 0.5f);
		}
	}
	else if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK)
	{
		float mx = (float)GET_X_LPARAM(lParam),
			  my = (float)GET_Y_LPARAM(lParam);

		if (mx >= (x + 1)         && my >= y + _bar_pad_y
		 && mx <= (x + width - 1) && my <= (y +_bar_pad_y+_bar_height))
		{
			_clicked = true;
			g_focus  = this;

			*_value = (int)(((mx - x) / width) * (_max - _min) + _min + 0.5f);
			return true;
		}
	}
	else if (msg == WM_LBUTTONUP)
	{
		_clicked = false;
		g_focus  = nullptr;
	}

	return false;
}