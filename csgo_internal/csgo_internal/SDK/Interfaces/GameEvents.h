#pragma once

class bf_read;
class bf_write;

class IGameEvent
{
public:

	virtual ~IGameEvent() = 0;
	virtual const char *GetName() const = 0;

	virtual bool  IsReliable() const = 0;
	virtual bool  IsLocal() const = 0;
	virtual bool  IsEmpty(const char *keyName = 0) = 0;

	virtual bool  GetBool(const char *keyName = 0, bool defaultValue = false) = 0;
	virtual int   GetInt(const char *keyName = 0, int defaultValue = 0) = 0;
	virtual unsigned long GetUint64(const char *keyName = 0, unsigned long defaultValue = 0) = 0;
	virtual float GetFloat(const char *keyName = 0, float defaultValue = 0.0f) = 0;
	virtual const char *GetString(const char *keyName = 0, const char *defaultValue = "") = 0;
	virtual const wchar_t *GetWString(const char *keyName, const wchar_t *defaultValue = L"") = 0;

	virtual void SetBool(const char *keyName, bool value) = 0;
	virtual void SetInt(const char *keyName, int value) = 0;
	virtual void SetUint64(const char *keyName, unsigned long value) = 0;
	virtual void SetFloat(const char *keyName, float value) = 0;
	virtual void SetString(const char *keyName, const char *value) = 0;
	virtual void SetWString(const char *keyName, const wchar_t *value) = 0;
};

class IGameEventListener
{
public:
	virtual ~IGameEventListener() {}
	virtual void FireGameEvent(IGameEvent* Event) = 0;
	virtual int GetEventDebugID() { return 42; }
};

class IGameEventManager
{
public:
	virtual int __Unknown_1(int* dwUnknown) = 0;

	// load game event descriptions from a file eg "resource\gameevents.res"
	virtual int LoadEventsFromFile(const char *filename) = 0;

	// removes all and anything
	virtual void Reset() = 0;

	// adds a listener for a particular event
	virtual bool AddListener(IGameEventListener *listener, const char *name, bool bServerSide) = 0;

	// returns true if this listener is listens to given event
	virtual bool FindListener(IGameEventListener *listener, const char *name) = 0;

	// removes a listener
	virtual int RemoveListener(IGameEventListener *listener) = 0;

	// create an event by name, but doesn't fire it. returns NULL is event is not
	// known or no listener is registered for it. bForce forces the creation even if no listener is active
	virtual IGameEvent *CreateEvent(const char *name, bool bForce, unsigned int dwUnknown) = 0;

	// fires a server event created earlier, if bDontBroadcast is set, event is not send to clients
	virtual bool FireEvent(IGameEvent *event, bool bDontBroadcast = false) = 0;

	// fires an event for the local client only, should be used only by client code
	virtual bool FireEventClientSide(IGameEvent *event) = 0;

	// create a new copy of this event, must be free later
	virtual IGameEvent *DuplicateEvent(IGameEvent *event) = 0;

	// if an event was created but not fired for some reason, it has to bee freed, same UnserializeEvent
	virtual void FreeEvent(IGameEvent *event) = 0;

	// write/read event to/from bitbuffer
	virtual bool SerializeEvent(IGameEvent *event, bf_write *buf) = 0;

	// create new KeyValues, must be deleted
	virtual IGameEvent *UnserializeEvent(bf_read *buf) = 0;
};
