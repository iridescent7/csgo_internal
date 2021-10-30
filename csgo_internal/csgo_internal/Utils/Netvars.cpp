#include "Netvars.h"
#include "..\SDK\SDK.h"
#include "..\Globals.h"

RecvProp* CNetvars::GetProp(RecvTable* table, const char* name)
{
	for (int i = 0; i < table->m_nProps; ++i)
	{
		auto prop = &table->m_pProps[i];

		if (strcmp(prop->m_pVarName, name) == 0)
		{
			return prop;
		}
	}

	return nullptr;
}

void CNetvars::Dump()
{
	char path[MAX_PATH];
	GetModuleFileNameA(G::Dll, path, MAX_PATH);

	const char* filename = "csgo_netvar_dump.txt";
	size_t length_to_copy = strlen(filename) + 1; // + 1 to include the null terminator aswell

	size_t offset = std::string(path).rfind("\\");

	while (offset + length_to_copy >= MAX_PATH)
	{
		offset = std::string(path).rfind("\\", offset-1);
	}

	CopyMemory((char*)(path + offset + 1), filename, length_to_copy);

	std::ofstream ofs(path);

	const auto* clientClass = I::Client->GetAllClasses();
	while (clientClass != nullptr)
	{
		auto* table = clientClass->m_pRecvTable;

		ofs << table->m_pNetTableName
		    << " -> "
		    << std::endl;

		DumpRecursive(ofs, table);

		clientClass = clientClass->m_pNext;
	}
}

void CNetvars::DumpRecursive(std::ofstream& ofs, RecvTable* table, int indent)
{
	std::string spaces(indent * 4, ' ');

	for (int i = 0; i < table->m_nProps; ++i)
	{
		const auto* prop = &table->m_pProps[i];

		if (isdigit(prop->m_pVarName[0])) // skip memes
			continue;

		if (strcmp(prop->m_pVarName, "baseclass") == 0) // skip known datatables
			continue;

		ofs << spaces.c_str() // meme wtf
			<< prop->m_pVarName
			<< " -> "
			<< prop->m_Offset
			<< std::endl;

		if (prop->m_RecvType == DPT_DataTable)
		{
			DumpRecursive(ofs, prop->m_pDataTable, indent + 1);
		}
	}
}