#include "control.h"
#include "..\hotkey.h"

class Keybind : public Control
{
public:
	virtual void Paint();
	virtual void Scale();

	virtual bool Update(UINT msg, WPARAM wParam, LPARAM lParam);

	Keybind(enc_string text, hotkey_t* key = nullptr)
	{
		this->text = text;
		this->_key = key;

		this->width  = 0;
		this->height = 0;

		this->_wait_key  = false;
		this->_wait_mode = false;

		this->_box_width       = 0;
		this->_mode_text_pad_x = 0;
	}

public:
	enc_string text;

private:
	hotkey_t*         _key;

	bool _wait_key,
		 _wait_mode;

	float _box_width,
		  _mode_text_pad_x;
};