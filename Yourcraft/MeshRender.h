#pragma once
#include "Render.h"

class MeshRender :
	public Render
{
public:
	MeshRender();
	virtual ~MeshRender();
	void Update(float dt);
};

