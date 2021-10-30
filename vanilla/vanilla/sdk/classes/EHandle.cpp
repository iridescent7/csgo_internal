#include "EHandle.h"
#include "IHandleEntity.h"
#include "..\..\globals.h"

// -------------------------------------------------------------------------------------------------- //
// Game-code CBaseHandle implementation.
// -------------------------------------------------------------------------------------------------- //

inline IHandleEntity* CBaseHandle::Get() const
{
	return g_entitylist->GetClientEntityFromHandle(*this);
}
