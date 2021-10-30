#include "utils.h"
#include "..\classes\datamap.h"
#include "..\..\utils\cx_fnv1.h"

namespace datamap
{
	unsigned long FindInDataMap(datamap_t* map, hashed_string name)
	{
		while (map)
		{
			for (int i = 0; i < map->dataNumFields; i++)
			{
				if (map->dataDesc[i].fieldName == nullptr)
					continue;

				if (hash_compare(name, map->dataDesc[i].fieldName))
				{
					return map->dataDesc[i].fieldOffset[TD_OFFSET_NORMAL];
				}

				if (map->dataDesc[i].fieldType == FIELD_EMBEDDED)
				{
					if (map->dataDesc[i].td)
					{
						unsigned int offset;

						if ((offset = FindInDataMap(map->dataDesc[i].td, name)) != 0)
							return offset;
					}
				}
			}

			map = map->baseMap;
		}

		return 0;
	}
}