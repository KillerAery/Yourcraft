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
	// ���ú���ײ���� �͵��� 
	mCollisionConfiguration = std::make_shared<btDefaultCollisionConfiguration>();
	mDispatcher = std::make_shared<btCollisionDispatcher>(mCollisionConfiguration.operator->());
	// ʵ���ϵ�����ģ����
	mSolver = std::make_shared<btSequentialImpulseConstraintSolver>();
	//����
	mDynamicsWorld = std::make_shared<btDiscreteDynamicsWorld>(mDispatcher.get(), mBroadphase.get(), mSolver.get(), mCollisionConfiguration.get());
	//��������
	mDynamicsWorld->setGravity(btVector3(0, -10, 0));
	//��������
	CreateGround();
}

void PhysicsWorld::StepWorld(float dt) {
	//ģ�ⲽ��
	mDynamicsWorld->stepSimulation(dt, 20);

	auto & objectArray = mDynamicsWorld->getCollisionObjectArray();
	//�������ɾ���������
	//��ɾ�����������û�ָ��ָ��Entity::NoEntity��Ϊ��ɾ�����
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

	//������������ÿһ����������	
	for (int i = 0; i < objectArray.size(); ++i)
	{
		//�������û�ָ�����˯���У��򲻴���
		if (!objectArray[i]->isActive() || objectArray[i]->isStaticObject())continue;
		Transform* object = reinterpret_cast<Transform*>(objectArray[i]->getUserPointer());
		if (!object)continue;
		//����Ŀ�������λ��
		const auto & pos = objectArray[i]->getWorldTransform().getOrigin();
		object->SetPosition(Vector3(pos.x(), pos.y(), pos.z()));
		//����Ŀ���������ת�Ƕ�
		const auto & rotationM = objectArray[i]->getWorldTransform().getRotation();
		//object->SetRotation(Vector4f(rotationM.getX(), rotationM.getY(), rotationM.getZ(), rotationM.getW()));
	}
}

btDiscreteDynamicsWorld* PhysicsWorld::GetWorld()const
{
	return mDynamicsWorld.get();
}

void PhysicsWorld::CreateGround() {
	//������� ����
	//mGroundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
	auto a = new btCollisionObject();
	mGroundShape = std::make_shared<btBoxShape>(btVector3(8.8f, 1.0f, 8.8f));
	mGroundMotionState = std::make_shared<btDefaultMotionState>(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, mGroundMotionState.get(), mGroundShape.get(), btVector3(0, 0, 0));
	mGroundbody = std::make_shared<btRigidBody>(groundRigidBodyCI);
	mGroundbody->setFriction(10.0f);
	//�����������ӵ� ��������
	mDynamicsWorld->addRigidBody(mGroundbody.get());
}
