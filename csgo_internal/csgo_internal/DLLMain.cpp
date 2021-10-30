#include "SDK\SDK.h"

#include "Globals.h"
#include "Hooks\Hooks.h"

#include <cstdlib>
#include <ctime>

void SetupThread()
{
	srand((unsigned)time(0));

	while ( !( G::Window = FindWindowA("Valve001", NULL) ) )
		Sleep(100);

	while ( !GetModuleHandleA("serverbrowser.dll") )
		Sleep(100);

	Sleep(100);

	I::SetupInterfaces();
	G::SetupVars();
	O::SetupOffsets();
	C::SetupCheat();
	H::SetupHooks();
}

BOOL WINAPI DllMain(HMODULE hDll, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		G::Dll = hDll;

		DisableThreadLibraryCalls(hDll);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)SetupThread, NULL, NULL, NULL);
	}

	return true;
}