#pragma once
#include "Component.h"

class Render :
	public Component
{
public:
	Render();
	virtual ~Render() = 0;
	void Update(float dt);
};

