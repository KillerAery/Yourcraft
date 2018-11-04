#pragma once
#include "d3dUtil.h"
#include "Object.h"

using namespace DirectX::SimpleMath;

class Transform : public Object
{
public:
	Transform();
	virtual void Init();
	virtual ~Transform();
	virtual bool IsAlive();
	virtual bool IsEnabled();
	void BecomeRoot();

	void SetPosition(const Vector3& pos);
	void SetWorldPosition(const Vector3& pos);
	const Vector3& GetPosition();
	const Vector3& GetWorldPosition();

	void SetScale(const Vector3& scale);
	void SetWorldScale(const Vector3& scale);
	const Vector3& GetScale();
	const Vector3& GetWorldScale();

	//void SetRotation();
	//void SetWorldRotation();
	//const Vector3& GetRotation();
	//Vector3 GetWorldRotation();

	void CaculateWorldMatrix();
	virtual void Update();

	Transform* GetParent();
	Transform* GetChild();
	std::vector<Transform*> GetChildren();

	DirectX::XMFLOAT4X4& GetWorldMatrix();

	bool AddChild(Transform* child);
	void RemoveChild(Transform* child);
	bool FindChild(Transform* child);
	void SetParent(Transform* parent);
private:
	void PositionChanged();
	void ScaleChanged();
	//void RotationChanged();
	void SetChildrenIsAliveAndEnabled();
protected:
	Vector3 mPosition;
	Vector3 mScale;
	Vector4 mRotation;
	Vector3 mWorldPosition;
	Vector3 mWorldScale;
	Vector4 mWorldRotation;

	bool mChanged;								// �Ƿ�ı���λ��/����/��ת(���¾�����)
	bool mParentAlive;							// ��ĸ�Ƿ���
	bool mParentEnabled;						// ��ĸ�Ƿ���
	DirectX::XMFLOAT4X4 mWorldMatrix;			// �������

	Transform* mParent;
	Transform* mLast;
	Transform* mNext;
	Transform* mChildren;
};

