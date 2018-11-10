#include "Rigidbody.h"



Rigidbody::Rigidbody()
{
}

void Rigidbody::Init(GameObject * gameObject, const btRigidBody::btRigidBodyConstructionInfo& constructionInfor, btDiscreteDynamicsWorld * world)
{
	Component::Init(gameObject);
	//�����������
	mBody = new btRigidBody(constructionInfor);
	//�����û�ָ��
	mBody->setUserPointer(gameObject);
	//��������� ��ӵ� ��������
	world->addRigidBody(mBody);
}

Rigidbody::~Rigidbody()
{
}
