#pragma once

#include "Control.h"
#include <vector>

class GroupBox : public Control, public ControlParent
{
public:
	virtual void Draw();
	virtual bool Think();

	GroupBox(std::string text, float width, float height, const std::vector<Control*>& controls)
	{
		this->Text = text;

		this->width  = width;
		this->height = height;

		this->Controls = controls;
		this->Focus    = nullptr;

		for (auto& control : this->Controls)
			control->parent = this;
	}

public:
	std::vector<Control*> Controls;
};
