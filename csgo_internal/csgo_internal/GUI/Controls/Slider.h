#pragma once
#include "Control.h"

class Slider : public Control
{
protected:
	virtual void Draw();
	virtual bool Think();

public:
	Slider(const char* text, int* value = nullptr, int min=0, int max=100)
	{
		this->Min = min;
		this->Max = max;

		if (value)
		{
			if (*value < min)
				*value = min;
			else if (*value > max)
				*value = max;
		}

		this->Value = value;
		this->ValueString = this->Value ? std::to_string(*value) : "";

		this->Dragging = false;

		this->width = 100;
		this->height = 17;

		if (text)
		{
			this->Text = text;
			this->height += 18 + 2;
		}
	}

public:
	bool Dragging;
	int* Value;

	int Min;
	int Max;

	std::string ValueString;
};
