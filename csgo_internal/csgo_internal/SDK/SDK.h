#pragma once
#pragma warning (disable:4244)

// CRT's memory leak detection
#if defined(DEBUG) || defined(_DEBUG)
#include <crtdbg.h>
#endif

#include "csgo_enums.h"

#include "Interfaces\CCSPlayer.h"
#include "Interfaces\CBaseCombatWeapon.h"
#include "Interfaces\CClientEntityList.h"
#include "Interfaces\CGlowObjectManager.h"
#include "Interfaces\CInputSystem.h"
#include "Interfaces\CPanel.h"
#include "Interfaces\CPrediction.h"
#include "Interfaces\CTrace.h"
#include "Interfaces\GameEvents.h"
#include "Interfaces\IClient.h"
#include "Interfaces\IEngine.h"
#include "Interfaces\IMaterial.h"
#include "Interfaces\IMemAlloc.h"
#include "Interfaces\INetChannelInfo.h"
#include "Interfaces\ISurface.h"
#include "Interfaces\IVDebugOverlay.h"
#include "Interfaces\IViewRenderBeams.h"
#include "Interfaces\ModelRender.h"
#include "Interfaces\RenderView.h"

#include "Color.h"
#include "ConVar.h"
#include "modelchanger.h"
#include "ModelInfo.h"
#include "QAngle.h"
#include "Quaternion.h"
#include "RecvData.h"
#include "studio.h"
#include "Vector.h"
#include "Vector2D.h"
#include "VMatrix.h"
#include "checksum_md5.h"

#include "..\chocoladevkit\cstr.h"
#include "..\chocoladevkit\hash.h"

#define IN_ATTACK					(1 << 0)
#define IN_JUMP						(1 << 1)
#define IN_DUCK						(1 << 2)
#define IN_FORWARD					(1 << 3)
#define IN_BACK						(1 << 4)
#define IN_USE						(1 << 5)
#define IN_CANCEL					(1 << 6)
#define IN_LEFT						(1 << 7)
#define IN_RIGHT					(1 << 8)
#define IN_MOVELEFT					(1 << 9)
#define IN_MOVERIGHT				(1 << 10)
#define IN_ATTACK2					(1 << 11)
#define IN_RUN						(1 << 12)
#define IN_RELOAD					(1 << 13)
#define IN_ALT1						(1 << 14)
#define IN_ALT2						(1 << 15)
#define IN_SCORE					(1 << 16)
#define IN_SPEED					(1 << 17)
#define IN_WALK						(1 << 18)
#define IN_ZOOM						(1 << 19)
#define IN_WEAPON1					(1 << 20)
#define IN_WEAPON2					(1 << 21)
#define IN_BULLRUSH					(1 << 22)
#define IN_GRENADE1					(1 << 23)
#define IN_GRENADE2					(1 << 24)
#define	IN_ATTACK3					(1 << 25)

#define	FL_ONGROUND					(1 << 0)
#define FL_DUCKING					(1 << 1)
#define	FL_WATERJUMP				(1 << 2)
#define FL_ONTRAIN					(1 << 3)
#define FL_INRAIN					(1 << 4)
#define FL_FROZEN					(1 << 5)
#define FL_ATCONTROLS				(1 << 6)
#define	FL_CLIENT					(1 << 7)
#define FL_FAKECLIENT				(1 << 8)
#define	FL_INWATER					(1 << 9)
#define	FL_FLY						(1 << 10)
#define	FL_SWIM						(1 << 11)
#define	FL_CONVEYOR					(1 << 12)
#define	FL_NPC						(1 << 13)
#define	FL_GODMODE					(1 << 14)
#define	FL_NOTARGET					(1 << 15)
#define	FL_AIMTARGET				(1 << 16)
#define	FL_PARTIALGROUND			(1 << 17)
#define FL_STATICPROP				(1 << 18)
#define FL_GRAPHED					(1 << 19)
#define FL_GRENADE					(1 << 20)
#define FL_STEPMOVEMENT				(1 << 21)
#define FL_DONTTOUCH				(1 << 22)
#define FL_BASEVELOCITY				(1 << 23)
#define FL_WORLDBRUSH				(1 << 24)
#define FL_OBJECT					(1 << 25)
#define FL_KILLME					(1 << 26)
#define FL_ONFIRE					(1 << 27)
#define FL_DISSOLVING				(1 << 28)
#define FL_TRANSRAGDOLL				(1 << 29)
#define FL_UNBLOCKABLE_BY_PLAYER	(1 << 30)

#define PLAYER_FLAG_BITS			10

#define DAMAGE_NO		      0
#define DAMAGE_EVENTS_ONLY    1	
#define DAMAGE_YES		      2
#define DAMAGE_AIM		      3

