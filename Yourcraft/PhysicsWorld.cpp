#include "PhysicsWorld.h"
#include "Transform.h"
#include "GameObject.h"

PhysicsWorld::PhysicsWorld() :
	mBroadphase(nullptr),
	mCollisionConfiguration(nullptr),
	mDispatcher(nullptr),
	mSolver(nullptr),
	mDynamicsWorld(nullptr)
{
}

#define SAFE_DELETE_PTR(ptr) do{if(ptr){delete ptr;ptr = nullptr;}}while(0);

PhysicsWorld::~PhysicsWorld() {
	//������delete DynamicWorld
	SAFE_DELETE_PTR(mDynamicsWorld);
	//��delete���������Դ
	SAFE_DELETE_PTR(mBroadphase);
	SAFE_DELETE_PTR(mCollisionConfiguration);
	SAFE_DELETE_PTR(mDispatcher);
	SAFE_DELETE_PTR(mSolver);
}

void PhysicsWorld::Init() {
	btVector3 worldAabbMin(-10000, -10000, -10000);
	btVector3 worldAabbMax(10000, 10000, 10000);
	int maxProxies = 1024;
	mBroadphase = new btAxisSweep3(worldAabbMin, worldAabbMax, maxProxies);
	// ���ú���ײ���� �͵��� 
	mCollisionConfiguration = new btDefaultCollisionConfiguration();
	mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
	// ʵ���ϵ�����ģ����
	mSolver = new btSequentialImpulseConstraintSolver();
	//����
	mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher, mBroadphase, mSolver, mCollisionConfiguration);
	//��������
	mDynamicsWorld->setGravity(btVector3(0, -10, 0));
}

void PhysicsWorld::StepWorld(float dt) {
	//ģ�ⲽ��
	mDynamicsWorld->stepSimulation(dt, 10);

	auto & objectArray = mDynamicsWorld->getCollisionObjectArray();
	//������������ÿһ���������� 
	for (int i = 0; i < objectArray.size();)
	{
		Transform* object = static_cast<Transform*>(objectArray[i]->getUserPointer());
		//�����ɾ���������
		//��ɾ�����������û�ָ��ָ��nullptr��Ϊ��ɾ�����
		if (object == nullptr || !object->IsAlive()) {
			mDynamicsWorld->removeCollisionObject(objectArray[i]);
		}
		else {
			++i;
		}
	}

}

btDiscreteDynamicsWorld* PhysicsWorld::GetWorld()const
{
	return mDynamicsWorld;
}
