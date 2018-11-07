#pragma once

#include <btBulletCollisionCommon.h>
#include "Component.h"
#include "GameObject.h"


class Collider :
	public Component
{

public:
	Collider();
	~Collider();
	void Init();
protected:
	//Åö×²Ìå
	btCollisionObject* mCollision;
};
