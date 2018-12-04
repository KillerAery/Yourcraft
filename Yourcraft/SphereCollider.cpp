#include "SphereCollider.h"

SphereCollider::SphereCollider(float radius)
	:mRadius(radius)
{
	//创建长方体形状
	mShape = new btSphereShape(radius);
}

SphereCollider::~SphereCollider()
{
	if (mShape)
	{
		delete mShape;
		mShape = nullptr;
	}
}

ColliderPtr SphereCollider::Create(float radius)
{
	return std::make_shared<SphereCollider>(radius);
}

