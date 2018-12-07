#pragma once

#include "Collider.h"
#include <BulletCollision/CollisionShapes/btStaticPlaneShape.h>



class StaticPlaneCollider :
	public Collider
{
public:
	StaticPlaneCollider(float planeNormalX, float planeNormalY, float planeNormalZ, float planeConstant);
	~StaticPlaneCollider();

	static ColliderPtr Create(float planeNormalX,float planeNormalY,float planeNormalZ,float planeConstant);
protected:
	float mNormalX;
	float mNormalY;
	float mNormalZ;
	float mPlaneConstant;
};

