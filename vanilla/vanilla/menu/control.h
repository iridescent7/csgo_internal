#pragma once
#include <string>
#include <d3dx9.h>
#include <cstdint>
#include "..\utils\cx_strenc.h"
#include "..\utils\renderer.h"

#define MENU_COLOR(c) ( (D3DCOLOR)((g_menu.alpha << 24) | (c & 0x00FFFFFF)) )
#define TLV_AUTO(x, y, color) {(float)(x), (float)(y), 0.f, 1.f, color}

typedef unsigned int UINT;
typedef unsigned int WPARAM;
typedef long         LPARAM;

class BaseControl
{
public:
	__forceinline void SetPosition(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	__forceinline void SetSize(float width, float height)
	{
		this->width  = width;
		this->height = height;
	}

public:
	float x, y;
	float width, height;
};

class Control : public BaseControl
{
public:
	virtual void Paint() { };
	virtual void Scale() { };

	virtual bool Update(UINT msg, WPARAM wParam, LPARAM lParam) { return false; };

public:
	bool present = true;
};
