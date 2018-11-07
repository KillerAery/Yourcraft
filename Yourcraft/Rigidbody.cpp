#include "Rigidbody.h"



Rigidbody::Rigidbody()
{
}

void Rigidbody::Init(GameObject * gameObject, const btRigidBody::btRigidBodyConstructionInfo& constructionInfor, btDiscreteDynamicsWorld * world)
{
	//���� ��������
	mBody = new btRigidBody(constructionInfor);
	//�����û�ָ��
	mBody->setUserPointer(this);
	//���������� ��ӵ� ��������
	world->addRigidBody(mBody);
}

Rigidbody::~Rigidbody()
{
}
