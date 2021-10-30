#pragma once
#include "Control.h"

class Space : public Control
{
public:
	virtual void Draw() {}
	virtual bool Think() { return false; }

	Space(float height = 3.f)
	{
		this->width  = 100;
		this->height = height;
	}
};
