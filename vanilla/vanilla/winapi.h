#pragma once
#include <Windows.h>

#define DECLARE_IMPORT_FN(import_name) extern decltype(import_name)* _##import_name;

DECLARE_IMPORT_FN(CreateThread);
DECLARE_IMPORT_FN(DisableThreadLibraryCalls);
DECLARE_IMPORT_FN(FreeLibraryAndExitThread);
DECLARE_IMPORT_FN(ExitThread);

DECLARE_IMPORT_FN(FindWindowA);
DECLARE_IMPORT_FN(GetModuleHandleA);
DECLARE_IMPORT_FN(GetModuleFileNameA);
DECLARE_IMPORT_FN(MessageBoxA);
DECLARE_IMPORT_FN(Sleep);

DECLARE_IMPORT_FN(SetWindowLongA);
DECLARE_IMPORT_FN(CallWindowProcA);

DECLARE_IMPORT_FN(GetDC);
DECLARE_IMPORT_FN(ReleaseDC);
DECLARE_IMPORT_FN(GetDeviceCaps);

DECLARE_IMPORT_FN(GetSystemMetrics);
DECLARE_IMPORT_FN(GetTickCount);
DECLARE_IMPORT_FN(ScreenToClient);

DECLARE_IMPORT_FN(GetAsyncKeyState);