#define HITGROUP_GENERIC	  0
#define HITGROUP_HEAD         1
#define HITGROUP_CHEST        2
#define HITGROUP_STOMACH      3
#define HITGROUP_LEFTARM      4
#define HITGROUP_RIGHTARM     5
#define HITGROUP_LEFTLEG      6
#define HITGROUP_RIGHTLEG     7
#define HITGROUP_GEAR         10

#define MAX_STUDIOBONES 128
#define MAXSTUDIOSKINS 32

#define BONE_USED_MASK				0x0007FF00
#define BONE_USED_BY_ANYTHING		0x0007FF00
#define BONE_USED_BY_HITBOX			0x00000100	// bone (or child) is used by a hit box
#define BONE_USED_BY_ATTACHMENT		0x00000200	// bone (or child) is used by an attachment point
#define BONE_USED_BY_VERTEX_MASK	0x0003FC00
#define BONE_USED_BY_VERTEX_LOD0	0x00000400	// bone (or child) is used by the toplevel model via skinned vertex
#define BONE_USED_BY_VERTEX_LOD1	0x00000800	
#define BONE_USED_BY_VERTEX_LOD2	0x00001000  
#define BONE_USED_BY_VERTEX_LOD3	0x00002000
#define BONE_USED_BY_VERTEX_LOD4	0x00004000
#define BONE_USED_BY_VERTEX_LOD5	0x00008000
#define BONE_USED_BY_VERTEX_LOD6	0x00010000
#define BONE_USED_BY_VERTEX_LOD7	0x00020000
#define BONE_USED_BY_BONE_MERGE		0x00040000	// bone is available for bone merge to occur against it

#define MAXSTUDIOBONES 128
#define MAX_PLAYERS 64

#define TIME_TO_TICKS( dt )		( (int)( 0.5 + (float)(dt) / I::Globals->interval_per_tick ) )
#define TICKS_TO_TIME( t )		( I::Globals->interval_per_tick *( t ) )

#define MAX_COORD_FLOAT ( 16384.0f )
#define MIN_COORD_FLOAT ( -MAX_COORD_FLOAT )

template < typename T >
inline void Clamp(T& val, const T& min, const T& max)
{
	if (val < min)
		val = min;
	else if (val > max)
		val = max;
}

class IClientModeShared;

enum ClientFrameStage_t
{
	FRAME_UNDEFINED = -1,
	FRAME_START,
	FRAME_NET_UPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_START,
	FRAME_NET_UPDATE_POSTDATAUPDATE_END,
	FRAME_NET_UPDATE_END,
	FRAME_RENDER_START,
	FRAME_RENDER_END
};

class CGlobalVars
{
public:
	float	realtime; // 0x4
	int		framecount; // 0x8
	float	absoluteframetime; // 0xC
	float	absoluteframestarttimestddev; // 0x10
	float	curtime; // 0x14
	float	frametime; // 0x18
	int		maxclients; // 0x1B
	int		tickcount; //0x1F
	float	interval_per_tick;
	float	interpolation_amount;
	int		simTicksThisFrame;
	int		network_protocol;
	char	pad00[14];
};

class CUserCmd
{
public:
	char	pad_0x0000[0x4]; //0x0000
	int		command_number;		// 0x04 For matching server and client commands for debugging
	int		tick_count;			// 0x08 the tick the client created this command
	QAngle	viewangles;			// 0x0C Player instantaneous view angles.
	Vector	aimdirection;		// 0x18
	float	forwardmove;		// 0x24
	float	sidemove;			// 0x28
	float	upmove;				// 0x2C
	int		buttons;			// 0x30 Attack button states
	byte    impulse;			// 0x34
	int		weaponselect;		// 0x38 Current weapon id
	int		weaponsubtype;		// 0x3C
	int		random_seed;		// 0x40 For shared random functions
	short	mousedx;			// 0x44 mouse accum in x from create move
	short	mousedy;			// 0x46 mouse accum in y from create move
	bool	hasbeenpredicted;	// 0x48 Client only, tracks whether we've predicted this command at least once
	char	pad_0x4C[0x18];		// 0x4C Current sizeof( usercmd ) =  100  = 0x64
};

class CVerifiedUserCmd
{
public:
	CUserCmd	m_cmd;
	unsigned long	m_crc;
};

