#include "Transform.h"



Transform::Transform():
mPosition(0,0,0),mWorldPosition(0,0,0),
mScale(1,1,1),mWorldScale(1,1,1),
mRotation(0,0,0,0),mWorldRotation(0,0,0,0),
mWorldMatrix(
	1.0f, 0.0f, 0.0f, 0.0f,
	0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f),
mParent(nullptr),
mNext(nullptr),
mChildren(nullptr),
mChanged(false)
{

}

void Transform::Init()
{
	Object::Init();
	mPosition = Vector3(0, 0, 0);
	mWorldPosition = Vector3(0, 0, 0);
	mScale = Vector3(1, 1, 1);
	mWorldScale = Vector3(1, 1, 1);
	mRotation = Vector4(0, 0, 0, 0);
	mWorldRotation = Vector4(0, 0, 0, 0);
	mWorldMatrix = DirectX::XMFLOAT4X4(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
	mChanged = false;
	mParent = nullptr;
	mNext = nullptr;
	mChildren = nullptr;
}

Transform::~Transform()
{

}

void Transform::SetPosition(const Vector3 & pos)
{
	mPosition = pos;
	mWorldPosition = mParent->mWorldPosition + mPosition;
	PositionChanged();
}

void Transform::SetWorldPosition(const Vector3& pos)
{
	mPosition = pos - mParent->mWorldPosition;
	mWorldPosition = pos;
	PositionChanged();
}

const Vector3& Transform::GetPosition()
{
	return mPosition;
}

const Vector3& Transform::GetWorldPosition()
{
	return mWorldPosition;
}

void Transform::SetScale(const Vector3& s)
{
	mScale = s;
	mWorldScale = s * mParent->mWorldScale;
	ScaleChanged();
}

void Transform::SetWorldScale(const Vector3& s)
{
	mScale = s / mParent->mWorldScale;
	mWorldScale = s;
	ScaleChanged();
}

const Vector3& Transform::GetScale()
{
	return mScale;
}

const Vector3& Transform::GetWorldScale()
{
	return mWorldScale;
}

void Transform::CaculateWorldMatrix()
{
	//计算世界变换
	DirectX::XMMATRIX matrix = DirectX::XMMatrixScaling(mWorldScale.x,mWorldScale.y,mWorldScale.z)
	*DirectX::XMMatrixRotationQuaternion(mWorldRotation)
	*DirectX::XMMatrixTranslation(mWorldPosition.x,mWorldPosition.y,mWorldPosition.z);
	//存储世界变换
	DirectX::XMStoreFloat4x4(&mWorldMatrix,matrix);
}

void Transform::Update()
{	//如果改变了位置/缩放/旋转，则重新计算世界变换
	if (mChanged)
	{
		CaculateWorldMatrix();
		mChanged = false;
	}
}

Ref<Transform> Transform::GetParent()
{
	return mParent;
}

Ref<Transform> Transform::GetChild()
{
	return mChildren;
}

std::vector<Ref<Transform>> Transform::GetChildren()
{
	std::vector<Ref<Transform>> vec;

	auto itr = mChildren;
	while(itr != nullptr)
	{
		vec.push_back(itr);
		itr = itr->mNext;
	}

	return vec;
}

DirectX::XMFLOAT4X4& Transform::GetWorldMatrix()
{
	return mWorldMatrix;
}

void Transform::AddChild(Ref<Transform> child)
{
	//孩子为空则滚粗
	if (child == nullptr)return;

	if(mChildren == nullptr)
	{
		mChildren = child;
	}
	else
	{
		auto itr = mChildren;
		while (itr->mNext != nullptr)
		{
			itr = itr->mNext;
		}
		itr->mNext = child;
	}
	//添加的新孩子要重新计算变换属性
	child->PositionChanged();
	child->ScaleChanged();
	//child->RotateChanged();
}

void Transform::SetParent(Ref<Transform> parent)
{
	mParent = parent;
}

//位置属性发生改变
void Transform::PositionChanged()
{
	mChanged = true;
	//所有孩子的位置属性也会发生改变 
	auto itr = mChildren;
	while (itr != nullptr)
	{
		//更新孩子的位置属性
		itr->SetWorldPosition(itr->GetPosition() + mWorldPosition);
		itr->PositionChanged();
		itr = itr->mNext;
	}
}

//缩放属性发生改变
void Transform::ScaleChanged()
{
	mChanged = true;
	//所有孩子的变换属性也会发生改变 
	auto itr = mChildren;
	while (itr != nullptr)
	{	//更新孩子的缩放属性
		itr->SetWorldScale(itr->GetScale() + mWorldScale);
		itr->ScaleChanged();
		itr = itr->mNext;
	}
}

////旋转属性发生改变
//void Transform::RotationChanged()
//{
//	mChanged = true;
//	//所有孩子的旋转属性也会发生改变 
//	auto itr = mChildren;
//	while (itr != nullptr)
//	{	//更新孩子的旋转属性
//		itr->SetWorldRotation(itr->GetPosition()  mWorldPosition);
//		itr->RotationChanged();
//		itr = itr->mNext;
//	}
//}

