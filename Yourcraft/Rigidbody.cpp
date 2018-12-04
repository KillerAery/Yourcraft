#include "Rigidbody.h"



Rigidbody::Rigidbody():mBody(0,nullptr,nullptr),mCollider(nullptr),mMass(1)
{
}

Rigidbody::~Rigidbody()
{
//  �������
//	mBody->setUserPointer(gameObject);
}

void Rigidbody::Init(GameObject* gameObject, PhysicsWorld& world, const ColliderPtr& collider, int mass)
{
	Component::Init(gameObject);
	mCollider = collider;
	mMass = mass;

	//����    ������
	btVector3 inertia;
	//�����ܶ��Զ����㲢���ù���     
	mCollider->GetShape()->calculateLocalInertia(mMass, inertia);

	//�����������任��Ϣ
	auto pos = gameObject->GetPosition();
	mMotionState = btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(pos.x, pos.y, pos.z)));

	//�����������
	mBody = btRigidBody(mMass, &mMotionState, mCollider->GetShape(), inertia);

	//�����û�ָ��
	mBody.setUserPointer(gameObject);
	//��������� ��ӵ� ��������
	world.GetWorld()->addRigidBody(&mBody);
}