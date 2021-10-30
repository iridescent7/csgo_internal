#pragma once

#include "Control.h"
#include <vector>

class Combobox : public Control
{
public:
	virtual void Paint();
	virtual bool Update(UINT msg, WPARAM wParam, LPARAM lParam);

	Combobox(encrypted_string text, const std::vector<std::string>& items, int* index = nullptr)
	{
		_text = text;

		_items = items;
		_index = index;

		_opened = false;

		width  = 0;
		height = 0;

		_item_height = 0;
		_item_pad_x  = 0;
	}

public:
	encrypted_string _text;
	bool             _opened;

	std::vector<encrypted_string> _items;
	int*                          _index;

	float _item_height,
		  _item_pad_x;
};
