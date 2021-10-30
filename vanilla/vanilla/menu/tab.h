#pragma once
#include "control.h"
#include <vector>

class Tab : public BaseControl//, public ControlParent
{
public:
	void Paint();
	void Scale();

	bool Update(UINT msg, WPARAM wParam, LPARAM lParam);

	Tab(enc_string title, const std::vector<Control*> controls = { })
	{
		this->title     = title;
		this->_controls = controls;

		this->width  = 0;
		this->height = 0;

		this->_missed_mmove        = false;
		this->_missed_mmove_lparam = 0;

		this->_ctrl_indent_y = 0;
	}

public:
	enc_string title;

private:
	std::vector<Control*> _controls;

	bool   _missed_mmove;
	LPARAM _missed_mmove_lparam;

	float _ctrl_indent_y;
};
