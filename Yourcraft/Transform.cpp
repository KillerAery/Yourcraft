#include "Transform.h"

using namespace DirectX;

Transform::Transform():
mPosition(0,0,0),
mScale(1,1,1),
mRotation(0,0,0,0),
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

Transform::~Transform()
{

}

void Transform::Init()
{
	Object::Init();
	mPosition = Vector3(0, 0, 0);
	mScale = Vector3(1, 1, 1);
	mRotation = Vector4(0, 0, 0, 1);
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

void Transform::Update()
{	//如果改变了位置/缩放/旋转，则重新计算世界变换
	if (mChanged)
	{
		CaculateWorldMatrix();
		mChanged = false;
	}
}

bool Transform::IsAlive()
{
	return Object::IsAlive() || mParentAlive;
}

bool Transform::IsEnabled()
{
	return Object::IsEnabled() && mParentEnabled;
}

void Transform::AddRefCount()
{
	Object::AddRefCount();
	SetChildrenIsAliveAndEnabled();
}

void Transform::ReduceRefCount()
{
	Object::ReduceRefCount();
	SetChildrenIsAliveAndEnabled();
}

void Transform::BecomeRoot()
{
	this->mParentEnabled = true;
	this->AddRefCount();
}

void Transform::SetPosition(const Vector3 & pos)
{
	mPosition = pos;
	WorldTransformChanged();
}

void Transform::SetWorldPosition(const Vector3& pos)
{
	if(mParent) mPosition = pos - mParent->GetPosition();
	else mPosition = pos;
	WorldTransformChanged();
}

const Vector3& Transform::GetPosition()
{
	return mPosition;
}

Vector3 Transform::GetWorldPosition()
{
	if (!mParent)return mPosition;
	return mParent->GetWorldPosition() + mPosition;
}

void Transform::SetScale(const Vector3& s)
{
	mScale = s;
	WorldTransformChanged();
}

void Transform::SetWorldScale(const Vector3& s)
{
	if (mParent)mScale = s / mParent->GetScale();
	else mScale = s;
	WorldTransformChanged();
}

const Vector3& Transform::GetScale()
{
	return mScale;
}

Vector3 Transform::GetWorldScale()
{
	if (!mParent)return mScale;
	return mParent->GetWorldScale()*mScale;
}


void Transform::SetRotation(const Vector4 & quaternion)
{
	mRotation = quaternion;
	WorldTransformChanged();
}

void Transform::SetWorldRotation(const Vector4 & quaternion)
{
	if (mParent) {
		auto pm = XMMatrixRotationQuaternion(mParent->GetWorldRotation());
		auto det = XMMatrixDeterminant(pm);
		XMMATRIX m = XMMatrixInverse(&det,pm) * XMMatrixRotationQuaternion(quaternion);
		mRotation = XMQuaternionRotationMatrix(m);
	}
	else {
		mRotation = quaternion;
	}
	WorldTransformChanged();
}

const Vector4 & Transform::GetRotation()
{
	return mRotation;
}

Vector4 Transform::GetWorldRotation()
{
	if (mParent) {
		XMMATRIX m = XMMatrixRotationQuaternion(mRotation) * XMMatrixRotationQuaternion(mParent->GetWorldRotation());
		return XMQuaternionRotationMatrix(m);
	}
	else {
		return mRotation;
	}
}


void Transform::CaculateWorldMatrix()
{
	//计算世界变换
	DirectX::XMMATRIX matrix = DirectX::XMMatrixScaling(mScale.x,mScale.y,mScale.z)
	*DirectX::XMMatrixRotationQuaternion(mRotation)
	*DirectX::XMMatrixTranslation(mPosition.x,mPosition.y,mPosition.z);
	if (mParent)
		matrix = matrix * mParent->GetWorldMatrix();
	//存储世界变换
	DirectX::XMStoreFloat4x4(&mWorldMatrix,matrix);
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
	//要添加的孩子为空或者本来就有父母
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
	mChildren->SetPosition(mChildren->GetPosition());
	mChildren->SetScale(mChildren->GetScale());
	mChildren->SetRotation(mChildren->GetRotation());
	return true;
}

void Transform::RemoveChild(Transform* child)
{
	if (!FindChild(child))return;

	if(child->mLast){
		child->mLast->mNext = child->mNext;
	}
	else{
		this->mChildren = child->mLast;
	}

	//给孩子设置空关系
	child->mNext = child->mLast = child->mParent = nullptr;
	//然后给其孩子们更新父母存活属性
	child->mParentAlive = false;
	child->mParentEnabled = false;
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


bool Transform::HasChanged()
{
	return mChanged;
}


//世界变换发生改变
void Transform::WorldTransformChanged()
{
	mChanged = true;
	//所有孩子的世界变换也会发生改变 
	auto itr = mChildren;
	while (itr != nullptr){
		itr->WorldTransformChanged();
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

