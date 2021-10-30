#pragma once
#include <string>
#include <cstdint>
#include "sdk\interfacefactory.h"
#include "utils\cx_fnv1.h"
#include "utils\cx_pcg32.h"
#include "utils\cx_strenc.h"
#include "utils\environment.h"
#include "utils\renderer.h"

namespace utils
{
	template <typename T>
	void Clamp(T* val, const T min, const T max)
	{
		if (*val < min)
			*val = min;
		else if (*val > max)
			*val = max;
	}

	const win::LDR_DATA_TABLE_ENTRY_T* GetModuleEntry(hashed_string moduleName);

	unsigned long    ModulePatternScan(const win::LDR_DATA_TABLE_ENTRY_T* entry, const char* signature);
	InterfaceFactory GetInterfaceFactory(const win::LDR_DATA_TABLE_ENTRY_T* entry);
}