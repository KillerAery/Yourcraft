#pragma once

#include <btBulletDynamicsCommon.h>
#include "Component.h"

class Rigidbody :
	public Component
{
public:
	Rigidbody();
	~Rigidbody();
	void Init(GameObject * gameObject, const btRigidBody::btRigidBodyConstructionInfo& constructionInfor, btDiscreteDynamicsWorld * world);
protected:
	//ŒÔ¿Ì∏’ÃÂ
	btRigidBody * mBody;
};