#include "Rigidbody.h"
#include "ComponentHelper.h"

Rigidbody::Rigidbody():mBody(0,nullptr,nullptr),mCollider(nullptr),mMass(1)
{
}

Rigidbody::~Rigidbody()
{
//  死亡标记
//	mBody->setUserPointer(gameObject);
}

void Rigidbody::Init(PhysicsWorld& world, const ColliderPtr& collider, int mass)
{
	Component::Init();

	mCollider = collider;
	mMass = mass;

	//惯性    　　　
	btVector3 inertia;
	//根据密度自动计算并设置惯性     
	mCollider->GetShape()->calculateLocalInertia(mMass, inertia);

	//生成物理刚体变换信息
	mMotionState = btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), btVector3(0,0,0)));

	//生成物理刚体
	mBody = btRigidBody(mMass, &mMotionState, mCollider->GetShape(), inertia);

	//设置用户指针
	mBody.setUserPointer(nullptr);
	//将物理刚体 添加到 物理世界
	world.GetWorld()->addRigidBody(&mBody);
}

void Rigidbody::Update()
{
	//非活跃状态/睡眠中，则不处理
	if (!mBody.isActive() || mBody.isStaticObject()) {
		return;
	}

	//若目标物体有变化，先进行物理世界更新
	if (mGameObject->HasChanged()) {
		auto pos = mGameObject->GetWorldPosition();
		auto rotation = mGameObject->GetWorldRotation();
		mBody.setCenterOfMassTransform(btTransform(btQuaternion(rotation.x, rotation.y, rotation.z, rotation.w), btVector3(pos.x, pos.y, pos.z)));
	}

	//更新目标物体的位置
	const auto & pos = mBody.getWorldTransform().getOrigin();
	mGameObject->SetWorldPosition(Vector3(pos.x(), pos.y(), pos.z()));
	//更新目标物体的旋转角度
	const auto & rotation = mBody.getWorldTransform().getRotation();
	mGameObject->SetWorldRotation(Vector4(rotation.getX(), rotation.getY(), rotation.getZ(), rotation.getW()));
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
		//更新刚体属性
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