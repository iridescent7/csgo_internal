#pragma once

#include "Control.h"
#include <vector>

class ComboBox : public Control
{
public:
	virtual void Draw();
	virtual bool Think();

	ComboBox(const char* text, const std::vector<std::string>& items, int* index = nullptr)
	{
		this->Items = items;
		this->Index = index;

		this->Opened = false;
		this->width  = 100;
		this->height = 20;

		if (text)
		{
			this->Text = text;
			this->height += 18 + 2;
		}
	}

public:
	bool Opened;

	std::vector<std::string> Items;
	int*                     Index;
};
