#pragma once
#include "..\SDK\SDK.h"
#include <vector>

enum
{
	REMOVALS_RECOIL,
	REMOVALS_ZOOM,
	REMOVALS_DECALS,
	REMOVALS_SCOPE_OVERLAY,
	REMOVALS_SCOPE_BLUR,
	REMOVALS_FLASH,
	REMOVALS_SMOKE,
};

class CVisualExtras
{
private:
	DWORD next_tp_update = 0;

	bool last_no_recoil = false;
	bool last_zoom_sens = false;

	std::vector<IMaterial*> smoke_materials;
	IMaterial* scope_blur_mat;
	bool first_load = true;

public:
	void CreateMove(CUserCmd* cmd, bool* send_packet);
	void FrameStageNotify(int stage);
	void OverrideView(CViewSetup* pSetup);

public:
	bool VisualRemovals[7] = { false };

	bool Thirdperson = false;
	int  ThirdpersonKey = -1;

	bool OverrideFov = false;

	bool DeadVisuals = true;

	int Fov = 90;
	int ThirdpersonFov = 90;
};