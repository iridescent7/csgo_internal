#pragma once

class IRefCounted
{
private:
	volatile long RefCount;

public:
	virtual void Destructor(char bDelete) = 0;
	virtual bool OnFinalRelease() = 0;

	void Unreference();
};