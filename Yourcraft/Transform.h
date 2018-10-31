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

	void AddChild(Transform* child);
private:
	void PositionChanged();
	void ScaleChanged();
	//void RotationChanged();
protected:
	Vector3 mPosition;
	Vector3 mScale;
	Vector4 mRotation;
	Vector3 mWorldPosition;
	Vector3 mWorldScale;
	Vector4 mWorldRotation;

	bool mChanged;								// 是否改变了位置/伸缩/旋转(更新矩阵用)
	DirectX::XMFLOAT4X4 mWorldMatrix;			// 世界矩阵

	Transform* mParent;
	Transform* mNext;
	Transform* mChildren;
};

