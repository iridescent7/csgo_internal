#include "checkbox.h"
#include "..\globals.h"

void Checkbox::Scale()
{
	width  = DYN_X(200);
	height = DYN_Y(16);

	_text_pad_x = DYN_X(8);

	g_renderer->GetTextSizeA(g_fonts.text, text, &_w, &_h);
}

void Checkbox::Paint()
{
	bool  ischecked = (_checked && *_checked);
	float size = height;

	g_renderer->DrawSoftFilledRect(x,     y,     size,     size,     MENU_COLOR(g_colors.outline));
	g_renderer->DrawSoftFilledRect(x + 1, y + 1, size - 2, size - 2, ischecked ? MENU_COLOR(g_colors.foreground) : MENU_COLOR(g_colors.background));

	g_renderer->DrawOutlinedText(g_fonts.text, text, x + size + _text_pad_x, y + (height / 2 - _h / 2), MENU_COLOR(g_colors.text), MENU_COLOR(g_colors.outline));
}

bool Checkbox::Update(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (!_checked)
		return false;

	if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK)
	{
		float mx = (float)GET_X_LPARAM(lParam),
			  my = (float)GET_Y_LPARAM(lParam);

		float w;
		g_renderer->GetTextSizeA(g_fonts.text, text, &w, NULL);

		if ( mx >= x && my >= y
			&& mx <= (x + height + _text_pad_x + w)
			&& my <= (y + height))
		{
			*_checked = !*_checked;

			if (_callback)
				_callback(this);

			return true;
		}
	}

	return false;
}