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
protected:
	//��ײ��
	btCollisionObject* mCollision;
};
