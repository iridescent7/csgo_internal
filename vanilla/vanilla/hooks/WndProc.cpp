#include "..\hooks.h"
#include "..\globals.h"
#include <Windows.h>

LRESULT CALLBACK hooks::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	uint32_t key  = NULL;
	bool firstkey = true;
	bool keyup    = false;

	switch (msg)
	{
	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		key = VK_LBUTTON;
		break;
	case WM_RBUTTONDOWN:
	case WM_RBUTTONDBLCLK:
		key = VK_RBUTTON;
		break;
	case WM_MBUTTONDOWN:
	case WM_MBUTTONDBLCLK:
		key = VK_MBUTTON;
		break;
	case WM_XBUTTONDOWN:
	case WM_XBUTTONDBLCLK:
		key = (lParam == XBUTTON1) ? VK_XBUTTON1 : VK_XBUTTON2;
		break;
	case WM_KEYDOWN:
	{
		key = wParam;
		firstkey = !(lParam & 2);
		break;
	}

	case WM_LBUTTONUP:
		key = VK_LBUTTON;
		keyup = true;
		break;
	case WM_RBUTTONUP:
		key = VK_RBUTTON;
		keyup = true;
		break;
	case WM_MBUTTONUP:
		key = VK_MBUTTON;
		keyup = true;
		break;
	case WM_XBUTTONUP:
		key = (lParam == XBUTTON1) ? VK_XBUTTON1 : VK_XBUTTON2;
		keyup = true;
		break;
	case WM_KEYUP:
		key = wParam;
		keyup = true;
		break;

	default:
		break;
	}

	if (key)
	{
		hotkey_t* hotkeys = (hotkey_t*)&g_hotkeys;
		size_t    count   = sizeof(g_hotkeys) / sizeof(hotkey_t);

		for (size_t i = 0; i < count; ++i)
		{
			if (hotkeys[i].key != key)
				continue;
	
			if (hotkeys[i].mode == hk_always)
				continue;

			if (hotkeys[i].mode == hk_hold)
			{
				hotkeys[i].pressed = keyup ? false : true;
			}
			else // hk_toggle
			{
				if (hotkeys[i].key == key && firstkey)
				{
					hotkeys[i].pressed = !hotkeys[i].pressed;
				}
			}
		}
	}

	g_menu.Update(msg, wParam, lParam);

	if (g_menu.open)
		return TRUE;

	return _CallWindowProcA(hooks::oWndProc, hwnd, msg, wParam, lParam);
}
