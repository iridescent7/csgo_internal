#include "ItemSelector.h"
#include "..\Colors.h"
#include "..\..\Globals.h"

#define ITEM_HEIGHT 20

void ItemSelector::Draw()
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
		R::Text(F::Text, x + 4, ty + 1, CL_TEXT, SelectedItemsString.c_str());

		if (Opened)
		{
			float endh = size * ITEM_HEIGHT + 3;

			R::FilledRect(x, ty + ITEM_HEIGHT, width, endh, CL_CONTROL);
			R::Rect(x, ty + ITEM_HEIGHT, width, endh, CL_OUTLINE);

			float my = ty + ITEM_HEIGHT + 2;

			for (size_t i = 0; i < size; ++i)
			{
				if (G::Menu.MouseOver(x, my, x + width, my + ITEM_HEIGHT))
				{
					R::FilledRect(x + 1, my, width - 2, ITEM_HEIGHT, CL_LIGHT_BG);
				}

				R::Text(F::Text, x + 4, my, SelectedItems[i] ? CL_BG_END : CL_TEXT, Items[i].c_str());

				my += ITEM_HEIGHT;
			}
		}
	}
}

bool ItemSelector::Think()
{
	if (G::Menu.LButton == 1)
	{
		float ty = y;

		if (!Text.empty())
			ty += 20;

		if (Opened)
		{
			ty += ITEM_HEIGHT + 2;
			size_t size = Items.size();

			if (SelectedItems && G::Menu.MouseOver(x, ty, x + width, ty + ITEM_HEIGHT * size))
			{
				size_t i = (size_t)((G::Menu.Mouse.y - ty) / ITEM_HEIGHT);

				if (i < size)
				{
					SelectedItems[i]    = !SelectedItems[i];
					SelectedItemsString = GetHighlightString();
				}
			}
			else
			{
				Opened = false;
				parent->Release(this);
			}

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