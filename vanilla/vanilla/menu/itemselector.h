#pragma once

#include "Control.h"
#include <vector>
#include <sstream>

class CItemSelector : public CControl
{
public:
	virtual void Paint();
	virtual bool Update();

	inline std::string GetHighlightString()
	{
		bool first_item = true;
		std::ostringstream oss;

		for (size_t i = 0; i < Items.size(); ++i)
		{
			if (this->SelectedItems[i])
			{
				if (first_item)
				{
					oss << Items[i];
					first_item = false;
				}
				else
					oss << ", " << Items[i];
			}
		}

		return oss.str();
	}

	CItemSelector(const char* text, const std::vector<std::string>& items, bool* selectedItems = nullptr)
	{
		this->Items			= items;
		this->SelectedItems = selectedItems;

		this->SelectedItemsString = GetHighlightString();

		this->width  = 100;
		this->height = 20;

		if (text)
		{
			this->Text = text;
			this->height += 18 + 2;
		}
	}

public:
	std::vector<std::string> Items;
	bool*            		 SelectedItems; // bool array

	bool					 Opened;
	std::string				 SelectedItemsString;
};
