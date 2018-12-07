#pragma once

#include <btBulletDynamicsCommon.h>
#include "Component.h"
#include "Collider.h"
#include "PhysicsWorld.h"

//物理刚体组件
class Rigidbody :
	public Component
{
public:
	Rigidbody();
	~Rigidbody();

	void Init(PhysicsWorld& world, const ColliderPtr& collider ,int mass = 1);

	void Update();

	void BindGameObject(GameObject * gameObject);
	void UnbindGameObject();

protected:
	//物理刚体
	btRigidBody mBody;
	//运动状态
	btDefaultMotionState mMotionState;
	//碰撞体
	ColliderPtr mCollider;
	//密度
	int mMass;
};

