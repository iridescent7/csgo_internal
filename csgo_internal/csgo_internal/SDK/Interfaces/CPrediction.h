#pragma once
#include "getvfunc.h"
#include "..\Vector.h"

#define MAX_SPLITSCREEN_PLAYERS 1

class C_CSPlayer;
class CUserCmd;

struct CMoveData { unsigned char data[184]; };

class IMoveHelper
{
public:
	void SetHost(C_CSPlayer* host)
	{
		typedef void(__thiscall* OriginalFn)(void*, C_CSPlayer *host);
		getvfunc<OriginalFn>(this, 1)(this, host);
	}
};

class IGameMovement
{
public:
	virtual			~IGameMovement(void) {}

	virtual void	ProcessMovement(C_CSPlayer *pPlayer, CMoveData *pMove) = 0;
	virtual void	Reset(void) = 0;
	virtual void	StartTrackPredictionErrors(C_CSPlayer *pPlayer) = 0;
	virtual void	FinishTrackPredictionErrors(C_CSPlayer *pPlayer) = 0;
	virtual void	DiffPrint(char const *fmt, ...) = 0;

	virtual Vector const&	GetPlayerMins(bool ducked) const = 0;
	virtual Vector const&	GetPlayerMaxs(bool ducked) const = 0;
	virtual Vector const&   GetPlayerViewOffset(bool ducked) const = 0;

	virtual bool			IsMovingPlayerStuck(void) const = 0;
	virtual C_CSPlayer*	GetMovingPlayer(void) const = 0;
	virtual void			UnblockPusher(C_CSPlayer* pPlayer, C_CSPlayer *pPusher) = 0;

	virtual void    SetupMovementBounds(CMoveData *pMove) = 0;
};

class CPrediction
{
	// Construction
public:

	virtual ~CPrediction(void) = 0;//

	virtual void Init(void) = 0;//
	virtual void Shutdown(void) = 0;//

	// Implement IPrediction
public:
	virtual void Think
	(
		int startframe, // World update ( un-modded ) most recently received
		bool validframe, // Is frame data valid
		int incoming_acknowledged, // Last command acknowledged to have been run by server (un-modded)
		int outgoing_command // Last command (most recent) sent to server (un-modded)
	) = 0;//

	virtual void PreEntityPacketReceived(int commands_acknowledged, int current_world_update_packet) = 0;//
	virtual void PostEntityPacketReceived() = 0;//5
	virtual void PostNetworkDataReceived(int commands_acknowledged) = 0;//

	virtual void OnReceivedUncompressedPacket() = 0;//

													// The engine needs to be able to access a few predicted values
	virtual void GetViewOrigin(Vector& org) = 0;//
	virtual void SetViewOrigin(Vector& org) = 0;//
	virtual void GetViewAngles(Vector& ang) = 0;//10
	virtual void SetViewAngles(Vector& ang) = 0;//

	virtual void GetLocalViewAngles(QAngle& ang) = 0;//
	virtual void SetLocalViewAngles(QAngle& ang) = 0;//

	virtual bool InPrediction() const = 0;//14
	virtual bool IsFirstTimePredicted() const = 0;//

	virtual int GetLastAcknowledgedCommandNumber() const = 0;//

//#if !defined( NO_ENTITY_PREDICTION )
	virtual int GetIncomingPacketNumber() const = 0;//
//#endif

	virtual void CheckMovingGround(C_CSPlayer* player, double frametime) = 0;//
	virtual void RunCommand(C_CSPlayer* player, CUserCmd* cmd, IMoveHelper* moveHelper) = 0;//

	virtual void SetupMove(C_CSPlayer* player, CUserCmd* cmd, IMoveHelper* pHelper, CMoveData* move) = 0;//20
	virtual void FinishMove(C_CSPlayer* player, CUserCmd* cmd, CMoveData* move) = 0;//
	virtual void SetIdealPitch(int nSlot, C_CSPlayer* player, const Vector& origin, const Vector& angles, const Vector& viewheight) = 0;//

	virtual void CheckError(int nSlot, C_CSPlayer* player, int commands_acknowledged) = 0;//

public:
	virtual void _Update
	(
		int nSlot,
		bool received_new_world_update,
		bool validframe,            // Is frame data valid
		int incoming_acknowledged,  // Last command acknowledged to have been run by server (un-modded)
		int outgoing_command        // Last command (most recent) sent to server (un-modded)
	) = 0;

	// Actually does the prediction work, returns false if an error occurred
	bool PerformPrediction(int nSlot, C_CSPlayer* localPlayer, bool received_new_world_update, int incoming_acknowledged, int outgoing_command);
	void ShiftIntermediateDataForward(int nSlot, int slots_to_remove, int previous_last_slot);
	void RestoreEntityToPredictedFrame(int nSlot, int predicted_frame);
	int ComputeFirstCommandToExecute(int nSlot, bool received_new_world_update, int incoming_acknowledged, int outgoing_command);
	void DumpEntity(C_CSPlayer* ent, int commands_acknowledged);
	void ShutdownPredictables();
	void ReinitPredictables();
	void RemoveStalePredictedEntities(int nSlot, int last_command_packet);
	void RestoreOriginalEntityState(int nSlot);
	void RunSimulation(int current_command, float curtime, CUserCmd* cmd, C_CSPlayer* localPlayer);
	void Untouch(int nSlot);
	void StorePredictionResults(int nSlot, int predicted_frame);
	bool ShouldDumpEntity(C_CSPlayer* ent);
	void SmoothViewOnMovingPlatform(C_CSPlayer* pPlayer, Vector& offset);
	void ResetSimulationTick();
	void ShowPredictionListEntry(int listRow, int showlist, C_CSPlayer* ent, int& totalsize, int& totalsize_intermediate);
	void FinishPredictionList(int listRow, int showlist, int totalsize, int totalsize_intermediate);
	void CheckPredictConvar();
};
