#include "Translator.h"

void MiscTranslator::DispatchUserMessage(unsigned int msg_type, unsigned int pad1, unsigned int bytes, const void* msg_data)
{
	if (msg_type != CS_UM_SayText2) return;
	/*
	// note: check if entity index is local player but not for now since we're debugging offline
	msg_data_struct mdi((void*) msg_data, bytes);
	
	int         entidx  = mdi.get_ent_idx().Int32() - 1;
	std::string chatmsg = mdi.getAll_params().at(1).String();
	bool        allchat = mdi.get_allchat().Bool();
	*/
}

void MiscTranslator::ClientCmd(const char* szCmdString, bool allChat)
{
	// meme concept
	// if(!g_Vars.misc.translator.enabled) return;
}
