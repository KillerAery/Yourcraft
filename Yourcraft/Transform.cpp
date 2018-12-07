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

Transform::~Transform()
{

}

void Transform::Init()
{
	Object::Init();
	mPosition = Vector3(0, 0, 0);
	mWorldPosition = Vector3(0, 0, 0);
	mScale = Vector3(1, 1, 1);
	mWorldScale = Vector3(1, 1, 1);
	mRotation = Vector4(0, 0, 0, 1);
	mWorldRotation = Vector4(0, 0, 0, 1);
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
{	//����ı���λ��/����/��ת�������¼�������任
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
	if(mParent){mWorldPosition = mParent->mWorldPosition + mPosition;}
	else {mWorldPosition = mPosition;}
	PositionChanged();
}

void Transform::SetWorldPosition(const Vector3& pos)
{
	if(mParent) mPosition = pos - mParent->mWorldPosition;
	else mPosition = mWorldPosition;

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
	if (mParent) {mWorldScale = s * mParent->mWorldScale;}
	else { mWorldScale = s; }
	ScaleChanged();
}

void Transform::SetWorldScale(const Vector3& s)
{
	if (mParent)mScale = s / mParent->mWorldScale;
	else mScale = s;

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

void Transform::SetRotation(const Vector4 & quaternion)
{
	mRotation = quaternion;
	if (mParent) { mWorldRotation = quaternion * mParent->mWorldRotation; }
	else { mWorldRotation = quaternion; }
	RotationChanged();
}

void Transform::SetWorldRotation(const Vector4 & quaternion)
{
	if (mParent)mRotation = quaternion / mParent->mWorldRotation;
	else mRotation = quaternion;

	mWorldRotation = quaternion;
	RotationChanged();
}

const Vector4 & Transform::GetRotation()
{
	return mRotation;
}

const Vector4 & Transform::GetWorldRotation()
{
	return mWorldRotation;
}

void Transform::CaculateWorldMatrix()
{
	//��������任
	DirectX::XMMATRIX matrix = DirectX::XMMatrixScaling(mWorldScale.x,mWorldScale.y,mWorldScale.z)
	*DirectX::XMMatrixRotationQuaternion(mWorldRotation)
	*DirectX::XMMatrixTranslation(mWorldPosition.x,mWorldPosition.y,mWorldPosition.z);
	//�洢����任
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
	//Ҫ��ӵĺ���Ϊ�ջ��߱������и�ĸ �����
	if (child == nullptr || child->mParent)return false;
	//��Ӻ��Ӳ���
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
	//�����������ø�ĸ
	mChildren->mParent = this;
	//���亢���Ǹ��¸�ĸ�������
	mChildren->mParentAlive = this->IsAlive();
	mChildren->mParentEnabled = this->IsEnabled();
	mChildren->SetChildrenIsAliveAndEnabled();
	//��ӵ��º���Ҫ���¼���任����
	mChildren->mWorldPosition = mWorldPosition + mChildren->mPosition;
	mChildren->PositionChanged();
	mChildren->mWorldScale = mWorldScale * mChildren->mScale;
	mChildren->ScaleChanged();
	mChildren->mWorldRotation = mWorldRotation * mChildren->mRotation;
	mChildren->RotationChanged();
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

	//���������ÿչ�ϵ
	child->mNext = child->mLast = child->mParent = nullptr;
	//Ȼ����亢���Ǹ��¸�ĸ�������
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
		//������һ����Ŀ�꣬����ں���
		if (child == itr)return true;
		itr = itr->mNext;
	}
	//��Ȼ�򲻴���
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


//λ�����Է����ı�
void Transform::PositionChanged()
{
	mChanged = true;
	//���к��ӵ�λ������Ҳ�ᷢ���ı� 
	auto itr = mChildren;
	while (itr != nullptr)
	{
		//���º��ӵ�λ������
		itr->SetWorldPosition(itr->GetPosition() + mWorldPosition);
		itr->PositionChanged();
		itr = itr->mNext;
	}
}

//�������Է����ı�
void Transform::ScaleChanged()
{
	mChanged = true;
	//���к��ӵı任����Ҳ�ᷢ���ı� 
	auto itr = mChildren;
	while (itr != nullptr)
	{	//���º��ӵ���������
		itr->SetWorldScale(itr->GetScale() * mWorldScale);
		itr->ScaleChanged();
		itr = itr->mNext;
	}
}


//��ת���Է����ı�
void Transform::RotationChanged()
{
	mChanged = true;
	//���к��ӵ���ת����Ҳ�ᷢ���ı� 
	auto itr = mChildren;
	while (itr != nullptr)
	{	//���º��ӵ���ת����
		itr->SetWorldRotation(itr->GetRotation() * mWorldRotation);
		itr->RotationChanged();
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

