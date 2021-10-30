#include "Button.h"
#include "..\Colors.h"
#include "..\..\Globals.h"

void Button::Draw()
{
	R::Rect(x, y, width, height, CL_OUTLINE);

	if (!Held && G::Menu.MouseOver(x, y, x + width, y + height))
	{
		R::FilledRect(x + 1, y + 1, width - 2, height - 2, CL_LIGHT_BG);
	}

	float w;
	R::GetTextSize(F::Text, Text.c_str(), &w, NULL);

	R::Text(F::Text, x + width / 2 - w / 2, y + height / 2 - 9, CL_TEXT, Text.c_str());
}

bool Button::Think()
{
	if (G::Menu.LButton == 1 && G::Menu.MouseOver(x, y, x + width, y + height))
	{
		Held = true;
		return true;
	}

	if (Held && G::Menu.LButton != 2)
	{
		if (CallBack)
			CallBack();

		Held = false;
		return true;
	}

	return Held;
}