#include "datamap.h"

namespace datamap
{
	unsigned int FindOffsetInDataMap(datamap_t* pMap, const char* name)
	{
		while (pMap)
		{
			for (int i = 0; i < pMap->dataNumFields; i++)
			{
				if (pMap->dataDesc[i].fieldName == NULL)
					continue;

				if (strcmp(name, pMap->dataDesc[i].fieldName) == 0)
					return pMap->dataDesc[i].fieldOffset[TD_OFFSET_NORMAL];

				if (pMap->dataDesc[i].fieldType == FIELD_EMBEDDED)
				{
					if (pMap->dataDesc[i].td)
					{
						unsigned int offset;

						if ((offset = FindOffsetInDataMap(pMap->dataDesc[i].td, name)) != 0)
							return offset;
					}
				}
			}

			pMap = pMap->baseMap;
		}

		return NULL;
	}
}
