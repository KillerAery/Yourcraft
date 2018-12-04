#include "Collider.h"



Collider::Collider():mShape(nullptr)
{
}


Collider::~Collider()
{

}

btCollisionShape* Collider::GetShape() const
{
	return mShape;
}
