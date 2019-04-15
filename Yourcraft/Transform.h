#pragma once
#include "d3dUtil.h"
#include "Object.h"

using namespace DirectX::SimpleMath;

class Transform : public Object
{
public:
	Transform();
	~Transform();
	void Init();
	void Update();
	bool IsAlive();
	bool IsEnabled();
	void AddRefCount();
	void ReduceRefCount();

	void BecomeRoot();

	void SetPosition(const Vector3& pos);
	void SetWorldPosition(const Vector3& pos);
	const Vector3& GetPosition();
	const Vector3& GetWorldPosition();

	void SetScale(const Vector3& scale);
	void SetWorldScale(const Vector3& scale);
	const Vector3& GetScale();
	const Vector3& GetWorldScale();

	void SetRotation(const Vector4& quaternion);
	void SetWorldRotation(const Vector4& quaternion);
	const Vector4& GetRotation();
	const Vector4& GetWorldRotation();

	void CaculateWorldMatrix();

	Transform* GetParent();
	Transform* GetChild();
	std::vector<Transform*> GetChildren();

	DirectX::XMFLOAT4X4& GetWorldMatrix();

	bool AddChild(Transform* child);
	void RemoveChild(Transform* child);
	bool FindChild(Transform* child);
	void SetParent(Transform* parent);
	bool HasChanged();
private:
	void WorldTransformChanged();
	void SetChildrenIsAliveAndEnabled();
protected:
	Vector3 mPosition;
	Vector3 mScale;
	Vector4 mRotation;

	bool mChanged;								// 是否改变了位置/伸缩/旋转(更新世界变换矩阵用)
	bool mParentAlive;							// 父母是否存活
	bool mParentEnabled;						// 父母是否开启
	DirectX::XMFLOAT4X4 mWorldMatrix;			// 世界矩阵

	Transform* mParent;
	Transform* mLast;
	Transform* mNext;
	Transform* mChildren;
};

