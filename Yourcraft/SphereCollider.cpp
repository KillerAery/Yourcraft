#include "SphereCollider.h"

SphereCollider::SphereCollider(float radius)
	:mRadius(radius)
{
	//������������״
	mShape = new btSphereShape(radius);
}

SphereCollider::~SphereCollider()
{

}

ColliderPtr SphereCollider::Create(float radius)
{
	return std::make_shared<SphereCollider>(radius);
}

