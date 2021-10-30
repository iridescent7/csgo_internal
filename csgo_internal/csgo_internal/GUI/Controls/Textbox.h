#pragma once

#include "Control.h"

class TextBox : public Control
{
	public:
		virtual void Draw();
		virtual bool Think();

		// Constructor
		TextBox(const float _width, char *_buffer)
		{
			if (!_buffer) return;
			this->width = _width;
			this->height = 20;
			*buffer = (*_buffer);
			this->Text = *buffer;
			maxBuffer = sizeof(*buffer);

			caret_pos = maxBuffer + 1;
			caret_show = false;
			active = false;
		}

	public:
		bool           caret_show, // Tracks whether it should show the caret at the given tick (blinking caret thing)
					   active;     // Track if the textbox is active

		short unsigned caret_pos;  // Tracks the current caret position

		int   unsigned maxBuffer;  // Stores our maximum buffer size
		char           *buffer;    // Pointer to our char buffer that will store our input
};