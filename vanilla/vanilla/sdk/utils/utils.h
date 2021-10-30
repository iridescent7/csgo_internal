#pragma once
#include <cstdint>
#include "characterset.h"
#include "checksum_crc.h"
#include "checksum_md5.h"
#include "..\..\utils\cx_fnv1.h"

struct datamap_t;

namespace datamap
{
	unsigned long FindInDataMap(datamap_t* map, hashed_string name);
}