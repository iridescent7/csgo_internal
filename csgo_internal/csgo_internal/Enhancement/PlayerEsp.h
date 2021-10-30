#pragma once
#include "..\SDK\SDK.h"
#include <string>

enum
{
	ESP_TEAM,
	ESP_ENEMY
};

enum
{
	ESP_NAME,
	ESP_WEAPON,
	ESP_BOX,
	ESP_HEALTH,
	ESP_AMMOBAR,
	ESP_BARREL
};

struct ScreenBBox
{
	int x, y, w, h;
};

class VisualEntity
{
public:
	bool valid = false;
	bool valid_weapon = false;

	bool dormant;

	int health;
	std::string hp_text;

	wchar_t name[25];

	Vector origin;
	bool jumping;
	float duck_amount;

	char weapon_name[25];
	bool weapon_shootable;

	int weapon_ammo;
	int weapon_maxclip;
	std::string weapon_ammo_text;

	bool  weapon_reloading;
	float weapon_reload_cycle;

	Vector headpos;
	Vector eyepos;
	QAngle angles;
};

class CPlayerEsp
{
private:
	VisualEntity visual_entities[MAX_PLAYERS];
	float esp_alpha[MAX_PLAYERS];

	bool   ve_list_cleared = true;
	size_t local_observe_target = 0;

public:
	void CreateMove(CUserCmd* cmd, bool* send_packet);
	void EndScene();

	bool GetPlayerBoundingBox(const VisualEntity& ve, ScreenBBox* bbox);

	void DrawPlayerBox(const VisualEntity& ve, const ScreenBBox& bbox, byte alpha);
	void DrawPlayerHealth(const VisualEntity& ve, const ScreenBBox& bbox, byte alpha);
	void DrawPlayerName(const VisualEntity& ve, const ScreenBBox& bbox, byte alpha);
	void DrawPlayerWeapon(const VisualEntity& ve, const ScreenBBox& bbox, byte alpha);
	void DrawPlayerAmmoBar(const VisualEntity& ve, const ScreenBBox& bbox, byte alpha);
	void DrawBarrel(const VisualEntity& ve, const ScreenBBox& bbox, byte alpha);

public:
	bool EspTeams[2] = { false, true };
	bool EspFeatures[6] = { true, true, true, true, false, true };
};
