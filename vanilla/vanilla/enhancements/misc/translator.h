#pragma once

#include <Windows.h>
#include "..\..\sdk.h"
#include "..\..\utils\protobufparser.h"

class MiscTranslator
{
public:
	void DispatchUserMessage(unsigned int msg_type, unsigned int pad1, unsigned int bytes, const void* msg_data);
	void ClientCmd(const char* szCmdString, bool allChat);

	private:
		// msg_data info struct specifically for CS_UM_SayText2 message type.
		// https://www.unknowncheats.me/forum/counterstrike-global-offensive/293454-protobuff-parser-writer-meme.html
		make_struct(msg_data_struct, 5)
		  make_field(ent_idx , 1, TYPE_INT32)    // sender's entity index (entity index - 1)
		  make_field(chat    , 2, TYPE_BOOL)
		  make_field(msg_name, 3, TYPE_STRING)
		  make_field(params  , 4, TYPE_STRING)   // Message is stored in the second field as a string
		  make_field(allchat , 5, TYPE_BOOL)     // boolean value that indicates if the message is either all (true) or team (false)
		};
};