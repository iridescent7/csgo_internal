#include "..\hooks.h"
#include "..\globals.h"

bool __fastcall hooks::Hooked_DispatchUserMessage(IBaseClientDLL* client, void* edx, UINT msg_type, UINT pad1, UINT bytes, const void* msg_data)
{
	//MiscTranslator.DispatchUserMessage(msg_type, pad1, bytes, msg_data); // Handle chat message display in chat box.
	return hooks::oDispatchUserMessage(client, msg_type, pad1, bytes, msg_data);
}