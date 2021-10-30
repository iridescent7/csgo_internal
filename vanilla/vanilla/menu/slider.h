#pragma once
#include "control.h"

class Slider : public Control
{
public:
	virtual void Paint();
	virtual void Scale();

	virtual bool Update(UINT msg, WPARAM wParam, LPARAM lParam);

	Slider(enc_string text, int* value, int min, int max)
	{
		this->text = text;

		this->_value = value;
		this->_min   = min;
		this->_max   = max;

		this->_clicked = false;

		this->_bar_pad_y  = 0;
		this->_bar_height = 0;
		this->_w = 0;
		this->_h = 0;
	}

public:
	enc_string text;

private:
	int* _value;

	int _min,
		_max;

	bool _clicked;

	float _bar_pad_y,
		  _bar_height,
		  _w,
		  _h;
};