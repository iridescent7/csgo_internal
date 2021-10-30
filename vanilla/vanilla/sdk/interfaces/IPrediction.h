#pragma once

#include "../math/QAngle.h"
#include "../classes/CUserCmd.h"
#include "IMoveHelper.h"
#include "vfunc.h"

class C_BasePlayer;
class CMoveData;

class IGameMovement
{
public:
    virtual			~IGameMovement(void) {}

    virtual void	          ProcessMovement(C_BasePlayer *pPlayer, CMoveData *pMove) = 0;
    virtual void	          Reset(void) = 0;
    virtual void	          StartTrackPredictionErrors(C_BasePlayer *pPlayer) = 0;
    virtual void	          FinishTrackPredictionErrors(C_BasePlayer *pPlayer) = 0;
    virtual void	          DiffPrint(char const *fmt, ...) = 0;
    virtual Vector const&	  GetPlayerMins(bool ducked) const = 0;
    virtual Vector const&	  GetPlayerMaxs(bool ducked) const = 0;
    virtual Vector const&   GetPlayerViewOffset(bool ducked) const = 0;
    virtual bool		        IsMovingPlayerStuck(void) const = 0;
    virtual C_BasePlayer*   GetMovingPlayer(void) const = 0;
    virtual void		        UnblockPusher(C_BasePlayer *pPlayer, C_BasePlayer *pPusher) = 0;
    virtual void            SetupMovementBounds(CMoveData *pMove) = 0;
};

class CGameMovement
    : public IGameMovement
{
public:
    virtual ~CGameMovement(void) {}
};

class IPrediction
{
public:
	bool InPrediction()
	{
		typedef bool(__thiscall* oInPrediction)(void*);
		return GetVFunc<oInPrediction>(this, 14)(this);
	}

	void RunCommand(C_BasePlayer *player, CUserCmd *ucmd, IMoveHelper *moveHelper)
	{
		typedef void(__thiscall* oRunCommand)(void*, C_BasePlayer*, CUserCmd*, IMoveHelper*);
		return GetVFunc<oRunCommand>(this, 19)(this, player, ucmd, moveHelper);
	}

	void SetupMove(C_BasePlayer *player, CUserCmd *ucmd, IMoveHelper *moveHelper, void* pMoveData)
	{
		typedef void(__thiscall* oSetupMove)(void*, C_BasePlayer*, CUserCmd*, IMoveHelper*, void*);
		return GetVFunc<oSetupMove>(this, 20)(this, player, ucmd, moveHelper, pMoveData);
	}

	void FinishMove(C_BasePlayer *player, CUserCmd *ucmd, void*pMoveData)
	{
		typedef void(__thiscall* oFinishMove)(void*, C_BasePlayer*, CUserCmd*, void*);
		return GetVFunc<oFinishMove>(this, 21)(this, player, ucmd, pMoveData);
	}
};