class CInput
{
public:
	void*               pvftable;
	char				pad[8];//0x00
	bool                m_fTrackIRAvailable;          //0x04
	bool                m_fMouseInitialized;          //0x05
	bool                m_fMouseActive;               //0x06
	bool                m_fJoystickAdvancedInit;      //0x07
	char                pad_0x08[0x2C];               //0x08
	void*               m_pKeys;                      //0x34
	char                pad_0x38[0x64];               //0x38
	int                    pad_0x41;
	int                    pad_0x42;
	bool                m_fCameraInterceptingMouse;   //0x9C
	bool                m_fCameraInThirdPerson;       //0x9D
	bool                m_fCameraMovingWithMouse;     //0x9E
	Vector                m_vecCameraOffset;            //0xA0
	bool                m_fCameraDistanceMove;        //0xAC
	int                 m_nCameraOldX;                //0xB0
	int                 m_nCameraOldY;                //0xB4
	int                 m_nCameraX;                   //0xB8
	int                 m_nCameraY;                   //0xBC
	bool                m_CameraIsOrthographic;       //0xC0
	Vector              m_angPreviousViewAngles;      //0xC4
	Vector              m_angPreviousViewAnglesTilt;  //0xD0
	float               m_flLastForwardMove;          //0xDC
	int                 m_nClearInputState;           //0xE0
	char                pad_0xE4[0x8];                //0xE4
	CUserCmd*           m_pCommands;                  //0xEC
	CVerifiedUserCmd*   m_pVerifiedCommands;          //0xF0
};

class NetChannel
{
public:
	char pad_0x0000[0x17];
	bool m_bShouldDelete;
	int m_nOutSequenceNr;
	int m_nInSequenceNr;
	int m_nOutSequenceNrAck;
	int m_nOutReliableState;
	int m_nInReliableState;
	int m_nChokedPackets;
};

struct CViewSetup
{
	char _0x0000[16];
	__int32 x;
	__int32 x_old;
	__int32 y;
	__int32 y_old;
	__int32 width;
	__int32    width_old;
	__int32 height;
	__int32    height_old;
	char _0x0030[128];
	float fov;
	float fovViewmodel;
	Vector origin;
	Vector angles;
	float zNear;
	float zFar;
	float zNearViewmodel;
	float zFarViewmodel;
	float m_flAspectRatio;
	float m_flNearBlurDepth;
	float m_flNearFocusDepth;
	float m_flFarFocusDepth;
	float m_flFarBlurDepth;
	float m_flNearBlurRadius;
	float m_flFarBlurRadius;
	float m_nDoFQuality;
	__int32 m_nMotionBlurMode;
	char _0x0104[68];
	__int32 m_EdgeBlur;
};

struct RenderableInfo_t
{
	IClientRenderable* m_pRenderable;
	void* m_pAlphaProperty;
	int m_EnumCount;
	int m_nRenderFrame;
	unsigned short m_FirstShadow;
	unsigned short m_LeafList;
	short m_Area;
	unsigned __int16 m_Flags;   // 0x0016
	unsigned __int16 m_Flags2; // 0x0018
	Vector m_vecBloatedAbsMins;
	Vector m_vecBloatedAbsMaxs;
	Vector m_vecAbsMins;
	Vector m_vecAbsMaxs;
	int pad;
};

class CSWeaponInfo
{
public:
	char _0x0000[20];
	__int32 max_clip;			//0x0014
	char _0x0018[12];
	__int32 max_reserved_ammo;	//0x0024
	char _0x0028[88];
	char* bullet_type;			//0x0080	
	char _0x0084[4];
	char* hud_name;				//0x0088
	char* weapon_name;			//0x008C
	char _0x0090[56];
	__int32 weapon_type;		//0x00CC
	__int32 basic_weapon_type;		//0x00CC
	__int32 price;				//0x00D0
	__int32 reward;				//0x00D4
	char _0x00D8[20];
	BYTE full_auto;				//0x00EC
	char _0x00ED[3];
	__int32 damage;				//0x00F0
	float armor_ratio;			//0x00F4
	__int32 bullets;			//0x00F8
	float penetration;			//0x00FC
	char _0x0100[8];
	float range;				//0x0108
	float range_modifier;		//0x010C
	char _0x0110[16];
	BYTE has_silencer;			//0x0120
	char _0x0121[15];
	float max_speed;			//0x0130
	float max_speed_alt;		//0x0134
	char _0x0138[76];
	__int32 recoil_seed;		//0x0184
	char _0x0188[32];
};

class CCSPlayerAnimState
{
public:
	char pad_0000[120]; //0x0000
	float m_flEyeYaw; //0x0078
	float m_flPitch; //0x007C
	float m_flGoalFeetYaw; //0x0080
	float m_flCurrentFeetYaw; //0x0084
	char pad_0088[28]; //0x0088
	float m_flDuckAmount; //0x00A4
	char pad_00A8[8]; //0x00A8
	Vector m_vecOrigin; //0x00B0
	Vector m_vecLastOrigin; //0x00BC
	char pad_00C8[0x30]; //0x00C8
	float m_flSpeedFraction; //0x00F8
	float m_flSpeedFactor; //0x00FC
	char pad_0100[28]; //0x0100
	float m_flLandingRatio; //0x011C
	char pad_0120[528]; //0x0120
	float m_flMinBodyYawDegrees; //0x0330
	float m_flMaxBodyYawDegrees; //0x0334
};

struct player_info_t
{
	char __pad0[0x8];

	int xuidlow;
	int xuidhigh;

	char name[128];
	int  userid;
	char guid[33];

	char __pad1[0x17B];
};
