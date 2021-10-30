#pragma once
#include "Control.h"

enum
{
	KBMODE_NONE,
	KBMODE_HOLD,
	KBMODE_TOGGLE
};

class KeyBind : public Control
{
public:
	virtual void Draw();
	virtual bool Think();

	KeyBind(const char* text, int* vKey = nullptr)
	{
		this->vKey = vKey;
		this->UpdateKey = false;

		this->width = 100;
		this->height = 24;

		if (text)
		{
			this->Text = text;
			this->height += 18 + 2;
		}
	}

public:
	int*		vKey;
	std::string KeyString;

	bool UpdateKey;
	bool Held;
};
