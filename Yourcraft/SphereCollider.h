#pragma once
#include <memory>
#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include "Collider.h"

class SphereCollider :
	public Collider
{
public:
	SphereCollider(float radius = 1.0f);
	~SphereCollider();
	static ColliderPtr Create(float radius = 1.0f);
protected:
	float mRadius;
};

