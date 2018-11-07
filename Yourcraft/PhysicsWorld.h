#pragma once
#include <memory>
#include "Bullet/inc/btBulletDynamicsCommon.h"

class PhysicsWorld
{
public:
	PhysicsWorld();
	~PhysicsWorld();

	void Init();

	//物理模拟步长
	void StepWorld(float dt);
	btDiscreteDynamicsWorld* GetWorld()const;
private:
	//创建地面
	void CreateGround();
private:
	//物理模拟所需
	std::shared_ptr<btAxisSweep3> mBroadphase;
	std::shared_ptr<btDefaultCollisionConfiguration> mCollisionConfiguration;
	std::shared_ptr<btCollisionDispatcher> mDispatcher;
	std::shared_ptr<btSequentialImpulseConstraintSolver> mSolver;

	//物理世界本体
	std::shared_ptr<btDiscreteDynamicsWorld> mDynamicsWorld;

	//地面形状
	std::shared_ptr<btCollisionShape> mGroundShape;
	//地面transform
	std::shared_ptr<btDefaultMotionState> mGroundMotionState;
	//地面身体
	std::shared_ptr<btRigidBody> mGroundbody;

};

