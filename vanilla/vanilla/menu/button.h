#pragma once
#include ".\control.h"
#include "..\globals.h"

class Button;
using btnClickCallback = void(*)(Button*);

class Button: public Control
{
public:
	virtual void Paint();
	virtual void Scale();

	virtual bool Update(UINT msg, WPARAM wParam, LPARAM lParam);

	Button (enc_string text, btnClickCallback callback = nullptr)
	{
		this->text = text;

		this->_callback = callback;
		
		this->width  = 0;
		this->height = 0;

		this->_clicked = false;
		this->_hover   = false;

		this->_w = 0;
		this->_h = 0;
	}

public:
	enc_string text;

private:
	btnClickCallback _callback;

	bool _clicked,
		 _hover;

	float _w,
		  _h;
};