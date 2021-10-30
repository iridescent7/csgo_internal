#include "IRefCounted.h"
#include <Windows.h>

void IRefCounted::Unreference()
{
	if (InterlockedDecrement(&RefCount) == 0 && OnFinalRelease())
	{
		Destructor(1);
	}
}