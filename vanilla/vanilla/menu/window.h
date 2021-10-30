#pragma once
#include "control.h"
#include <vector>
#include "tab.h"

class Window : public BaseControl
{
public:
	void Paint();
	void Scale();

	bool Update(UINT msg, WPARAM wParam, LPARAM lParam);

	Window()
	{
		this->_index     = 0;
		this->_lastindex = 0;
		this->_starttick = 0;

		this->width  = 0;
		this->height = 0;

		this->_tab_title_start_x = 0;
		this->_tab_title_last_x  = 0;
	}

	Window(enc_string title, float x, float y, const std::vector<Tab>& tabs, unsigned long index)
	{
		this->x = x;
		this->y = y;

		this->title= title;

		this->_tabs      = tabs;
		this->_index     = index;
		this->_lastindex = index;

		this->_starttick = 0;

		this->width  = 0;
		this->height = 0;

		this->_tab_title_start_x = 0;
		this->_tab_title_last_x  = 0;

		this->_tab_titles.resize(tabs.size(), { 0 });
	}

public:
	enc_string title;

private:
	std::vector<Tab> _tabs;
	std::vector<BaseControl> _tab_titles;

	unsigned long _index;
	unsigned long _lastindex;
	unsigned long _starttick;

	float _tab_title_start_x,
		  _tab_title_last_x;
};
