#include "Rigidbody.h"



Rigidbody::Rigidbody():mBody(0,nullptr,nullptr),mCollider(nullptr),mMass(1)
{
}

Rigidbody::~Rigidbody()
{
//  死亡标记
//	mBody->setUserPointer(gameObject);
}

void Rigidbody::Init(GameObject* gameObject, PhysicsWorld* world, ColliderPtr& collider, int mass)
{
	Component::Init(gameObject);
	mCollider = collider;
	mMass = mass;

	//惯性    　　　
	btVector3 inertia;
	//根据密度自动计算并设置惯性     
	mCollider->GetShape()->calculateLocalInertia(mMass,inertia);

	//生成物理刚体变换信息
	mMotionState = btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 10, 0)));

	//生成物理刚体
	mBody = btRigidBody(mMass, &mMotionState, mCollider->GetShape(), inertia);

	//设置用户指针
	mBody.setUserPointer(gameObject);
	//将物理刚体 添加到 物理世界
	world->GetWorld()->addRigidBody(&mBody);
}