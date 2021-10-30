#include "Slider.h"
#include "..\Colors.h"
#include "..\..\Globals.h"

#define SL_TALL 17

void Slider::Draw()
{
	float ty = y;

	if (!Text.empty())
	{
		R::Text(F::Text, x, ty, CL_TEXT, Text.c_str());
		ty += 20;
	}

	float cy = ty + 0.3f * SL_TALL;

	R::Rect(x, cy - 3, width, 4, CL_DARK_OUTLINE);

	int value = Value ? *Value : 0;

	float ratio = (float)(value - Min) / (float)(Max - Min);
	float bw = ratio * width;

	if (bw < 0)
		bw = 0;

	R::FilledRect(x+bw, cy - 3, width-bw, 5, CL_WHITE);
	R::FilledRect(x, cy - 3, bw, 5, CL_CONTROL);

	float radius = 0.3f * SL_TALL;

	R::FilledCircle(x + bw, cy, radius, 24, CL_WHITE);

	float w, h;
	R::GetTextSize(F::SliderText, ValueString.c_str(), &w, &h);

	R::Text(F::SliderText, x + bw - w / 2, cy+4, CL_TEXT, ValueString.c_str());
}

bool Slider::Think()
{
	float r = 0.33f * SL_TALL;

	if (!Dragging)
	{
		float ty = y;

		if (!Text.empty())
			ty += 20;

		if (G::Menu.LButton == 1 && G::Menu.MouseOver(x-r, ty, x + width+r, ty + SL_TALL))
		{
			Dragging = true;
		}

		return Dragging;
	}

	if (G::Menu.LButton != 2)
	{
		Dragging = false;
	}
	else if (Value)
	{
		float nx = G::Menu.Mouse.x - this->x;

		if (nx < 0)
			nx = 0;

		if (nx > width)
			nx = width;

		float ratio = nx / (float)width;
		*Value = Min + (int)((Max - Min) * ratio + 0.5f);

		ValueString = std::to_string(*Value);
	}

	return Dragging;
}