#include "Checkbox.h"
#include "..\Colors.h"
#include "..\..\Globals.h"

void CheckBox::Draw()
{
	float ty = y + height / 2;

	R::FilledCircle(x + 6, ty, 6, 16, CL_WHITE);

	if (Checked && *Checked)
		R::FilledCircle(x + 6,ty, 5, 16, CL_CONTROL);

	R::Text(F::Text, x + 18, y-2, CL_TEXT, Text.c_str());
}

bool CheckBox::Think()
{
	float w;
	R::GetTextSize(F::Text, Text.c_str(), &w, NULL);

	if (G::Menu.LButton == 1 && G::Menu.MouseOver(x, y, x + 18 + w, y + height))
	{
		if(Checked)
			*Checked = !*Checked;

		return true;
	}

	return false;
}