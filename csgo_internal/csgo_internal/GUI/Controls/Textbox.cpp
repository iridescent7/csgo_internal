#pragma once

#include "Textbox.h"
#include "..\Colors.h"
#include "..\..\Globals.h"

void TextBox::Draw()
{
	R::Rect(this->x, this->y, this->width, this->height, CL_OUTLINE); // Draw the box
	R::Text(F::Text, x + 4, height + 1, CL_TEXT, buffer); // Draw the text
}

bool TextBox::Think()
{
	if (G::Menu.LButton == 1 && G::Menu.MouseOver(x, y, x + width, y + height))
	{
		active = true;
	}

	active = false;

	return active;
}