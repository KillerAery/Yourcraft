#pragma once
#include "Object.h"

class Component
{
public:
	Component();
	virtual ~Component();
protected:
	virtual void Update() = 0;
};

