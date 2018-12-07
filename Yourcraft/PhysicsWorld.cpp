#include "PhysicsWorld.h"
#include "Transform.h"

#define SAFE_DELETE_PTR(ptr) do{if(ptr){delete ptr;ptr = nullptr;}}while(0);

PhysicsWorld::PhysicsWorld() :
	mBroadphase(nullptr),
	mCollisionConfiguration(nullptr),
	mDispatcher(nullptr),
	mSolver(nullptr),
	mDynamicsWorld(nullptr)
{

}

PhysicsWorld::~PhysicsWorld() {
	SAFE_DELETE_PTR(mBroadphase);
	SAFE_DELETE_PTR(mCollisionConfiguration);
	SAFE_DELETE_PTR(mDispatcher);
	SAFE_DELETE_PTR(mSolver);
	//SAFE_DELETE_PTR(mDynamicsWorld);
}

void PhysicsWorld::Init() {
	btVector3 worldAabbMin(-10000, -10000, -10000);
	btVector3 worldAabbMax(10000, 10000, 10000);
	int maxProxies = 1024;
	mBroadphase = new btAxisSweep3(worldAabbMin, worldAabbMax, maxProxies);
	// 设置好碰撞属性 和调度 
	mCollisionConfiguration = new btDefaultCollisionConfiguration();
	mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
	// 实际上的物理模拟器
	mSolver = new btSequentialImpulseConstraintSolver();
	//世界
	mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher, mBroadphase, mSolver, mCollisionConfiguration);
	//世界重力
	mDynamicsWorld->setGravity(btVector3(0, -10, 0));
}

void PhysicsWorld::StepWorld(float dt) {
	//模拟步长
	mDynamicsWorld->stepSimulation(dt, 10);

	auto & objectArray = mDynamicsWorld->getCollisionObjectArray();
	//先清除待删除物理刚体
	//待删除物理刚体的用户指针指向nullptr作为待删除标记
	for (int i = 0; i < objectArray.size();)
	{
		Transform* userPointer = reinterpret_cast<Transform*>(objectArray[i]->getUserPointer());
		if (userPointer == nullptr || !userPointer->IsAlive()){
			mDynamicsWorld->removeCollisionObject(objectArray[i]);
		}
		else {
			i++;
		}
	}

	//更新物理世界每一个物理物体	
	for (int i = 0; i < objectArray.size(); ++i)
	{
		//不存在用户指针或者睡眠中，则不处理
		if (!objectArray[i]->isActive() || objectArray[i]->isStaticObject())continue;
		Transform* object = reinterpret_cast<Transform*>(objectArray[i]->getUserPointer());
		if (!object)continue;
		//更新目标物体的位置
		const auto & pos = objectArray[i]->getWorldTransform().getOrigin();
		object->SetWorldPosition(Vector3(pos.x(), pos.y(), pos.z()));

		//更新目标物体的旋转角度
		const auto & rotationM = objectArray[i]->getWorldTransform().getRotation();
		object->SetWorldRotation(Vector4(rotationM.getX(), rotationM.getY(), rotationM.getZ(), rotationM.getW()));
	}

}

btDiscreteDynamicsWorld* PhysicsWorld::GetWorld()const
{
	return mDynamicsWorld;
}
