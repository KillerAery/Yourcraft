#include "Rigidbody.h"
#include "ComponentHelper.h"

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
	auto pos = gameObject->GetWorldPosition();
	auto rotation = gameObject->GetWorldRotation();
	mMotionState = btDefaultMotionState(btTransform(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w), btVector3(pos.x, pos.y, pos.z)));

	//�����������
	mBody = btRigidBody(mMass, &mMotionState, mCollider->GetShape(), inertia);

	//�����û�ָ��
	mBody.setUserPointer(gameObject);
	//��������� ��ӵ� ��������
	world.GetWorld()->addRigidBody(&mBody);
}

void Rigidbody::Update()
{
	if (mGameObject->HasChanged()) {
		auto pos = mGameObject->GetWorldPosition();
		auto rotation = mGameObject->GetWorldRotation();
		mBody.setCenterOfMassTransform(btTransform(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w), btVector3(pos.x, pos.y, pos.z)));
	}
}

void Rigidbody::BindGameObject(GameObject * gameObject)
{
	if (mGameObject) {
		UnbindGameObject();
	}
	if (gameObject) {
		mGameObject = gameObject;
		mGameObject->AddComponentInfor(static_cast<int>(ComponentType::Rigidbody),mIndex);
		mBody.setUserPointer(mGameObject);
	}
}

void Rigidbody::UnbindGameObject()
{
	if (mGameObject) {
		mGameObject->RemoveComponentInfor(static_cast<int>(ComponentType::Rigidbody));
		mGameObject = nullptr;
		mBody.setUserPointer(nullptr);
	}
}