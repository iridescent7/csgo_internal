#pragma once

#include "Control.h"
#include <sstream>
#include <iomanip>

class SliderFloat : public Control
{
protected:
	virtual void Draw();
	virtual bool Think();

public:
	SliderFloat(const char* text, float* value = nullptr, float min = 0.f, float max = 1.f)
	{
		this->Min = min;
		this->Max = max;

		if (value)
		{
			if (*value < min)
				*value = min;
			else if (*value > max)
				*value = max;

			std::ostringstream oss;
			oss << std::fixed << std::setprecision(2) << *value;

			this->ValueString = oss.str();
		}

		this->Value    = value;
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
	bool   Dragging;
	float* Value;

	float Min;
	float Max;

	std::string ValueString;
};
