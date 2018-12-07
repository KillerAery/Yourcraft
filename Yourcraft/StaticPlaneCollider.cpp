#include "StaticPlaneCollider.h"


StaticPlaneCollider::StaticPlaneCollider(float planeNormalX, float planeNormalY, float planeNormalZ, float planeConstant):
	mNormalX(planeNormalX),
	mNormalY(planeNormalY),
	mNormalZ(planeNormalZ),
	mPlaneConstant(planeConstant)
{
	mShape = new btStaticPlaneShape(btVector3(planeNormalX, planeNormalY, planeNormalZ), planeConstant);
}

StaticPlaneCollider::~StaticPlaneCollider()
{
	if (mShape)
	{
		delete mShape;
		mShape = nullptr;
	}
}

ColliderPtr StaticPlaneCollider::Create(float planeNormalX, float planeNormalY, float planeNormalZ, float planeConstant)
{
	return std::make_shared<StaticPlaneCollider>(planeNormalX, planeNormalY, planeNormalZ, planeConstant);
}
