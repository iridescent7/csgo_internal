#pragma once
#include "control.h"

template <typename T = int>
class Slider2 : public Control
{
public:
	virtual void Paint();
	virtual void Scale();

	virtual bool Update(UINT msg, WPARAM wParam, LPARAM lParam);

	Slider2(enc_string text, T* lvalue, T* rvalue, T min, T max)
	{
		this->text = text;
		
		this->_lvalue = lvalue;
		this->_rvalue = rvalue;

		this->_min = min;
		this->_max = max;
	}

public:
	enc_string text;

private:
	T* _lvalue,
	   _rvalue;

	T _min,
	  _max;

	float _bar_width_offset,
		  _bar_height,
		  _bar_x,
		  _bar_y;

	float _text_w = -1,
		  _text_y;
};