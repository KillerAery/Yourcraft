#include "Rigidbody.h"



Rigidbody::Rigidbody()
{
}

void Rigidbody::Init(GameObject * gameObject, const btRigidBody::btRigidBodyConstructionInfo& constructionInfor, btDiscreteDynamicsWorld * world)
{
	//生成 物理身体
	mBody = new btRigidBody(constructionInfor);
	//设置用户指针
	mBody->setUserPointer(this);
	//将物理身体 添加到 物理世界
	world->addRigidBody(mBody);
}

Rigidbody::~Rigidbody()
{
}
