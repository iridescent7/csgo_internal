#include "Keybind.h"
#include "..\Colors.h"
#include "..\..\Globals.h"

#define KB_TALL 24

void KeyBind::Draw()
{
	float ty = y;

	if (!Text.empty())
	{
		R::Text(F::Text, x, ty, CL_TEXT, Text.c_str());
		ty += 20;
	}

	R::Rect(x, ty, width, KB_TALL, CL_OUTLINE);

	if (G::Menu.MouseOver(x, ty, x + width, ty + KB_TALL))
	{
		R::FilledRect(x + 1, ty + 1, width - 2, KB_TALL - 2, CL_LIGHT_BG);
	}

	const char* text;

	if (UpdateKey)
		text = "Press any key";
	else if (!vKey || *vKey == -1)
		text = "Not set";
	else
		text = KeyString.c_str();

	R::Text(F::Text, x + 4, ty+2, CL_TEXT, text);
}

bool KeyBind::Think()
{
	float ty = y;

	if (!Text.empty())
		ty += 20;

	if (!UpdateKey)
	{
		if (G::Menu.LButton == 1 && G::Menu.MouseOver(x, ty, x + width, ty + KB_TALL))
		{
			parent->SetFocus(this);
			UpdateKey = true;

			Held = true;
			return true;
		}

		return false;
	}

	if (Held)
	{
		if (G::Menu.LButton != 2)
			Held = false;
		else
			return true;
	}

	int curKey = (G::Menu.LButton == 1) ? VK_LBUTTON : G::Menu.CurrentKey;

	if (curKey != -1 && GetAsyncKeyState(curKey))
	{
		if (curKey == VK_ESCAPE)
			*vKey = -1;
		else
		{
			char buf[32] = { NULL };
			GetKeyNameTextA(curKey, buf, 32);

			*vKey = curKey;
			KeyString = buf;
		}

		UpdateKey = false;
		parent->Release(this);

		return true;
	}

	return false;
}
