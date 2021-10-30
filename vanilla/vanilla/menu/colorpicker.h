#pragma once

#include "control.h"
#include "..\globals.h"

class Colorpicker: public Control
{
public:
	virtual void Paint();
	virtual void Scale();
	virtual bool Update(UINT msg, WPARAM wParam, LPARAM lParam);

	Colorpicker(enc_string text, D3DCOLOR* pColor = nullptr, D3DCOLOR* pCache = nullptr)
	{
		if (!pColor)
			pColor = &_colorFallback;

		this->_text   = text;
		this->_pColor = pColor;
		this->_pCache = pCache;
	}

private:
	enc_string _text;

	D3DCOLOR _colorFallback;

	D3DCOLOR* _pColor;
	D3DCOLOR* _pCache;
};