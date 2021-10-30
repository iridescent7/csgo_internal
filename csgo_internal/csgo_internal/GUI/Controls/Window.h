#pragma once

#include "Control.h"
#include <Windows.h>
#include <vector>
#include "Tab.h"

class Window : public BaseControl
{
public:
	void Draw();
	bool Think();

	Window() { }
	Window(std::string text, float x,float y,float width, float height, const std::vector<Tab>& tabs, size_t index = 0)
	{
		this->Title = text;

		this->x = x;
		this->y = y;

		this->Tabs = tabs;
		this->Index = index;

		this->width = width;
		this->height = height;

		this->Dragging = false;
	}

public:
	std::vector<Tab> Tabs;
	size_t           Index;

	std::string		 Title;

	bool		     Dragging;
	POINT		     DragOffset;
};
