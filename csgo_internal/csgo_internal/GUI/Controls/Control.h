#pragma once
#include <Windows.h>
#include <string>

class BaseControl
{
public:
	inline void SetPosition(float x, float y)
	{
		this->x = x;
		this->y = y;
	}

	inline void SetAlpha(byte alpha)
	{
		this->alpha = alpha;
	}

public:
	float x, y;
	float width, height;
	byte alpha;
};

class ControlParent;

class Control : public BaseControl
{
public:
	virtual void Draw() {};
	virtual bool Think() { return false; };

public:
	ControlParent* parent;
	std::string Text;
};

class ControlParent
{
public:
	inline void SetFocus(Control* control) { Focus = control; }
	inline void Release(Control* control) { if (control == Focus) Focus = nullptr; }

public:
	Control* Focus;
};

