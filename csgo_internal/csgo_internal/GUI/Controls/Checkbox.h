#pragma once
#include "Control.h"

class CheckBox : public Control
{
public:
	virtual void Draw();
	virtual bool Think();

	CheckBox(std::string text, bool* checked = nullptr)
	{
		this->Text    = text;
		this->Checked = checked;

		this->width  = 100;
		this->height = 14;
	}

public:
	bool* Checked;
};
