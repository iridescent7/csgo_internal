#pragma once

#include "Control.h"
#include <vector>

class Tab : public BaseControl, public ControlParent
{
public:
	void Draw();
	bool Think();

	Tab(std::string title, const std::vector<Control*>& controls)
	{
		this->Title = title;

		this->Controls = controls;
		this->Focus    = nullptr;

		for (auto& control : Controls)
			control->parent = this;
	}

public:
	std::vector<Control*> Controls;
	std::string			  Title;
};
