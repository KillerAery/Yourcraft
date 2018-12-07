#pragma once

#include <btBulletDynamicsCommon.h>
#include "Component.h"
#include "Collider.h"
#include "PhysicsWorld.h"

//����������
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
	//�������
	btRigidBody mBody;
	//�˶�״̬
	btDefaultMotionState mMotionState;
	//��ײ��
	ColliderPtr mCollider;
	//�ܶ�
	int mMass;
};

