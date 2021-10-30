#pragma once
#include "control.h"

class Checkbox;
using cbChangedCallback = void(*)(Checkbox*);

class Checkbox : public Control
{
public:
	virtual void Paint();
	virtual void Scale();

	virtual bool Update(UINT msg, WPARAM wParam, LPARAM lParam);

	Checkbox(enc_string text, bool* checked = nullptr, cbChangedCallback callback = nullptr)
	{
		this->text = text;

		this->_checked  = checked;
		this->_callback = callback;

		this->width  = 0;
		this->height = 0;

		this->_text_pad_x = 0;
		this->_w          = 0;
		this->_h          = 0;
	}

public:
	enc_string text;

private:
	bool*             _checked;
	cbChangedCallback _callback;

	float _text_pad_x,
		  _w,
		  _h;
};
