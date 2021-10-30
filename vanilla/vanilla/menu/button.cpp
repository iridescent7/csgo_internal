#include "Button.h"
#include "..\globals.h"

void Button::Scale()
{
	width  = DYN_X(200);
	height = DYN_Y(24);

	g_renderer->GetTextSizeA(g_fonts.text, text, &_w, &_h);
}

void Button::Paint()
{
	g_renderer->DrawSoftFilledRect(x,     y,     width,     height,     MENU_COLOR(g_colors.outline));
	g_renderer->DrawSoftFilledRect(x + 1, y + 1, width - 2, height - 2, (_hover && !_clicked) ? MENU_COLOR(g_colors.highlight) : MENU_COLOR(g_colors.foreground));

	g_renderer->DrawOutlinedText(g_fonts.text, text, x + (width / 2 - _w / 2), y + (height / 2 - _h / 2), MENU_COLOR(g_colors.background), MENU_COLOR(g_colors.outline));
}

bool Button::Update(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (!_callback)
		return false;

	if (msg == WM_MOUSEMOVE)
	{
		float mx = (float)GET_X_LPARAM(lParam),
		      my = (float)GET_Y_LPARAM(lParam);

		_hover = (mx >= x && my >= y && mx <= (x + width) && my <= (y + height));
	}
	else if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK)
	{
		float mx = (float)GET_X_LPARAM(lParam),
			  my = (float)GET_Y_LPARAM(lParam);

		if (mx >= x           && my >= y
		 && mx <= (x + width) && my <= (y + height))
		{
			_clicked = true;

			_callback(this);
			return true;
		}
	}
	else if (msg == WM_LBUTTONUP)
	{
		_clicked = false;
	}

	return false;
}