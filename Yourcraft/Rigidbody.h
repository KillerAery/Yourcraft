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
	//物理刚体
	btRigidBody * mBody;
	//密度
	btScalar mMass;
	//惯性
	btVector3 mInertia;
};