#include "menu.h"
#include <d3dx9.h>
#include "globals.h"
#include "controls.h"

CheatMenu::CheatMenu()
{
	open  = false;
	alpha = 0xFF;

	_dragging  = false;
	_animating = false;
	_firstdrag = true;

	_dragx = 0.f;
	_dragy = 0.f;

	_animTick = 0;

	_window =
	Window(encstr("vanilla"), 200, 200,
	{
		Tab(encstr("RAGEBOT"),
		{
			new Groupbox(encstr("Aim"),
			{
				new Checkbox(encstr("Enabled"), &g_vars.aimbot.enabled),
				new Keybind(encstr("Hotkey"), &g_hotkeys.aimbot),

				new SliderF(encstr("Max fov"), &g_vars.aimbot.fov,1.f, 180.f)
			})
		}),
		Tab(encstr("LEGITBOT")),
		Tab(encstr("VISUAL")),
		Tab(encstr("MISC"))
	}, 0);
}

void CheatMenu::Scale()
{
	_window.Scale();
}

void CheatMenu::Paint()
{
	if (!open && !_animating)
		return;

	constexpr auto
		fade_in_time  = 200,
		fade_out_time = 200;

	DWORD deltaTick = _GetTickCount() - _animTick;

	if (_animating)
	{
		if (open)
		{
			if (deltaTick < fade_in_time)
				alpha = (unsigned char)((deltaTick / (float)fade_in_time * 255.f) + 0.5f);
			else
				alpha = 255;
		}
		else
		{
			if (deltaTick < fade_out_time)
				alpha = (unsigned char)(((fade_out_time - deltaTick) / (float)fade_out_time * 255.f) + 0.5f);
			else
			{
				alpha      = 0;
				_animating = false;
			}
		}
	}

	_window.Paint();
}

void CheatMenu::Update(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_KEYDOWN && wParam == VK_INSERT)
	{
		open = !open;

		_animating = true;
		_animTick  = _GetTickCount();

		return;
	}

	if (!open)
	{
		if (_animating)
		{
			// we're doing fade out anim
			// so we still have interest for mousemove incase we're dragging
			if (!_dragging || msg != WM_MOUSEMOVE)
				return;
		}
		else
		{
			_dragging = false;
			return;
		}
	}

	// ======================================================
	// handles mouse button swap / left handed mouse settings
	// fits raw mouse pos into client area
	// ======================================================

	auto& swapmsbutton = [](WPARAM& wparam)
	{
		WPARAM newparam = (wparam & ~(MK_LBUTTON | MK_RBUTTON)) |
			((wparam & MK_LBUTTON) ? MK_RBUTTON : 0) |
			((wparam & MK_RBUTTON) ? MK_LBUTTON : 0);

		wparam = newparam;
	};

	if (msg == WM_MOUSEMOVE)
	{
		if (_GetSystemMetrics(SM_SWAPBUTTON))
			swapmsbutton(wParam);
	}
	// check if it's a mouse button input
	else if (msg >= WM_LBUTTONDOWN && msg <= WM_RBUTTONDBLCLK)
	{
		if (_GetSystemMetrics(SM_SWAPBUTTON))
		{
			// the delta between left & right mouse button message code
			UINT delta = WM_RBUTTONDOWN - WM_LBUTTONDOWN; // 3

			// right mouse button are pressed
			if (msg > WM_LBUTTONDBLCLK)
			{
				msg -= delta;
			}
			else
			{
				msg += delta;
			}

			swapmsbutton(wParam);
		}
	}

	// return if message is already handled by window/child controls
	if (_window.Update(msg, wParam, lParam))
		return;
	
	switch (msg)
	{
	case WM_MOUSEMOVE:
	{
		if (_dragging)
		{
			if (!(wParam & MK_LBUTTON))
			{
				_dragging = false;
				break;
			}

			float mx = (float)GET_X_LPARAM(lParam),
			      my = (float)GET_Y_LPARAM(lParam);

			float x = mx - _dragx,
				  y = my - _dragy;

			if (_firstdrag)
			{
				float dragPx = (float)_GetSystemMetrics(SM_CXDRAG);

				if (fabs(x - _window.x) >= dragPx
				 || fabs(y - _window.y) >= dragPx)
				{
					_window.SetPosition(x, y);
					_firstdrag = false;
				}
			}
			else
			{
				_window.SetPosition(x, y);
			}
		}
		break;
	}
	case WM_LBUTTONDOWN:
	{
		if (_dragging)
			return;
		
		float mx = (float)GET_X_LPARAM(lParam),
			  my = (float)GET_Y_LPARAM(lParam);

		if (mx < _window.x || mx > _window.x + _window.width)
			return;

		if (my < _window.y || my > _window.y + _window.height)
			return;

		_dragx = mx - _window.x;
		_dragy = my - _window.y;

		_firstdrag = true;
		_dragging  = true;

		g_focus = nullptr;

		break;
	}
	case WM_LBUTTONUP:
	{
		_dragging = false;
		break;
	}
	default:
		break;
	}
}
