#pragma once
#include "Render.h"
class BatchRender :
	public Render
{
public:
	BatchRender();
	virtual ~BatchRender();
	virtual void Update() = 0;
};

