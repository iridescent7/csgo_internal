#include "..\hooks.h"
#include "..\globals.h"

void __fastcall hooks::Hooked_PaintTraverse(IPanel* panel, void* edx, unsigned int vguiPanel, bool forceRepaint, bool allowForce)
{
	/*if (!strcmp(g_Panel->GetName(vguiPanel), "MatSystemTopPanel"))
	{

	}*/

	return hooks::oPaintTraverse(panel, vguiPanel, forceRepaint, allowForce);
}
