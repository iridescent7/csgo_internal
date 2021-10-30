#pragma once

#include "Control.h"
#include <vector>

class Groupbox : public Control
{
public:
	virtual void Paint();
	virtual void Scale();

	virtual bool Update(UINT msg, WPARAM wParam, LPARAM lParam);

	Groupbox(enc_string text, const std::vector<Control*>& controls)
	{
		this->text = text;

		this->_controls = controls;

		this->_ctrl_pad_x = 0;
		this->_ctrl_pad_y = 0;
		this->_ctrl_indent_y = 0;
		this->_w = 0;
		this->_h = 0;
	}

public:
	enc_string text;

private:
	std::vector<Control*> _controls;

	float _ctrl_pad_x,
		  _ctrl_pad_y,
		  _ctrl_indent_y,
		  _w,
		  _h;
};
