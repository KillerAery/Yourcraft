#include "BoxCollider.h"


BoxCollider::BoxCollider(float halfwidth, float halfheigth, float halfdepth)
	:mWidth(halfwidth),
	mHeight(halfheigth),
	mDepth(halfdepth)
{
	//创建长方体形状
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

