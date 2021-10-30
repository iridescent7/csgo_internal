#include "Window.h"
#include "..\Colors.h"
#include "..\..\Globals.h"

void Window::Draw()
{
	// Draw window background
	D3DTLVERTEX background_grad_vertices[] =
	{
		TL2DVERT(x,	 y + height, D3DCOLOR_XRGB(138,62,104)),
		TL2DVERT(x,  y,	CL_BG_END),

		TL2DVERT(x + width, y + height, CL_BG_START),
		TL2DVERT(x + width, y, D3DCOLOR_XRGB(138,62,104))
	};

	R::Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, background_grad_vertices, sizeof(D3DTLVERTEX));
	R::Rect(x, y, width, height, CL_BG_OUTLINE);

	constexpr D3DCOLOR topbar_color = D3DCOLOR_ARGB(30, 0, 0, 0);

	D3DTLVERTEX topbar_vertices[] =
	{
		TL2DVERT(x,y,topbar_color),
		TL2DVERT(x+width,y,topbar_color),

		TL2DVERT(x, y + 45, topbar_color),
		TL2DVERT(x + width, y + 45, topbar_color),

		TL2DVERT(x, y + 45 + 8, 0),
		TL2DVERT(x + width, y + 45 + 8, 0)
	};

	R::Device->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 4, topbar_vertices, sizeof(D3DTLVERTEX));

	// 94px x 32px
	R::Text(F::Title, x + 15 + 2, y + 12 + 2, CL_CONTROL, Title.c_str());
	R::Text(F::Title, x + 15, y + 12, CL_TEXT, Title.c_str());

	float w;
	R::GetTextSize(F::Title, Title.c_str(), &w, NULL);

	// Draw tabs
	float tx = x + w + 40;

	for (size_t i = 0; i < Tabs.size(); ++i)
	{
		float w1;
		R::GetTextSize(F::TabText, Tabs[i].Title.c_str(), &w1, NULL);

		R::Text(F::TabText, tx, y + 17, (i == Index) ? CL_TEXT : CL_TEXT_DARK, Tabs[i].Title.c_str());

		if (i == Index)
			R::FilledRect(tx, y + 17 + 17, w1, 2, CL_CONTROL);

		tx += w1 + 20;
	}

	Tabs[Index].SetAlpha(alpha);
	Tabs[Index].Draw();
}

bool Window::Think()
{
	float w;
	R::GetTextSize(F::Title, Title.c_str(), &w, NULL);

	bool  think = false;
	float tx    = x + w + 40;

	bool found = false;

	if (G::Menu.LButton == 1 && G::Menu.MouseOver(tx, y, x + width, y+55))
	{
		size_t idx = 0;

		float  cur  = G::Menu.Mouse.x - tx;
		size_t size = Tabs.size();

		for (; idx < size && cur >= 0.f; ++idx, cur -= 20)
		{
			float w;
			R::GetTextSize(F::TabText, Tabs[idx].Title.c_str(), &w, NULL);

			cur -= w;
			if (cur <= 0.f)
			{
				found = true;
				break;
			}
		}

		if (found)
		{
			Index = idx;
			think = true;
		}
	}

	Tabs[Index].SetPosition(x + 16, y + 53 + 16);
	Tabs[Index].height = this->height - (53 + 16);

	if(!think || found)
		think = Tabs[Index].Think();

	if (Dragging)
	{
		if (G::Menu.LButton == 2)
		{
			this->x = G::Menu.Mouse.x - DragOffset.x;
			this->y = G::Menu.Mouse.y - DragOffset.y;
		}
		else
			Dragging = false;
	}
	else if (!think && G::Menu.LButton == 1 && G::Menu.MouseOver(x, y, x + width, y + height))
	{
		DragOffset.x = G::Menu.Mouse.x - this->x;
		DragOffset.y = G::Menu.Mouse.y - this->y;

		Dragging = true;
	}
	
	return true;
}
