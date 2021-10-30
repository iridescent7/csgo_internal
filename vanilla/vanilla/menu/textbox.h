#pragma once
#include "control.h"
#include "..\globals.h"

class Textbox;
using tbEnterCallbackFn = void(*)(Textbox*);

class Textbox: public Control
{
public:
	virtual void Paint();
	virtual void Scale();

	virtual bool Update(UINT msg, WPARAM wParam, LPARAM lParam);

	Textbox(enc_string text, std::string* inputBuffer = nullptr, tbEnterCallbackFn enterCallback = nullptr)
	{
		if (!inputBuffer)
			 inputBuffer = &this->_fallbackInputBuffer;

		this->_text          = text;
		this->_enterCallback = enterCallback;
		this->inputBuffer    = inputBuffer;
	}

public:
	std::string* inputBuffer;
	
private:
	enc_string  _text;
	std::string _fallbackInputBuffer;
	std::string _outputBuffer;

	tbEnterCallbackFn _enterCallback;

	float _text_w,
		  _text_h,
		  _text_y,
		  _text_padding_x;

	float _char_w;

	float _tb_width = 0.f,
		  _tb_x     = 0.f;

	UINT _last_tick_delta;
	UINT _caret_pos_offset;

	UINT _max_char; // lazy fix

	bool _caret_display;
};