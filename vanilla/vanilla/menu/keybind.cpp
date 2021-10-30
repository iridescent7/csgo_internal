#include "keybind.h"
#include "..\globals.h"
#include <sstream>
#include <iomanip>

__forceinline void GetKeyCodeString(uint32_t keycode, std::string* str)
{
	if (keycode == NULL)
	{
		*str = strenc("none");
		return;
	}

	switch (keycode)
	{
	case VK_LBUTTON:  *str = strenc("mouse1"); return;
	case VK_RBUTTON:  *str = strenc("mouse2"); return;
	case VK_MBUTTON:  *str = strenc("mouse3"); return;
	case VK_XBUTTON1: *str = strenc("mouse4"); return;
	case VK_XBUTTON2: *str = strenc("mouse5"); return;

	case VK_BACK: *str = strenc("backspace"); return;
	case VK_TAB:  *str = strenc("tab");       return;

	//case VK_RETURN: *str = strenc("enter"); return;

	case VK_SHIFT:   *str = strenc("shift");    return;
	case VK_CONTROL: *str = strenc("ctrl");     return;
	case VK_MENU:    *str = strenc("alt");      return;
	case VK_PAUSE:   *str = strenc("pause");    return;
	case VK_CAPITAL: *str = strenc("capslock"); return;
	case VK_NUMLOCK: *str = strenc("numlock");  return;
	//case VK_SCROLL:  *str = strenc("scroll-lock"); return; why would we need this

	//case VK_ESCAPE: *str = strenc("escape"); return;
	case VK_SPACE:  *str = strenc("space");    return;
	case VK_PRIOR:  *str = strenc("pageup");   return;
	case VK_NEXT:   *str = strenc("pagedown"); return;
	case VK_END:    *str = strenc("end");      return;
	case VK_HOME:   *str = strenc("home");     return;

	case VK_LEFT:  *str = strenc("left");  return;
	case VK_UP:    *str = strenc("up");    return;
	case VK_RIGHT: *str = strenc("right"); return;
	case VK_DOWN:  *str = strenc("down");  return;

	//case VK_INSERT: *str = strenc("insert"); return;
	case VK_DELETE:   *str = strenc("del"); return;

	// i think we need to specify that the key belongs to the numpad since the same chars exists in a different vk code
	case VK_MULTIPLY:  *str = strenc("num *"); return;
	case VK_ADD:       *str = strenc("num +"); return;
	case VK_SEPARATOR: *str = strenc("num ?"); return;
	case VK_SUBTRACT:  *str = strenc("num -"); return;
	case VK_DECIMAL:   *str = strenc("num ."); return;
	case VK_DIVIDE:    *str = strenc("num /"); return;

	case VK_OEM_1:      *str = strenc("; key"); return;
	case VK_OEM_PLUS:   *str = strenc("= key"); return;
	case VK_OEM_MINUS:  *str = strenc("- key"); return;
	case VK_OEM_PERIOD: *str = strenc(". key"); return;
	case VK_OEM_COMMA:  *str = strenc(", key"); return;

	case VK_OEM_2: *str = strenc("/ key");    return;
	case VK_OEM_3: *str = strenc("~ key");    return;
	case VK_OEM_4: *str = strenc("[ key");    return;
	case VK_OEM_5: *str = strenc("\\ key");   return;
	case VK_OEM_6: *str = strenc("] key");    return;
	case VK_OEM_7: *str = strenc("' key");    return;
	case VK_OEM_8: *str = strenc("misc key"); return;
	}

	if (keycode >= VK_NUMPAD0 && keycode <= VK_NUMPAD9)
		*str = strenc("num ") + std::to_string(keycode - VK_NUMPAD0);

	else if (keycode >= VK_F1 && keycode <= VK_F24)
		*str = strenc("F") + std::to_string((keycode - VK_F1) + 1);

	else if (keycode >= '0' && keycode <= '9')
		*str = (char)keycode;

	else if (keycode >= 'A' && keycode <= 'Z')
		*str = (char)(keycode + ('a' - 'A'));

	else
	{
		std::ostringstream oss;
		oss << strenc("0x") << std::hex << keycode;
		*str = oss.str();
	}
}

void Keybind::Scale()
{
	width  = DYN_X(200);
	height = DYN_Y(24);

	_box_width       = DYN_X(60);
	_mode_text_pad_x = DYN_X(8);
}

