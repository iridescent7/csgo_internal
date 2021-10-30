#include "combobox.h"
#include "..\globals.h"

void Combobox::Paint()
{
	_item_height = DYN_Y(20);
	_item_pad_x  = DYN_X(4);

	width  = DYN_X(100);
	height = DYN_Y(20) + _item_height;

	g_renderer->DrawOutlinedText(g_fonts.text, _text, x, y, MENU_COLOR(g_colors.text), MENU_COLOR(g_colors.outline));

	float ty = y + 20;
	g_renderer->DrawSoftFilledRect(x - 1, ty - 1, width + 2, _item_height + 2, MENU_COLOR(g_colors.outline));
	g_renderer->DrawSoftFilledRect(x,     ty,     width,     _item_height,     MENU_COLOR(g_colors.background));

	size_t size = _items.size();

	if (size)
	{
		if (_index)
			g_renderer->DrawOutlinedText(g_fonts.text, _items.at(*_index), x + _item_pad_x, ty + 1, MENU_COLOR(g_colors.text), MENU_COLOR(g_colors.outline));

		if (_opened)
		{
			float ty1  = ty + _item_height + 2,
				  endh = size * _item_height + 3;

			g_renderer->DrawSoftFilledRect(x, ty1, width, endh, CL_CONTROL);
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

bool Combobox::Update(UINT msg, WPARAM wParam, LPARAM lParam)
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
			Parent->Release(this);

			return true;
		}
		else if (G::Menu.MouseOver(x, ty, x + width, ty + ITEM_HEIGHT))
		{
			Parent->SetFocus(this);
			Opened = true;

			return true;
		}
	}

	return false;
}