#pragma once

#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>



enum class PhysicsShape{

};

struct PhysicsMaterial
{
	btScalar mass;	//√‹∂»
	btVector3 inertia;	//πﬂ–‘
};

class PhysicsHelper
{
	using RigidBodyInfor = btRigidBody::btRigidBodyConstructionInfo;
	//btSphereShape;
	//btBoxShape;
	//btCapsuleShape;
	//btCylinderShape;
	//btStaticPlaneShape;
	static RigidBodyInfor Create();

};