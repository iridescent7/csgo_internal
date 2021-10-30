#include "Tab.h"
#include "..\Colors.h"
#include "..\..\Globals.h"

void Tab::Draw()
{
	for (auto& control : Controls)
	{
		if (control != Focus)
		{
			control->SetAlpha(alpha);
			control->Draw();
		}
	}

	// unnecessary, for now
	if (Focus)
	{
		Focus->SetAlpha(alpha);
		Focus->Draw();
	}
}

bool Tab::Think()
{
	int  cx = x, cy = y;
	bool think = false;

	if (Focus)
	{
		for (auto& control : Controls)
		{
			control->SetPosition(cx, cy);

			if (cy + control->height > this->height)
			{
				cx += control->width + 8;
				cy = y;
			}
			else
				cy += control->height + 8;
		}

		think = Focus->Think();
	}
	else
	{
		for (auto& control : Controls)
		{
			control->SetPosition(cx, cy);

			if (!think)
				think = control->Think();

			if (cy + control->height > this->height)
			{
				cx += control->width + 8;
				cy = y;
			}
			else
				cy += control->height + 8;
		}
	}

	return think;
}