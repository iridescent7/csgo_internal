#pragma once
#include "menu/window.h"

class CheatMenu
{
public:
	void Paint();
	void Scale();

	void Update(UINT msg, WPARAM wParam, LPARAM lParam);

	CheatMenu();

public:
	bool          open;
	unsigned char alpha;

private:
	Window _window;

	bool _dragging;
	bool _animating;
	bool _firstdrag;

	float _dragx;
	float _dragy;

	unsigned long _animTick;

	D3DCOLOR _colorpicker_cache[8] =
	{
		D3DCOLOR_XRGB(245, 245, 245),
		D3DCOLOR_XRGB(156, 39, 176),
		D3DCOLOR_XRGB(116, 29, 131),
		D3DCOLOR_XRGB(214, 98, 232),
		D3DCOLOR_XRGB(66, 66, 66),
		D3DCOLOR_XRGB(45, 137, 239),
		D3DCOLOR_XRGB(255, 0, 255),
		D3DCOLOR_XRGB(0, 255, 255)
	};
};
