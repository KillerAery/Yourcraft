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

	Ref<Transform> GetParent();
	Ref<Transform> GetChild();
	std::vector<Ref<Transform>> GetChildren();

	DirectX::XMFLOAT4X4& GetWorldMatrix();

	void AddChild(Ref<Transform> child);
	void SetParent(Ref<Transform> parent);
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

	bool mChanged;								// �Ƿ�ı���λ��/����/��ת(���¾�����)
	DirectX::XMFLOAT4X4 mWorldMatrix;			// �������

	Ref<Transform> mParent;
	Ref<Transform> mNext;
	Ref<Transform> mChildren;
};

