#include "ComboBox.h"
#include "..\Colors.h"
#include "..\..\Globals.h"

#define ITEM_HEIGHT 20

void ComboBox::Draw()
{
	float ty = y;

	if (!Text.empty())
	{
		R::Text(F::Text, x, ty, CL_TEXT, Text.c_str());
		ty += 20;
	}

	R::Rect(x, ty, width, ITEM_HEIGHT, CL_OUTLINE);

	if (G::Menu.MouseOver(x, ty, x + width, ty + ITEM_HEIGHT))
	{
		R::FilledRect(x + 1, ty + 1, width - 2, ITEM_HEIGHT - 2, CL_LIGHT_BG);
	}

	size_t size = Items.size();

	if (size)
	{
		if(Index)
			R::Text(F::Text, x + 4, ty+1, CL_TEXT, Items.at(*Index).c_str());

		if (Opened)
		{
			float ty1  = ty + ITEM_HEIGHT + 2,
				  endh = size * ITEM_HEIGHT + 3;

			R::FilledRect(x, ty1, width, endh , CL_CONTROL);
			R::Rect(x, ty1, width, endh, CL_OUTLINE);

			for (size_t i = 0; i < size; ++i)
			{
				if (G::Menu.MouseOver(x, ty1, x + width, ty1 + ITEM_HEIGHT))
				{
					R::FilledRect(x + 1, ty1, width - 2, ITEM_HEIGHT, CL_LIGHT_BG);
				}

				R::Text(F::Text, x + 4, ty1, CL_TEXT, Items[i].c_str());
				ty1 += ITEM_HEIGHT;
			}
		}
	}
}

bool ComboBox::Think()
{
	if (G::Menu.LButton == 1)
	{
		float ty = y;

		if (!Text.empty())
			ty += 20;

		if (Opened)
		{
			if (Index)
			{
				ty += ITEM_HEIGHT + 2;
				size_t size = Items.size();

				if (G::Menu.MouseOver(x, ty, x + width, ty + ITEM_HEIGHT * size))
				{
					size_t idx = (size_t)((G::Menu.Mouse.y - ty) / ITEM_HEIGHT);

					if (idx < size)
						*Index = idx;
				}
			}

			Opened = false;
			parent->Release(this);

			return true;
		}
		else if (G::Menu.MouseOver(x, ty, x + width, ty + ITEM_HEIGHT))
		{
			parent->SetFocus(this);
			Opened = true;

			return true;
		}
	}

	return false;
}