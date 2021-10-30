#pragma once
#include <cstdint>

enum hotkeymode_t
{
	hk_always,
	hk_hold,
	hk_toggle
};

#pragma pack(push, 1)
struct hotkey_t
{
public:
	uint32_t key  = NULL;
	uint32_t mode = hk_hold;

	bool pressed = false;
};
#pragma pack(pop)
