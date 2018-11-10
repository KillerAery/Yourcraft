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
	void Init(GameObject * gameObject);
protected:
	//Åö×²Ìå
	btCollisionObject* mCollision;
};
