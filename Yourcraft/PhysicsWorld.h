#pragma once
#include <memory>
#include "Bullet/inc/btBulletDynamicsCommon.h"

class PhysicsWorld
{
public:
	PhysicsWorld();
	~PhysicsWorld();

	void Init();

	//����ģ�ⲽ��
	void StepWorld(float dt);
	btDiscreteDynamicsWorld* GetWorld()const;
private:
	//����ģ������
	btAxisSweep3* mBroadphase;
	btDefaultCollisionConfiguration* mCollisionConfiguration;
	btCollisionDispatcher* mDispatcher;
	btSequentialImpulseConstraintSolver* mSolver;
	//�������籾��
	btDiscreteDynamicsWorld* mDynamicsWorld;
};

