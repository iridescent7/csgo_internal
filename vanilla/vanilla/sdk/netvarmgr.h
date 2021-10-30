#pragma once
#include "../utils/cx_fnv1.h"
#include "../sdk/classes/RecvTable.h"
#include "../globals.h"

namespace netvarmgr
{
	DWORD GetOffsetRecursive(RecvTable* table, hashed_string propName, DWORD extra = NULL)
	{
		for (int i = 0; i < table->m_nProps; ++i)
		{
			auto* prop = &table->m_pProps[i];

			if (hash_compare(propName, prop->m_pVarName))
				return extra + prop->m_Offset;

			if (prop->m_RecvType == DPT_DataTable && prop->m_pDataTable)
			{
				auto offset = GetOffsetRecursive(prop->m_pDataTable, propName, extra + prop->m_Offset);

				if (offset)
					return offset;
			}
		}

		return NULL;
	}

	DWORD GetOffset(hashed_string tableName, hashed_string propName)
	{
		const auto* clientClass = g_client->GetAllClasses();

		while (clientClass)
		{
			auto* table = clientClass->m_pRecvTable;

			if (hash_compare(tableName, table->m_pNetTableName))
			{
				return GetOffsetRecursive(table, propName);
			}

			clientClass = clientClass->m_pNext;
		}

		return NULL;
	}

	RecvProp* GetNetvarPropRecursive(RecvTable* table, hashed_string propName)
	{
		for (int i = 0; i < table->m_nProps; ++i)
		{
			auto* prop = &table->m_pProps[i];

			if (hash_compare(propName, prop->m_pVarName))
				return prop;

			if (prop->m_RecvType == DPT_DataTable && prop->m_pDataTable)
			{
				auto prop_ptr = GetNetvarPropRecursive(prop->m_pDataTable, propName);

				if (prop_ptr)
					return prop_ptr;
			}
		}

		return nullptr;
	}

	RecvProp* GetNetvarProp(hashed_string tableName, hashed_string propName)
	{
		const auto* clientClass = g_client->GetAllClasses();

		while (clientClass)
		{
			auto* table = clientClass->m_pRecvTable;

			if (hash_compare(tableName, table->m_pNetTableName))
				GetNetvarPropRecursive(table, propName);

			clientClass = clientClass->m_pNext;
		}

		return nullptr;
	}
};
