#include "GroupBox.h"
#include "..\globals.h"

void Groupbox::Scale()
{
	_ctrl_pad_x = DYN_X(12);
	_ctrl_pad_y = DYN_Y(12 + 9); // +half text height

	_ctrl_indent_y = DYN_Y(5);

	float h = _ctrl_pad_y + (_ctrl_indent_y * (_controls.size() - 1)) + DYN_Y(12);

	for (Control*& c : _controls)
	{
		c->Scale();
		h += c->height;
	}

	width  = DYN_X(200) + _ctrl_pad_x * 2;
	height = h;

	g_renderer->GetTextSizeA(g_fonts.text, text, &_w, &_h);
}

void Groupbox::Paint()
{
	float xwh = x + width / 2 - _w / 2;

	g_renderer->DrawSoftFilledRect(x,     y,     width,     height,     MENU_COLOR(g_colors.foreground));
	g_renderer->DrawSoftFilledRect(x + 1, y + 1, width - 2, height - 2, MENU_COLOR(g_colors.background));

	g_renderer->DrawFilledRect(xwh, y - _h / 2, _w, _h, MENU_COLOR(g_colors.background));
	g_renderer->DrawOutlinedText(g_fonts.text, text, xwh, y - _h / 2, MENU_COLOR(g_colors.background), MENU_COLOR(g_colors.outline));

	float cx = x + _ctrl_pad_x,
		  cy = y + _ctrl_pad_y;

	for (Control*& c : _controls)
	{
		if (!c->present)
			continue;

		c->SetPosition(cx, cy);
		c->Paint();

		cy += c->height + _ctrl_indent_y;
	}
}

bool Groupbox::Update(UINT msg, WPARAM wParam, LPARAM lParam)
{
	for (Control*& c : _controls)
	{
		if (!c->present)
			continue;

		if (c->Update(msg, wParam, lParam))
			return true;
	}
	
	return false;
}