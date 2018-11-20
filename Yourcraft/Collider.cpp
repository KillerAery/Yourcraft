#include "Collider.h"



Collider::Collider():mShape(nullptr)
{
}


Collider::~Collider()
{
	if (mShape)
	{
		delete mShape;
		mShape = nullptr;
	}
}

btCollisionShape* Collider::GetShape() const
{
	return mShape;
}
