#include "GroupBox.h"
#include "..\Colors.h"
#include "..\..\Globals.h"

void GroupBox::Draw()
{
	float w, h;
	R::GetTextSize(F::Text, Text.c_str(), &w, &h);

	float xwh = x + width / 2 - w / 2;

	R::FilledRect(x, y, width, height, CL_LIGHT_BG);
	R::Text(F::Text, xwh, y - h / 2, CL_TEXT, Text.c_str());

	D3DTLVERTEX outline_vertices[] =
	{
		TL2DVERT(xwh - 2,     y,  CL_OUTLINE),
		TL2DVERT(x,           y,  CL_OUTLINE),
		TL2DVERT(x,	          y + height, CL_OUTLINE),
		TL2DVERT(x + width,   y + height, CL_OUTLINE),
		TL2DVERT(x + width,   y,  CL_OUTLINE),
		TL2DVERT(xwh + w + 2, y,  CL_OUTLINE)
	};

	R::Device->DrawPrimitiveUP(D3DPT_LINESTRIP, 5, outline_vertices, sizeof(D3DTLVERTEX));
	
	for (auto& control : Controls)
	{
		if (control != Focus)
		{
			control->SetAlpha(alpha);
			control->Draw();
		}
	}

	if (Focus)
	{
		Focus->SetAlpha(alpha);
		Focus->Draw();
	}
}

bool GroupBox::Think()
{
	float cx = x + 12,
		  cy = y + 25;

	bool think = false;

	if (Focus)
	{
		for (auto& control : Controls)
		{
			control->SetPosition(x + 12, cy);
			control->width = this->width - 24;

			cy += control->height + 8;
		}

		think = Focus->Think();
	}
	else
	{
		for (auto& control : Controls)
		{
			control->SetPosition(x + 12, cy);
			control->width = this->width - 24;

			if (!think)
				think = control->Think();
			
			cy += control->height + 8;
		}
	}

	return think;
}