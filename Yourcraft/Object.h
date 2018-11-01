#pragma once

#include "Ref.h"

class Object
{
public:
	Object();
	virtual void Init();
	void Kill();
	bool IsAlive();
	bool IsEnabled();
protected:
	~Object();
	void Dead();
protected:
	bool mAlive;
	bool mEnabled;
};

