#pragma once
#include "Control.h"

class Button : public Control
{
public:
	virtual void Draw();
	virtual bool Think();

	Button(const char* text, void(*callback)() = nullptr)
	{
		this->Text     = text;
		this->CallBack = callback;
		this->Held     = false;

		this->width  = 100;
		this->height = 24;
	}

public:
	void(*CallBack)();
	bool Held;
};
