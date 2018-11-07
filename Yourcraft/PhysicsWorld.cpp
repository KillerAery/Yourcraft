#include "PhysicsWorld.h"
#include "Transform.h"

PhysicsWorld::PhysicsWorld()
{
}

PhysicsWorld::~PhysicsWorld() {
}

void PhysicsWorld::Init() {
	btVector3 worldAabbMin(-10000, -10000, -10000);
	btVector3 worldAabbMax(10000, 10000, 10000);
	int maxProxies = 1024;
	mBroadphase = std::make_shared<btAxisSweep3>(worldAabbMin, worldAabbMax, maxProxies);
	// 设置好碰撞属性 和调度 
	mCollisionConfiguration = std::make_shared<btDefaultCollisionConfiguration>();
	mDispatcher = std::make_shared<btCollisionDispatcher>(mCollisionConfiguration.operator->());
	// 实际上的物理模拟器
	mSolver = std::make_shared<btSequentialImpulseConstraintSolver>();
	//世界
	mDynamicsWorld = std::make_shared<btDiscreteDynamicsWorld>(mDispatcher.get(), mBroadphase.get(), mSolver.get(), mCollisionConfiguration.get());
	//世界重力
	mDynamicsWorld->setGravity(btVector3(0, -10, 0));
	//创建地面
	CreateGround();
}

void PhysicsWorld::StepWorld(float dt) {
	//模拟步长
	mDynamicsWorld->stepSimulation(dt, 20);

	auto & objectArray = mDynamicsWorld->getCollisionObjectArray();
	//先清除待删除物理刚体
	//待删除物理刚体的用户指针指向Entity::NoEntity作为待删除标记
	for (int i = 0; i < objectArray.size();)
	{
		//int entityState = reinterpret_cast<int>(objectArray[i]->getUserPointer());
		//if (entityState == Entity::NoEntity) {
		//	mDynamicsWorld->removeCollisionObject(objectArray[i]);
		//}
		//else {
		//	i++;
		//}
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
		object->SetPosition(Vector3(pos.x(), pos.y(), pos.z()));
		//更新目标物体的旋转角度
		const auto & rotationM = objectArray[i]->getWorldTransform().getRotation();
		//object->SetRotation(Vector4f(rotationM.getX(), rotationM.getY(), rotationM.getZ(), rotationM.getW()));
	}
}

btDiscreteDynamicsWorld* PhysicsWorld::GetWorld()const
{
	return mDynamicsWorld.get();
}

void PhysicsWorld::CreateGround() {
	//地面刚体 设置
	//mGroundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
	auto a = new btCollisionObject();
	mGroundShape = std::make_shared<btBoxShape>(btVector3(8.8f, 1.0f, 8.8f));
	mGroundMotionState = std::make_shared<btDefaultMotionState>(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, mGroundMotionState.get(), mGroundShape.get(), btVector3(0, 0, 0));
	mGroundbody = std::make_shared<btRigidBody>(groundRigidBodyCI);
	mGroundbody->setFriction(10.0f);
	//将地面刚体添加到 物理世界
	mDynamicsWorld->addRigidBody(mGroundbody.get());
}