void Keybind::Paint()
{
	std::vector<std::string> modes =
	{
		strenc("always"),
		strenc("hold"),
		strenc("toggle")
	};

	float box_start_x = (x + width - _box_width);

	float h;
	g_renderer->GetTextSizeA(g_fonts.text, text, NULL, &h);
	g_renderer->DrawOutlinedText(g_fonts.text, text, x, y + (height / 2 - h / 2), MENU_COLOR(g_colors.text), MENU_COLOR(g_colors.outline));

	g_renderer->DrawSoftFilledRect(box_start_x,     y,     _box_width,     height,     _wait_key ? MENU_COLOR(g_colors.outline_highlight) : MENU_COLOR(g_colors.outline));
	g_renderer->DrawSoftFilledRect(box_start_x + 1, y + 1, _box_width - 2, height - 2, MENU_COLOR(g_colors.foreground));

	std::string text;

	if (_key)
	{
		float mw, mh;
		std::string mode_text = modes[_key->mode];

		g_renderer->GetTextSizeA(g_fonts.text, mode_text.c_str(), &mw, &mh);
		g_renderer->DrawOutlinedText(g_fonts.text, mode_text.c_str(), box_start_x - mw - _mode_text_pad_x, y + (height / 2 - mh / 2), MENU_COLOR(g_colors.background), MENU_COLOR(g_colors.outline));

		if (_wait_key)
			text = '-';
		else
			GetKeyCodeString(_key->key, &text);
	}
	else
		text = strenc("not bound");

	float tw, th;
	g_renderer->GetTextSizeA(g_fonts.text, text.c_str(), &tw, &th);
	g_renderer->DrawOutlinedText(g_fonts.text, text.c_str(), box_start_x + (_box_width / 2 - tw / 2), y + (height / 2 - th / 2), MENU_COLOR(g_colors.text), MENU_COLOR(g_colors.outline));

	if (_wait_mode && g_focus == this)
	{
		float tall    = height;
		float mheight = tall * 3;

		g_renderer->DrawFilledRect(box_start_x,     y + height, _box_width,     mheight,     MENU_COLOR(g_colors.outline));
		g_renderer->DrawFilledRect(box_start_x + 1, y + height, _box_width - 2, mheight - 1, MENU_COLOR(g_colors.foreground));

		float my = y + height;

		for (size_t i = 0; i < modes.size(); ++i)
		{
			if (_key && _key->mode == i)
			{
				g_renderer->DrawFilledRect(box_start_x+1, y + height + (i * tall)+1, _box_width-2, tall-2, MENU_COLOR(g_colors.highlight));
			}

			float mw, mh;
			g_renderer->GetTextSizeA(g_fonts.text, modes[i].c_str(), &mw, &mh);
			g_renderer->DrawOutlinedText(
				g_fonts.text,
				modes[i].c_str(),
				box_start_x + (_box_width / 2 - mw / 2),
				my + (tall / 2 - mh / 2),
				MENU_COLOR(g_colors.text),
				MENU_COLOR(g_colors.outline));

			my += tall;
		}
    }
}

bool Keybind::Update(UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (!_key)
		return false;

	float box_start_x = (x + width - _box_width);

	float mx = (float)GET_X_LPARAM(lParam),
		  my = (float)GET_Y_LPARAM(lParam);

	if (_wait_key || _wait_mode)
	{
		if (g_focus != this)
		{
			_wait_key  = false;
			_wait_mode = false;
			return false;
		}

		if (msg == WM_KEYDOWN && wParam == VK_ESCAPE)
		{
			_wait_key  = false;
			_wait_mode = false;

			g_focus = nullptr;
			return true;
		}
	}
	else if (mx >= box_start_x              && my >= y
		  && mx <= box_start_x + _box_width && my <= y + height)
	{
		if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK)
		{
			_key->key = NULL;

			_wait_key = true;
			g_focus   = this;
			return true;
		}
		else if (msg == WM_RBUTTONDOWN || msg == WM_RBUTTONDBLCLK)
		{
			_wait_mode = true;
			g_focus    = this;
			return true;
		}
	}

	if (_wait_key)
	{
		uint32_t key = NULL;

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
			// https://docs.microsoft.com/en-us/windows/win32/inputdev/wm-keydown
			if (lParam & 2) // 2 is 0000 0010 and we need the last 2nd bit for the value
				break; // if key is already presssed before we start listening, ignore

			switch (wParam)
			{
			case VK_RETURN:
			case VK_INSERT:
			case VK_BACK:
			case VK_LWIN:
			case VK_RWIN:
			case VK_APPS:
			{
				_wait_key = false;
				g_focus   = nullptr;
				return true;
			}
			default:
				break;
			}

			key = wParam;
			break;
		}
		default:
			break;
		}

		if (key)
		{
			_key->key = key;
			_wait_key = false;

			g_focus = nullptr;
			return true;
		}
	}
	else if (_wait_mode)
	{
		if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK)
		{
			float tall = DYN_Y(24);

			if (mx >= box_start_x              && my >= y + height
			 && mx <= box_start_x + _box_width && my <= y + height + tall * 3)
			{
				float delta = my - (y + height);

				_key->mode = min((int32_t)(delta / tall), 2);
				
				_wait_mode = false;
				g_focus    = nullptr;
				return true;
			}

			_wait_mode = false;
			g_focus    = nullptr;
		}
	}

	return false;
}
