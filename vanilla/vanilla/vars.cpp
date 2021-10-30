#include "vars.h"
#include "globals.h"

void CheatFonts::SetupFonts()
{
	D3DXCreateFontA(g_renderer->GetDevice(), (int)DYN_Y(28), 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_SWISS, "Helvetica", &title);
	D3DXCreateFontA(g_renderer->GetDevice(), (int)DYN_Y(18), 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_SWISS, "Helvetica", &tab);
	D3DXCreateFontA(g_renderer->GetDevice(), (int)DYN_Y(18), 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_SWISS, "Calibri", &text);
}

void CheatFonts::OnLostDevice()
{
	title->OnLostDevice();
	tab->OnLostDevice();
	text->OnLostDevice();
}

void CheatFonts::OnResetDevice()
{
	title->OnResetDevice();
	tab->OnResetDevice();
	text->OnResetDevice();
}
