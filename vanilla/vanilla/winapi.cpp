#include "winapi.h"
#include <string>
#include "utils/cx_fnv1.h"
#include "utils/cx_strenc.h"
#include "utils/environment.h"

void* _getImportFuncSafe(hashed_string importFuncName)
{
	auto       current_entry = env::ldr_data_entry();
	const auto first_entry   = current_entry;

	do
	{
		const env::exports_directory exports(current_entry->DllBase);

		if (exports)
		{
			auto export_index = exports.size();
			while (export_index--)
			{
				if (hash_compare(importFuncName, exports.name(export_index)))
					return (void*)(exports.address(export_index));
			}
		}

		current_entry = current_entry->load_order_next();

	} while (current_entry != first_entry);

	// meme
#ifdef _DEBUG
    throw (strenc("winapi: function hash not found ") + std::string(importFuncName)).c_str();
#elif
	throw (strenc("winapi: function hash not found ") + std::to_string(importFuncName)).c_str();
#endif
}

#define DEFINE_IMPORT_FN(import_name) \
	decltype(import_name)* _##import_name = (decltype(import_name)*) _getImportFuncSafe((hash(#import_name)));

DEFINE_IMPORT_FN(CreateThread);
DEFINE_IMPORT_FN(DisableThreadLibraryCalls);
DEFINE_IMPORT_FN(FreeLibraryAndExitThread);
DEFINE_IMPORT_FN(ExitThread);

DEFINE_IMPORT_FN(FindWindowA);
DEFINE_IMPORT_FN(GetModuleHandleA);
DEFINE_IMPORT_FN(GetModuleFileNameA);
DEFINE_IMPORT_FN(MessageBoxA);
DEFINE_IMPORT_FN(Sleep);

DEFINE_IMPORT_FN(SetWindowLongA);
DEFINE_IMPORT_FN(CallWindowProcA);

DEFINE_IMPORT_FN(GetDC);
DEFINE_IMPORT_FN(ReleaseDC);
DEFINE_IMPORT_FN(GetDeviceCaps);

DEFINE_IMPORT_FN(GetSystemMetrics);
DEFINE_IMPORT_FN(GetTickCount);
DEFINE_IMPORT_FN(ScreenToClient);

DEFINE_IMPORT_FN(GetAsyncKeyState);