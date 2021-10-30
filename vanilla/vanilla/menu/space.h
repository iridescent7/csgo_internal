#pragma once
#include "Control.h"

class CSpace : public CControl
{
public:
	virtual void Paint() {}
	virtual bool Update() { return false; }

	CSpace(float height = 3.f)
	{
		this->width  = 100;
		this->height = height;
	}
};
