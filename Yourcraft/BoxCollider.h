#pragma once
#include <memory>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include "Collider.h"


class BoxCollider :
	public Collider
{
public:
	BoxCollider(float halfwidth = 0.5f, float halfheigth = 0.5f, float halfdepth = 0.5f);
	~BoxCollider();
	static ColliderPtr Create(float halfwidth = 0.5f, float halfheigth = 0.5f, float halfdepth = 0.5f);
protected:
	float mWidth;
	float mHeight;
	float mDepth;

};


