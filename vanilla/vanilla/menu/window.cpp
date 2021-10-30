#include "window.h"
#include "..\globals.h"

void Window::Scale()
{
	// todo setup all window scaling variables here
	_tabs[_index].Scale();
}

void Window::Paint()
{
	width  = DYN_X(850);
	height = DYN_Y(550);

	float
		x_tc_radius = DYN_X(2),
		y_tc_radius = DYN_Y(2),

		frame_thick_width  = DYN_X(6),
		frame_thick_height = DYN_Y(6),

		tb_height   = DYN_Y(50),
		title_x_pad = DYN_X(8),

		tab_x_pad = DYN_X(12),
		tab_y_pad = DYN_Y(12),

		tab_title_x_indent = DYN_X(30),
		tab_title_x_pad    = DYN_X(44);

	float cx = x,
		cy = y,
		cw = width,
		ch = height;

	// draw frame
	g_renderer->DrawSoftFilledRect(cx, cy, cw, ch, MENU_COLOR(g_colors.outline));

	cx += 1, cy += 1;
	cw -= 2, ch -= 2;

	g_renderer->DrawSoftFilledRect(cx, cy, cw, ch, MENU_COLOR(g_colors.foreground));

	cx += (frame_thick_width + 1);
	cy += tb_height;

	cw -= (frame_thick_width  + 1) * 2;
	ch -= (frame_thick_height + 1) + tb_height;

	g_renderer->DrawSoftFilledRect(cx, cy, cw, ch, MENU_COLOR(g_colors.outline));

	cx += 1, cy += 1;
	cw -= 2, ch -= 2;

	// draw background
	g_renderer->DrawSoftFilledRect(cx, cy, cw, ch, MENU_COLOR(g_colors.background));

	float tb_y_mid = y + 1 + tb_height / 2;

	float tw, th;
	g_renderer->GetTextSizeA(g_fonts.title, title, &tw, &th);

	float tx = cx + title_x_pad,
		  ty = tb_y_mid - (th / 2);

	// draw title text
	g_renderer->DrawOutlinedText(g_fonts.title, title, tx, ty, MENU_COLOR(g_colors.title), MENU_COLOR(g_colors.outline));

	// setup tab titles area
	float _tabx = (float)(tx + tw) + tab_title_x_pad;
	_tab_title_start_x = _tabx;

	for (size_t i = 0; i < _tabs.size(); ++i)
	{
		float tabw, tabh;
		g_renderer->GetTextSizeA(g_fonts.tab, _tabs[i].title, &tabw, &tabh);

		float taby = tb_y_mid - tabh / 2;

		// store size data
		_tab_titles[i] = { _tabx, taby, tabw, tabh };
		_tabx += (float)tabw + tab_title_x_indent;
	}

	_tab_title_last_x = _tabx;

	// draw active tab highlight w/ animation
	float hx, hw;

	if (_index != _lastindex)
	{
		auto& cur  = _tab_titles[_index];
		auto& last = _tab_titles[_lastindex];

		unsigned long duration = 200;
		unsigned long tick = _GetTickCount() - _starttick;

		if (tick >= duration)
		{
			_lastindex = _index;
		}

		if (_index < _lastindex)
			hx = last.x - ((last.x - cur.x) * tick / duration);
		else
			hx = last.x + ((cur.x - last.x) * tick / duration);

		if (cur.width != last.width)
		{
			if (cur.width > last.width)
				hw = last.width + ((cur.width - last.width) * tick / duration);
			else
				hw = last.width - ((last.width - cur.width) * tick / duration);
		}
		else
			hw = cur.width;
	}
	else
	{
		hx = _tab_titles[_index].x;
		hw = _tab_titles[_index].width;
	}

	g_renderer->DrawFilledRect(
		hx - (tab_title_x_indent / 2), y + 1,
		hw + tab_title_x_indent,       cy - (y + 1),
		MENU_COLOR(g_colors.background));

	// draw tab title text
	for (size_t i = 0; i < _tabs.size(); ++i)
	{
		float tabx = _tab_titles[i].x,
			taby = tb_y_mid - (_tab_titles[i].height / 2);

		g_renderer->DrawOutlinedText(g_fonts.tab, _tabs[i].title, tabx, taby, MENU_COLOR(g_colors.tab), MENU_COLOR(g_colors.outline));
	}

	// draw our tab content
	
	float tcx = x + (frame_thick_width + 2 + tab_x_pad),
	  	  tcy = y + (tb_height + 2 + tab_y_pad),

		  tcwidth  = width  - (tcx * 2),
		  tcheight = height - (tcy + frame_thick_height + 2 + tab_y_pad);

	_tabs[_index].SetPosition(tcx, tcy);
	_tabs[_index].SetSize(tcwidth, tcheight);
	_tabs[_index].Paint();
}

bool Window::Update(UINT msg, WPARAM wParam, LPARAM lParam)
{
	float
		tb_height          = DYN_Y(50),
		tab_title_x_indent = DYN_X(30);

	// if already handled, then return
	if (_tabs[_index].Update(msg, wParam, lParam))
		return true;

	if (msg == WM_LBUTTONDOWN || msg == WM_LBUTTONDBLCLK)
	{
		float mx = (float)GET_X_LPARAM(lParam),
			  my = (float)GET_Y_LPARAM(lParam);

		if (my < (y + 1) || my >(y + tb_height + 2))
			return false;

		float half_indent   = tab_title_x_indent / 2,
			first_tab_start = _tab_title_start_x - half_indent,
			last_tab_end    = _tab_title_last_x - half_indent;

		if (mx < first_tab_start || mx > last_tab_end)
			return false;

		unsigned long index = 0;
		float lx = mx - first_tab_start;

		for(;;)
		{
			lx -= _tab_titles[index].width + tab_title_x_indent;

			if ((int)lx <= 0)
				break;

			++index;
		}

		index = min(index, _tabs.size() - 1);

		if (index != _index)
		{
			g_focus = nullptr;
			_lastindex = _index;
			_starttick = _GetTickCount();
			_index = index;

			return true;
		}

		return false;
	}

	return false;
}
