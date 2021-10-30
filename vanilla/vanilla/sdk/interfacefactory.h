#pragma once

#include <cstdint>
#include <string>
#include <algorithm>
#include "..\utils\cx_fnv1.h"

#if !(!_DEBUG || NDEBUG)
#include <fstream>
#endif

typedef void* (*InstantiateInterfaceFn)();

class InterfaceReg
{
public:
	InstantiateInterfaceFn	     m_CreateFn;
	const char				    *m_pName;

	InterfaceReg		    	*m_pNext;
};

class InterfaceFactory
{
public:
	InterfaceFactory(InterfaceReg** interface_list) : interface_list(interface_list)
	{ }

	void* GetInterface(hashed_string interfaceName, size_t interfaceNameLength)
	{
		if (interface_list)
		{
			InterfaceReg* current = *interface_list;

			while (current)
			{
				std::string name(current->m_pName);

				if (name.length() > interfaceNameLength)
					name.erase(interfaceNameLength);

				if (hash_compare(interfaceName, name.c_str()))
				{
					return current->m_CreateFn();
				}

				current = current->m_pNext;
			}
		}

		return nullptr;
	}

#if !(!_DEBUG || NDEBUG)
	void DumpInterfaces(std::ofstream& ofs, const char* dllname)
	{
		ofs << dllname
			<< '\n';

		if (interface_list)
		{
			InterfaceReg* current = *interface_list;

			while (current)
			{
				ofs << "   "
					<< current->m_pName
					<< '\n';

				current = current->m_pNext;
			}
		}

		ofs << '\n';
	}
#endif
public:
	InterfaceReg** interface_list;
};
