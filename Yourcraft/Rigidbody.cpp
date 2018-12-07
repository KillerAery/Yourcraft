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

void Rigidbody::Init(PhysicsWorld& world, const ColliderPtr& collider, int mass)
{
	Component::Init();

	mCollider = collider;
	mMass = mass;

	//����    ������
	btVector3 inertia;
	//�����ܶ��Զ����㲢���ù���     
	mCollider->GetShape()->calculateLocalInertia(mMass, inertia);

	//�����������任��Ϣ
	mMotionState = btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), btVector3(0,0,0)));

	//�����������
	mBody = btRigidBody(mMass, &mMotionState, mCollider->GetShape(), inertia);

	//�����û�ָ��
	mBody.setUserPointer(nullptr);
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
		//���¸�������
		auto pos = mGameObject->GetWorldPosition();
		auto rotation = mGameObject->GetWorldRotation();
		mBody.setCenterOfMassTransform(btTransform(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w), btVector3(pos.x, pos.y, pos.z)));
	}
}

void Rigidbody::UnbindGameObject()
{
	if (mGameObject) {
		mBody.setUserPointer(nullptr);
		mGameObject->RemoveComponentInfor(static_cast<int>(ComponentType::Rigidbody));
		mGameObject = nullptr;
	}
}