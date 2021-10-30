#pragma once
#include "Interfaces\getvfunc.h"
#include "Color.h"

// The default, no flags at all
#define FCVAR_NONE				0

	// Command to ConVars and ConCommands
	// ConVar Systems
#define FCVAR_UNREGISTERED		(1<<0)	// If this is set, don't add to linked list, etc.
#define FCVAR_DEVELOPMENTONLY	(1<<1)	// Hidden in released products. Flag is removed automatically if ALLOW_DEVELOPMENT_CVARS is defined.
#define FCVAR_GAMEDLL			(1<<2)	// defined by the game DLL
#define FCVAR_CLIENTDLL			(1<<3)  // defined by the client DLL
#define FCVAR_HIDDEN			(1<<4)	// Hidden. Doesn't appear in find or autocomplete. Like DEVELOPMENTONLY, but can't be compiled out.

	// ConVar only
#define FCVAR_PROTECTED			(1<<5)  // It's a server cvar, but we don't send the data since it's a password, etc.  Sends 1 if it's not bland/zero, 0 otherwise as value
#define FCVAR_SPONLY			(1<<6)  // This cvar cannot be changed by clients connected to a multiplayer server.
#define	FCVAR_ARCHIVE			(1<<7)	// set to cause it to be saved to vars.rc
#define	FCVAR_NOTIFY			(1<<8)	// notifies players when changed
#define	FCVAR_USERINFO			(1<<9)	// changes the client's info string
#define FCVAR_CHEAT				(1<<14) // Only useable in singleplayer / debug / multiplayer & sv_cheats

#define FCVAR_PRINTABLEONLY		(1<<10)  // This cvar's string cannot contain unprintable characters ( e.g., used for player name etc ).
#define FCVAR_UNLOGGED			(1<<11)  // If this is a FCVAR_SERVER, don't log changes to the log file / console if we are creating a log
#define FCVAR_NEVER_AS_STRING	(1<<12)  // never try to print that cvar

	// It's a ConVar that's shared between the client and the server.
	// At signon, the values of all such ConVars are sent from the server to the client (skipped for local
	//  client, of course )
	// If a change is requested it must come from the console (i.e., no remote client changes)
	// If a value is changed while a server is active, it's replicated to all connected clients
#define FCVAR_REPLICATED		(1<<13)	// server setting enforced on clients, TODO rename to FCAR_SERVER at some time
#define FCVAR_DEMO				(1<<16)  // record this cvar when starting a demo file
#define FCVAR_DONTRECORD		(1<<17)  // don't record these command in demofiles
#define FCVAR_RELOAD_MATERIALS	(1<<20)	// If this cvar changes, it forces a material reload
#define FCVAR_RELOAD_TEXTURES	(1<<21)	// If this cvar changes, if forces a texture reload

#define FCVAR_NOT_CONNECTED		(1<<22)	// cvar cannot be changed by a client that is connected to a server
#define FCVAR_MATERIAL_SYSTEM_THREAD (1<<23)	// Indicates this cvar is read from the material system thread
#define FCVAR_ARCHIVE_XBOX		(1<<24) // cvar written to config.cfg on the Xbox

#define FCVAR_ACCESSIBLE_FROM_THREADS	(1<<25)	// used as a debugging tool necessary to check material system thread convars

#define FCVAR_SERVER_CAN_EXECUTE	(1<<28)// the server is allowed to execute this command on clients via ClientCommand/NET_StringCmd/CBaseClientState::ProcessStringCmd.
#define FCVAR_SERVER_CANNOT_QUERY	(1<<29)// If this is set, then the server is not allowed to query this cvar's value (via IServerPluginHelpers::StartQueryCvarValue).
#define FCVAR_CLIENTCMD_CAN_EXECUTE	(1<<30)	// IVEngineClient::ClientCmd is allowed to execute this command.

