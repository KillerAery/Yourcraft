#include "Rigidbody.h"



Rigidbody::Rigidbody()
{
}

void Rigidbody::Init(GameObject * gameObject, const btRigidBody::btRigidBodyConstructionInfo& constructionInfor, btDiscreteDynamicsWorld * world)
{
	Component::Init(gameObject);
	//生成物理刚体
	mBody = new btRigidBody(constructionInfor);
	//设置用户指针
	mBody->setUserPointer(gameObject);
	//将物理刚体 添加到 物理世界
	world->addRigidBody(mBody);
}

Rigidbody::~Rigidbody()
{
}
