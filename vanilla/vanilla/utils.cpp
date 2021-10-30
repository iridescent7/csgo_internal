#include "utils.h"
#include "globals.h"

namespace utils
{
#define INRANGE(x,a,b)  (x >= a && x <= b)
#define GET_BITS( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define GET_BYTE( x )    (GET_BITS(x[0]) << 4 | GET_BITS(x[1]))

	const win::LDR_DATA_TABLE_ENTRY_T* GetModuleEntry(hashed_string moduleName)
	{
		auto       current_entry = env::ldr_data_entry();
		const auto first_entry = current_entry;

		do
		{
			auto unicode_str = current_entry->BaseDllName;
			auto length = (unicode_str.Length / sizeof(wchar_t));

			std::string buffer(length, '\0');

			for (size_t i = 0; i != length; ++i)
				buffer[i] = *(char*)&unicode_str.Buffer[i];

			if (hash_compare(moduleName, buffer.c_str()))
			{
				return current_entry;
			}
			current_entry = current_entry->load_order_next();

		} while (current_entry != first_entry);

		return nullptr;
	}

	unsigned long ModulePatternScan(const win::LDR_DATA_TABLE_ENTRY_T* entry, const char* signature)
	{
		const char* sig = signature;

		const char* start = entry->DllBase;
		const char* end = start + entry->SizeOfImage;

		uintptr_t match = NULL;

		for (const char* pCur = start; pCur != end; ++pCur)
		{
			if (!*sig)
				return match;

			if (*(unsigned char*)sig == '\?' || *(unsigned char*)pCur == GET_BYTE(sig))
			{
				if (!match)
					match = (uintptr_t)pCur;

				if (!sig[2])
					return match;

				if (*(uintptr_t*)sig == '\?\?' || *(unsigned char*)sig != '\?')
					sig += 3;
				else
					sig += 2;
			}
			else
			{
				sig = signature;
				match = NULL;
			}
		}

		return NULL;
	}

	InterfaceFactory GetInterfaceFactory(const win::LDR_DATA_TABLE_ENTRY_T* entry)
	{
		const env::exports_directory exports(entry->DllBase);
		const char* CreateInterface = nullptr;

		if (exports)
		{
			auto export_index = exports.size();
			while (export_index--)
			{
				if (hash_compare(hash("CreateInterface"), exports.name(export_index)))
				{
					CreateInterface = exports.address(export_index);
					break;
				}
			}
		}

		if (!CreateInterface)
			throw charenc("error: module CreateInterface not found");

		try
		{
			const char* jmp_displacement_addr = CreateInterface + 0x5;
			const char* CreateInterfaceInternal = jmp_displacement_addr + sizeof(long) + *(long*)(jmp_displacement_addr);

			InterfaceReg** interface_list = *(InterfaceReg***)(CreateInterfaceInternal + 0x6);
			InterfaceReg*  interface_test = *interface_list;

			return InterfaceFactory(interface_list);
		}
		catch (...)
		{
			throw charenc("error: bad module interface list");
		}
	}
}