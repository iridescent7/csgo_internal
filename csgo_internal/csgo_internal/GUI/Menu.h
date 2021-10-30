#pragma once
#include <Windows.h>
#include <string>
#include <thread>

#include "Controls.h"

class CMenu
{
public:
	void EndScene();
	void PollInput();

	void WndProcCallbk(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool MouseOver(int x, int y, int x1, int y1);

	void SaveConfig(std::string filename);
	void LoadConfig(std::string filename);

	CMenu();

public:
	bool Enabled;
	bool bAnimFinished;

	POINT Mouse;

	byte LButton;
	byte CurrentKey;

	DWORD last_update_tick;

	byte alpha;
	Window window;

};
