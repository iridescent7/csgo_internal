#include "tab.h"
#include "..\globals.h"

void Tab::Scale()
{
	for (Control*& c : _controls)
		c->Scale();

	_ctrl_indent_y = DYN_Y(5);
}

void Tab::Paint()
{
	float cy = y;

	for (Control*& c : _controls)
	{
		if (!c->present)
			continue;

		c->SetPosition(x, cy);

		if(c != g_focus)
			c->Paint();

		cy += c->height + _ctrl_indent_y;
	}

	if (g_focus && g_focus->present)
		g_focus->Paint();
}

bool Tab::Update(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (g_focus && g_focus->present)
	{
		if (msg == WM_MOUSEMOVE)
		{
			_missed_mmove        = true;
			_missed_mmove_lparam = lParam;
		}

		bool ret = g_focus->Update(msg, wParam, lParam);

		// if the focused control is closed and last mouse move was skipped.
		// we want to update the lost mousemove to all controls who needed it
		if (!g_focus && _missed_mmove)
		{
			for (Control*& c : _controls)
				c->Update(WM_MOUSEMOVE, 0, _missed_mmove_lparam);

			_missed_mmove        = false;
			_missed_mmove_lparam = 0; // unneeded, but whatever
		}

		return ret;
	}

	for (Control*& c : _controls)
	{
		if (!c->present)
			continue;

		if (c->Update(msg, wParam, lParam))
			return true;
	}

	return false;
}