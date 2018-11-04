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
mChanged(false),
mParent(nullptr),
mParentAlive(false),
mParentEnabled(false),
mLast(nullptr),
mNext(nullptr),
mChildren(nullptr)
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
	mParentAlive = false;
	mParentEnabled = false;
	mLast = nullptr;
	mNext = nullptr;
	mChildren = nullptr;
}

Transform::~Transform()
{

}

bool Transform::IsAlive()
{
	return Object::IsAlive() || mParentAlive;
}

bool Transform::IsEnabled()
{
	return Object::IsEnabled() && mParentEnabled;
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

Transform* Transform::GetParent()
{
	return mParent;
}

Transform* Transform::GetChild()
{
	return mChildren;
}

std::vector<Transform*> Transform::GetChildren()
{
	std::vector<Transform*> vec;

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

bool Transform::AddChild(Transform* child)
{
	//要添加的孩子为空或者本来就有父母 则滚粗
	if (child == nullptr || child->mParent)return false;
	//添加孩子操作
	if(mChildren == nullptr)
	{
		mChildren = child;
	}
	else
	{
		auto temp = mChildren;
		mChildren = child;
		mChildren->mNext = temp;
		temp->mLast = mChildren;
	}
	//最后给孩子设置父母
	mChildren->mParent = this;
	//给其孩子们更新父母存活属性
	mChildren->mParentAlive = this->IsAlive();
	mChildren->mParentEnabled = this->IsEnabled();
	mChildren->SetChildrenIsAliveAndEnabled();
	//添加的新孩子要重新计算变换属性
	mChildren->PositionChanged();
	mChildren->ScaleChanged();
	//child->RotateChanged();
	return true;
}

void Transform::RemoveChild(Transform* child)
{
	if(child->mLast){
		child->mLast->mNext = child->mNext;
	}
	else{
		this->mChildren = child->mLast;
	}

	//给孩子设置空关系
	child->mNext = child->mLast = child->mParent = nullptr;
	//然后给其孩子们更新父母存活属性
	mChildren->mParentAlive = this->IsAlive();
	mChildren->mParentEnabled = this->IsEnabled();
	child->SetChildrenIsAliveAndEnabled();
}

bool Transform::FindChild(Transform* child)
{
	if(!child)return false;
	auto itr = mChildren;
	while (itr != nullptr)
	{
		//若其中一项是目标，则存在孩子
		if (child == itr)return true;
		itr = itr->mNext;
	}
	//不然则不存在
	return false;
}


void Transform::SetParent(Transform* parent)
{
	if(mParent){
		mParent->RemoveChild(this);
	}
	if(parent)
	{
		parent->AddChild(this);
	}
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
		itr->SetWorldScale(itr->GetScale() * mWorldScale);
		itr->ScaleChanged();
		itr = itr->mNext;
	}
}

void Transform::SetChildrenIsAliveAndEnabled(){
	bool isParentAlive = this->IsAlive();
	bool isParentEnabled = this->IsEnabled();
	auto itr = mChildren;
	while (itr != nullptr)
	{
		itr->mParentAlive = isParentAlive;
		itr->mParentEnabled = isParentEnabled;
		itr->SetChildrenIsAliveAndEnabled();
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

