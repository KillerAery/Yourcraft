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
	//��������
	void CreateGround();
private:
	//����ģ������
	std::shared_ptr<btAxisSweep3> mBroadphase;
	std::shared_ptr<btDefaultCollisionConfiguration> mCollisionConfiguration;
	std::shared_ptr<btCollisionDispatcher> mDispatcher;
	std::shared_ptr<btSequentialImpulseConstraintSolver> mSolver;

	//�������籾��
	std::shared_ptr<btDiscreteDynamicsWorld> mDynamicsWorld;

	//������״
	std::shared_ptr<btCollisionShape> mGroundShape;
	//����transform
	std::shared_ptr<btDefaultMotionState> mGroundMotionState;
	//��������
	std::shared_ptr<btRigidBody> mGroundbody;

};

