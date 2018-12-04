#include "BoxCollider.h"


BoxCollider::BoxCollider(float halfwidth, float halfheigth, float halfdepth)
	:mWidth(halfwidth),
	mHeight(halfheigth),
	mDepth(halfdepth)
{
	//������������״
	mShape = new btBoxShape(btVector3(halfwidth, halfheigth, halfdepth));
}


BoxCollider::~BoxCollider()
{
	if (mShape)
	{
		delete mShape;
		mShape = nullptr;
	}
}

ColliderPtr BoxCollider::Create(float halfwidth, float halfheigth, float halfdepth)
{
	return std::make_shared<BoxCollider>(halfwidth, halfheigth, halfdepth);
}