class ConVarValues
{
public:
	char* 	    value_string;     // 0x24
	__int32		length_string;    // 0x28
	float		value_float;      // 0x2C
	__int32	    value_int;		  // 0x30
};

class ConVar
{
public:
	const char* GetString()
	{
		using Fn = const char*(__thiscall*)(ConVar*);
		return getvfunc<Fn>(this, 11)(this);
	}

	float GetFloat()
	{
		using Fn = float(__thiscall*)(ConVar*);
		return getvfunc<Fn>(this, 12)(this);
	}

	int GetInt()
	{
		using Fn = __int32(__thiscall*)(ConVar*);
		return getvfunc<Fn>(this, 13)(this);
	}

	inline bool GetBool()
	{
		return !!GetInt();
	}

	void SetValue(const char* value)
	{
		using Fn = void(__thiscall*)(ConVar*, const char*);
		return getvfunc<Fn>(this, 14)(this, value);
	}

	void SetFloat(const float value)
	{
		using Fn = void(__thiscall*)(ConVar*, float);
		return getvfunc<Fn>(this, 15)(this, value);
	}

	void SetInt(const int value)
	{
		using Fn = void(__thiscall*)(ConVar*, __int32);
		return getvfunc<Fn>(this, 16)(this, value);
	}

public:
	void**                 vtable;           // 0x00
	ConVar*                next;             // 0x04
	bool                   registered;       // 0x08
	const char*            name;             // 0x0C
	const char*            helper_string;    // 0x10
	__int32                flags;            // 0x14
	void*                  callback;         // 0x18
	ConVar*                parent;           // 0x1C
	const char*            default_value;    // 0x20
	ConVarValues           values;           // 0x24
	bool                   has_min_value;    // 0x34
	float                  min_value;        // 0x38
	bool                   has_max_value;    // 0x3C
	float                  max_value;        // 0x40
	void*                  change_callbacks; // 0x44
};

class SpoofedConvar
{
public:
	SpoofedConvar();
	SpoofedConvar(const char* szCVar);
	SpoofedConvar(ConVar* pCVar);

	~SpoofedConvar();

	void Spoof();

	void SetFlags(int flags);

	void SetInt(int value);
	void SetFloat(float value);
	void SetString(const char* value);

	int GetOriginalFlags();

	bool  GetBool();
	int   GetInt();
	float GetFloat();
	const char* GetString();

	void ResetInt();
	void ResetFloat();
	void ResetString();

private:
	ConVar* original_cvar = nullptr;
	ConVar* dummy_cvar = nullptr;

	int			 original_flags;
	ConVarValues original_values;

	char dummy_name[128];
	char dummy_default_value[128];

	char original_name[128];
	char original_default_value[128];
};

class IAppSystem
{
public:
	virtual void func0() = 0;
	virtual void func1() = 0;
	virtual void func2() = 0;
	virtual void func3() = 0;
	virtual void func4() = 0;
	virtual void func5() = 0;
	virtual void func6() = 0;
	virtual void func7() = 0;
	virtual void func8() = 0;
	virtual void func9() = 0;
};

struct CVarDLLIdentifier_t;

class ICVar : public IAppSystem
{
public:
	virtual void			func10() = 0;
	virtual void			RegisterConCommand(ConVar* pConvar) = 0;
	virtual void			UnregisterConCommand(ConVar* pConvar) = 0;
	virtual void			func13() = 0;
	virtual ConVar*			FindVar(const char* name) = 0;
	virtual void			func15() = 0;
	virtual void			func16() = 0;
	virtual void			func17() = 0;
	virtual void			func18() = 0;
	virtual void			func19() = 0;
	virtual void			func20() = 0;

	template <typename ...Args>
	inline void ConsoleColorPrintf(const Color& color, const char* format, Args... args)
	{
		using Fn = void(void*, const Color&, const char*, ...);
		getvfunc<Fn>(this, 25)(this, color, format, args...);
	}
};
