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
	//��������任
	DirectX::XMMATRIX matrix = DirectX::XMMatrixScaling(mWorldScale.x,mWorldScale.y,mWorldScale.z)
	*DirectX::XMMatrixRotationQuaternion(mWorldRotation)
	*DirectX::XMMatrixTranslation(mWorldPosition.x,mWorldPosition.y,mWorldPosition.z);
	//�洢����任
	DirectX::XMStoreFloat4x4(&mWorldMatrix,matrix);
}

void Transform::Update()
{	//����ı���λ��/����/��ת�������¼�������任
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

	//���������ÿչ�ϵ
	child->mNext = child->mLast = child->mParent = nullptr;
	//Ȼ����亢���Ǹ��¸�ĸ�������
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

////��ת���Է����ı�
//void Transform::RotationChanged()
//{
//	mChanged = true;
//	//���к��ӵ���ת����Ҳ�ᷢ���ı� 
//	auto itr = mChildren;
//	while (itr != nullptr)
//	{	//���º��ӵ���ת����
//		itr->SetWorldRotation(itr->GetPosition()  mWorldPosition);
//		itr->RotationChanged();
//		itr = itr->mNext;
//	}
//}

