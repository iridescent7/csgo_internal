#include "ConVar.h"
#include <stdio.h>
#include "..\Interfaces.h"

SpoofedConvar::SpoofedConvar()
{
}

SpoofedConvar::SpoofedConvar(const char* name)
{
	original_cvar = I::Cvar->FindVar(name);

	// todo: fix this causing us not to be able to join any server/map
	//if(original_cvar)
		//Spoof();
}

SpoofedConvar::SpoofedConvar(ConVar* pConvar)
{
	if (pConvar)
	{
		original_cvar = pConvar;
		Spoof();
	}
}

SpoofedConvar::~SpoofedConvar()
{
	if (dummy_cvar)
	{
		DWORD dwOld;

		SetFlags(original_flags);
		SetString(original_default_value);

		VirtualProtect((LPVOID)original_cvar->name, 128, PAGE_READWRITE, &dwOld);
		strcpy((char*)original_cvar->name, original_name);
		VirtualProtect((LPVOID)original_cvar->name, 128, dwOld, &dwOld);

		//Unregister dummy cvar
		I::Cvar->UnregisterConCommand(dummy_cvar);

		free(dummy_cvar);
		dummy_cvar = nullptr;
	}
}

void SpoofedConvar::Spoof()
{
	if (!dummy_cvar && original_cvar)
	{
		//Save old name value and flags so we can restore the cvar lates if needed
		original_flags = original_cvar->flags;

		strcpy(original_name, original_cvar->name);
		strcpy(original_default_value, original_cvar->default_value);

		CopyMemory(&original_values, &original_cvar->values, sizeof(ConVarValues));

		sprintf_s(dummy_name, 128, "ds_%s", original_name);

		//Create the dummy cvar
		dummy_cvar = (ConVar*)malloc(sizeof(ConVar));

		if (!dummy_cvar)
			return;

		memcpy(dummy_cvar, original_cvar, sizeof(ConVar));

		dummy_cvar->next = nullptr;

		//Register it
		I::Cvar->RegisterConCommand(dummy_cvar);

		//Fix "write access violation" bullshit
		DWORD oldProtect;
		VirtualProtect((LPVOID)original_cvar->name, 128, PAGE_READWRITE, &oldProtect);

		//Rename the cvar
		strcpy((char*)original_cvar->name, dummy_name);

		VirtualProtect((LPVOID)original_cvar->name, 128, oldProtect, &oldProtect);

		original_cvar->flags = FCVAR_NONE;
	}
}

void SpoofedConvar::SetFlags(int flags)
{
	if (dummy_cvar)
		original_cvar->flags = flags;
}

int SpoofedConvar::GetOriginalFlags()
{
	if (original_cvar)
		return original_flags;

	return FCVAR_NONE;
}

void SpoofedConvar::SetInt(int value)
{
	if (dummy_cvar)
		original_cvar->SetInt(value);
}

void SpoofedConvar::SetFloat(float value)
{
	if (dummy_cvar)
		original_cvar->SetFloat(value);
}

void SpoofedConvar::SetString(const char* value)
{
	if (dummy_cvar)
		original_cvar->SetValue(value);
}

bool SpoofedConvar::GetBool()
{
	if (original_cvar)
		return original_cvar->GetBool();

	return false;
}

int SpoofedConvar::GetInt()
{
	if (original_cvar)
		return original_cvar->GetInt();

	return 0;
}

float SpoofedConvar::GetFloat()
{
	if (original_cvar)
		return original_cvar->GetFloat();

	return 0.f;
}

const char* SpoofedConvar::GetString()
{
	if (original_cvar)
		return original_cvar->GetString();

	return "";
}

void SpoofedConvar::ResetInt()
{
	if (dummy_cvar)
		original_cvar->values.value_int = original_values.value_int;
}

void SpoofedConvar::ResetFloat()
{
	if (dummy_cvar)
		original_cvar->values.value_float = original_values.value_float;
}

void SpoofedConvar::ResetString()
{
	if (dummy_cvar)
	{
		original_cvar->values.value_string  = original_values.value_string;
		original_cvar->values.length_string = original_values.length_string;
	}
}
