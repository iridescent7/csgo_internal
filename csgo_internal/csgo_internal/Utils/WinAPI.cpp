#pragma once
#include "WinAPI.h"

#define DEFINE_IMPORT_FUNCTION(library_name, import_name) \
	import_name##Fn _##import_name = /*import_name;*/(import_name##Fn)_GetProcAddressR(_GetModuleHandleWR(library_name), #import_name);

typedef struct _UNICODE_STRING
{
	USHORT Length;
	USHORT MaximumLength;
	PWSTR Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _LDR_MODULE
{
	LIST_ENTRY      InLoadOrderModuleList; // 8 - 0
	LIST_ENTRY      InMemoryOrderModuleList; // 8 - 8
	LIST_ENTRY      InInitializationOrderModuleList; // 8 - 16
	PVOID           BaseAddress; // 4 - 20
	PVOID           EntryPoint; // 4 - 24
	ULONG           SizeOfImage; // 4 - 28
	UNICODE_STRING  FullDllName; // 8 - 32
	UNICODE_STRING  BaseDllName; // 8 - 40
	ULONG           Flags; // 4
	SHORT           LoadCount; // 2
	SHORT           TlsIndex; // 2
	LIST_ENTRY      HashTableEntry; // 8
	ULONG           TimeDateStamp; // 4
}LDR_MODULE, *PLDR_MODULE;

HMODULE _GetModuleHandleWR(const wchar_t* szModule)
{
	LDR_MODULE* pModule = NULL;

	_asm
	{
		mov eax, fs:[0x18];    // TEB (Thread Environment Block)
		mov eax, [eax + 0x30]; // PEB (Process Environment Block)
		mov eax, [eax + 0x0C]; // pModule
		mov eax, [eax + 0x0C]; // pModule->InLoadOrderModuleList.Flink
		mov pModule, eax;
	}

	while (pModule->BaseAddress)
	{
		if (_wcsicmp(pModule->BaseDllName.Buffer, szModule) == 0)
		{
			return (HMODULE)pModule->BaseAddress;
		}
		pModule = (LDR_MODULE*)pModule->InLoadOrderModuleList.Flink; // grab the next module in the list
	}


	return NULL;
}

FARPROC WINAPI _GetProcAddressR(HANDLE hModule, LPCSTR lpProcName)
{
	UINT_PTR uiLibraryAddress = 0;
	FARPROC fpResult = NULL;

	if (hModule == NULL)
		return NULL;

	// a module handle is really its base address
	uiLibraryAddress = (UINT_PTR)hModule;

	__try
	{
		UINT_PTR uiAddressArray = 0;
		UINT_PTR uiNameArray = 0;
		UINT_PTR uiNameOrdinals = 0;
		PIMAGE_NT_HEADERS pNtHeaders = NULL;
		PIMAGE_DATA_DIRECTORY pDataDirectory = NULL;
		PIMAGE_EXPORT_DIRECTORY pExportDirectory = NULL;

		// get the VA of the modules NT Header
		pNtHeaders = (PIMAGE_NT_HEADERS)(uiLibraryAddress + ((PIMAGE_DOS_HEADER)uiLibraryAddress)->e_lfanew);

		pDataDirectory = (PIMAGE_DATA_DIRECTORY)&pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT];

		// get the VA of the export directory
		pExportDirectory = (PIMAGE_EXPORT_DIRECTORY)(uiLibraryAddress + pDataDirectory->VirtualAddress);

		// get the VA for the array of addresses
		uiAddressArray = (uiLibraryAddress + pExportDirectory->AddressOfFunctions);

		// get the VA for the array of name pointers
		uiNameArray = (uiLibraryAddress + pExportDirectory->AddressOfNames);

		// get the VA for the array of name ordinals
		uiNameOrdinals = (uiLibraryAddress + pExportDirectory->AddressOfNameOrdinals);

		// test if we are importing by name or by ordinal...
		if (((DWORD)lpProcName & 0xFFFF0000) == 0x00000000)
		{
			// import by ordinal...

			// use the import ordinal (- export ordinal base) as an index into the array of addresses
			uiAddressArray += ((IMAGE_ORDINAL((DWORD)lpProcName) - pExportDirectory->Base) * sizeof(DWORD));

			// resolve the address for this imported function
			fpResult = (FARPROC)(uiLibraryAddress + *(DWORD*)(uiAddressArray));
		}
		else
		{
			// import by name...
			DWORD dwCounter = pExportDirectory->NumberOfNames;
			while (dwCounter--)
			{
				char * cpExportedFunctionName = (char *)(uiLibraryAddress + *(DWORD*)(uiNameArray));

				// test if we have a match...
				if (strcmp(cpExportedFunctionName, lpProcName) == 0)
				{
					// use the functions name ordinal as an index into the array of name pointers
					uiAddressArray += (*(WORD*)(uiNameOrdinals) * sizeof(DWORD));

					// calculate the virtual address for the function
					fpResult = (FARPROC)(uiLibraryAddress + *(DWORD*)(uiAddressArray));

					// finish...
					break;
				}

				// get the next exported function name
				uiNameArray += sizeof(DWORD);

				// get the next exported function name ordinal
				uiNameOrdinals += sizeof(WORD);
			}
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		fpResult = NULL;
	}

	return fpResult;
}

DEFINE_IMPORT_FUNCTION(L"kernel32.dll", CreateThread)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", CloseHandle)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GetTickCount)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", K32GetModuleInformation)
//DEFINE_IMPORT_FUNCTION(L"User32.dll", GetKeyNameTextA)
//DEFINE_IMPORT_FUNCTION(L"User32.dll", ScreenToClient)
//DEFINE_IMPORT_FUNCTION(L"User32.dll", GetForegroundWindow)
//DEFINE_IMPORT_FUNCTION(L"User32.dll", MessageBoxA)
//DEFINE_IMPORT_FUNCTION(L"User32.dll", FindWindowA)
//DEFINE_IMPORT_FUNCTION(L"Winmm.dll", PlaySoundA)
//DEFINE_IMPORT_FUNCTION(L"User32.dll", mouse_event)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GetFileAttributesA)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GetThreadContext)
//DEFINE_IMPORT_FUNCTION(L"User32.dll", GetClientRect)
//DEFINE_IMPORT_FUNCTION(L"User32.dll", SetCursor)
//DEFINE_IMPORT_FUNCTION(L"Imm32.dll", ImmGetContext)
//DEFINE_IMPORT_FUNCTION(L"Imm32.dll", ImmSetCompositionWindow)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", WaitForSingleObject)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", MultiByteToWideChar)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GetTickCount64)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GetLastError)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", CreateEventA)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", VirtualQuery)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", FindFirstFileA)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", FindNextFileA)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GetFullPathNameA)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", FindClose)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", lstrcmpiA)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", Sleep)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GetProcAddress)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", IsBadCodePtr)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", DeleteFileA)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GlobalAlloc)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GetLocalTime)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GlobalLock)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", WideCharToMultiByte)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", CreateDirectoryA)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GlobalUnlock)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", AddVectoredExceptionHandler)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", Beep)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GetCurrentThread)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GetModuleHandleW)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", QueryPerformanceFrequency)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", QueryPerformanceCounter)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", TerminateProcess)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", SetEndOfFile)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", WriteConsoleW)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", HeapSize)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", CreateFileW)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", SetStdHandle)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", SetEnvironmentVariableW)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GetModuleFileNameA)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GetEnvironmentStringsW)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GetCommandLineW)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GetCommandLineA)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GetOEMCP)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GetACP)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", IsValidCodePage)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", FindNextFileW)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GetModuleHandleA)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", lstrlenA)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GetCurrentProcess)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", IsBadReadPtr)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GetProcessHeap)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", HeapAlloc)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", FindFirstFileExW)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", HeapReAlloc)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", SetFilePointerEx)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", FlushFileBuffers)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", EnumSystemLocalesW)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GetUserDefaultLCID)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", IsValidLocale)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GetFileType)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GetStdHandle)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", HeapFree)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GetConsoleCP)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", WriteFile)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", ReadConsoleW)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GetConsoleMode)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GetModuleFileNameW)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GetModuleHandleExW)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", ExitProcess)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", ReadFile)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", LoadLibraryExW)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", FreeLibrary)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", InterlockedFlushSList)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", RaiseException)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", RtlUnwind)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", InitializeSListHead)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GetCurrentThreadId)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", FreeEnvironmentStringsW)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", EnterCriticalSection)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", LeaveCriticalSection)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", DeleteCriticalSection)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", SetLastError)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", InitializeCriticalSectionAndSpinCount)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", CreateEventW)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", SwitchToThread)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", TlsAlloc)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", TlsGetValue)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", TlsSetValue)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", TlsFree)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GetSystemTimeAsFileTime)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", EncodePointer)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", DecodePointer)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GetStringTypeW)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", CompareStringW)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", LCMapStringW)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GetLocaleInfoW)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GetCPInfo)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", InitializeCriticalSection)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", SetEvent)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", ResetEvent)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", WaitForSingleObjectEx)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", UnhandledExceptionFilter)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", SetUnhandledExceptionFilter)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", IsProcessorFeaturePresent)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", IsDebuggerPresent)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GetStartupInfoW)
//DEFINE_IMPORT_FUNCTION(L"kernel32.dll", GetCurrentProcessId)
//DEFINE_IMPORT_FUNCTION(L"User32.dll", SetWindowLongA)
//DEFINE_IMPORT_FUNCTION(L"User32.dll", GetKeyState)
//DEFINE_IMPORT_FUNCTION(L"User32.dll", GetClipboardData)
//DEFINE_IMPORT_FUNCTION(L"User32.dll", OpenClipboard)
//DEFINE_IMPORT_FUNCTION(L"User32.dll", CloseClipboard)
//DEFINE_IMPORT_FUNCTION(L"User32.dll", GetAsyncKeyState)
//DEFINE_IMPORT_FUNCTION(L"User32.dll", GetCursorPos)
//DEFINE_IMPORT_FUNCTION(L"User32.dll", SetClipboardData)
//DEFINE_IMPORT_FUNCTION(L"User32.dll", EmptyClipboard)
//DEFINE_IMPORT_FUNCTION(L"User32.dll", CallWindowProcA)
//DEFINE_IMPORT_FUNCTION(L"d3dx9_43.dll", D3DXCreateTextureFromFileInMemoryEx)
//DEFINE_IMPORT_FUNCTION(L"d3dx9_43.dll", D3DXCreateTextureFromFileExA)
