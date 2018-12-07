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
	//�������ɾ���������
	//��ɾ�����������û�ָ��ָ��nullptr��Ϊ��ɾ�����
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

	//������������ÿһ����������	
	for (int i = 0; i < objectArray.size(); ++i)
	{
		//�������û�ָ�����˯���У��򲻴���
		if (!objectArray[i]->isActive() || objectArray[i]->isStaticObject())continue;
		Transform* object = reinterpret_cast<Transform*>(objectArray[i]->getUserPointer());
		if (!object)continue;
		//����Ŀ�������λ��
		const auto & pos = objectArray[i]->getWorldTransform().getOrigin();
		object->SetWorldPosition(Vector3(pos.x(), pos.y(), pos.z()));

		//����Ŀ���������ת�Ƕ�
		const auto & rotationM = objectArray[i]->getWorldTransform().getRotation();
		object->SetWorldRotation(Vector4(rotationM.getX(), rotationM.getY(), rotationM.getZ(), rotationM.getW()));
	}

}

btDiscreteDynamicsWorld* PhysicsWorld::GetWorld()const
{
	return mDynamicsWorld;
}
