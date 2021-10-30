#pragma once

#include <string>
#include <d3dx9.h>
#include <cstdint>
#include "hotkey.h"

class CheatFonts
{
public:
	void SetupFonts();
	void OnLostDevice();
	void OnResetDevice();

public:
	LPD3DXFONT title   = nullptr;
	LPD3DXFONT tab     = nullptr;
	LPD3DXFONT text    = nullptr;
};

class CheatColors
{
public:
	D3DCOLOR background        = D3DCOLOR_XRGB(245, 245, 245);
	D3DCOLOR foreground        = D3DCOLOR_XRGB(156, 39, 176);
	D3DCOLOR depth             = D3DCOLOR_XRGB(116, 29, 131);
	D3DCOLOR highlight         = D3DCOLOR_XRGB(214, 98, 232);
	D3DCOLOR outline           = D3DCOLOR_XRGB(66, 66, 66);
	D3DCOLOR outline_highlight = D3DCOLOR_XRGB(45, 137, 239);

	D3DCOLOR title = D3DCOLOR_XRGB(245, 245, 245);
	D3DCOLOR tab   = D3DCOLOR_XRGB(245, 245, 245);
	D3DCOLOR text  = D3DCOLOR_XRGB(245, 245, 245);
};

class CheatVariables
{
public:
	struct
	{
	public:
		bool  enabled = false;
		float fov     = 1.f;
	} aimbot;

	struct
	{
	public:
		std::string clantag;
	} misc;
};


class CheatHotkeys
{
public:
	hotkey_t aimbot;
};