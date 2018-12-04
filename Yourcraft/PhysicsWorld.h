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
	btAxisSweep3* mBroadphase;
	btDefaultCollisionConfiguration* mCollisionConfiguration;
	btCollisionDispatcher* mDispatcher;
	btSequentialImpulseConstraintSolver* mSolver;
	//物理世界本体
	btDiscreteDynamicsWorld* mDynamicsWorld;
	//地面形状
	btCollisionShape* mGroundShape;
	//地面transform
	btDefaultMotionState* mGroundMotionState;
	//地面身体
	btRigidBody* mGroundbody;

};

