#pragma once
#include <Windows.h>
#include <fstream>
#include "..\SDK\RecvData.h"
#include "..\Interfaces.h"

#undef GetProp

class CNetvars
{
public:
	RecvProp* GetProp(RecvTable* table, const char* name);

	DWORD GetOffsetRecursive(DWORD extra, RecvTable* table, const char* name)
	{
		if (auto prop = GetProp(table, name))
		{
			return extra + prop->m_Offset;
		}

		return NULL;
	}

	template < typename... Args >
	DWORD GetOffsetRecursive(DWORD extra, RecvTable* table, const char* name, Args... args)
	{
		if (auto prop = GetProp(table, name))
		{
			if (prop->m_RecvType == DPT_DataTable)
			{
				return GetOffsetRecursive(extra + prop->m_Offset, prop->m_pDataTable, args...);
			}
			else
			{
				return extra + prop->m_Offset;
			}
		}

		return NULL;
	}

	template < typename... Args >
	DWORD GetOffset(const char* name, Args... args)
	{
		const auto* clientClass = I::Client->GetAllClasses();

		while (clientClass != nullptr)
		{
			auto* table = clientClass->m_pRecvTable;

			if (strcmp(table->m_pNetTableName, name) == 0)
			{
				return GetOffsetRecursive(NULL, table, args...);
			}

			clientClass = clientClass->m_pNext;
		}

		return NULL;
	}

	void Dump();
	void DumpRecursive(std::ofstream& ofs, RecvTable* table, int indent = 1);
};
