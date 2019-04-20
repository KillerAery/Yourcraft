#pragma once

#include <d3d11_1.h>
#include <SimpleMath.h>
#include "Component.h"

class Camera : public Component
{
public:
	Camera();
	~Camera();

	void Init();
	void Update();

	void BindGameObject(GameObject * gameObject);
	void UnbindGameObject();

	// ��ȡ�����λ��
	DirectX::XMVECTOR GetPositionXM() const;
	DirectX::XMFLOAT3 GetPosition() const;

	// ��ȡ�����������������
	DirectX::XMVECTOR GetRightXM() const;
	DirectX::XMFLOAT3 GetRight() const;
	DirectX::XMVECTOR GetUpXM() const;
	DirectX::XMFLOAT3 GetUp() const;
	DirectX::XMVECTOR GetLookXM() const;
	DirectX::XMFLOAT3 GetLook() const;

	// ��ȡ��׶����Ϣ
	float GetNearWindowWidth() const;
	float GetNearWindowHeight() const;
	float GetFarWindowWidth() const;
	float GetFarWindowHeight() const;

	// ��ȡ����
	DirectX::XMMATRIX GetViewXM() const;
	DirectX::XMMATRIX GetProjXM() const;
	DirectX::XMMATRIX GetViewProjXM() const;

	// ��ȡ�ӿ�
	D3D11_VIEWPORT GetViewPort() const;

	// ������׶��
	void SetFrustum(float fovY, float aspect, float nearZ, float farZ);

	// �����ӿ�
	void SetViewPort(const D3D11_VIEWPORT& viewPort);
	void SetViewPort(float topLeftX, float topLeftY, float width, float height, float minDepth = 0.0f, float maxDepth = 1.0f);

	// ����������ĳ���
	void LookAt(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR up);
	void LookAt(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& target,const DirectX::XMFLOAT3& up);
	void LookTo(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR to, DirectX::FXMVECTOR up);
	void LookTo(const DirectX::XMFLOAT3& pos, const DirectX::XMFLOAT3& to, const DirectX::XMFLOAT3& up);
	// ƽ��
	void Strafe(float d);
	// ֱ��(ƽ���ƶ�)
	void Walk(float d);
	// ǰ��(��ǰ���ƶ�)
	void MoveForward(float d);
	// ���¹۲�
	void Pitch(float rad);
	// ���ҹ۲�
	void RotateY(float rad);
protected:
	// ���¹۲����
	void UpdateViewMatrix();
protected:
	// ������Ĺ۲�ռ�����ϵ��Ӧ����������ϵ�еı�ʾ
	DirectX::XMFLOAT3 mRight;
	DirectX::XMFLOAT3 mUp;
	DirectX::XMFLOAT3 mLook;

	// ��׶������
	float mNearZ;
	float mFarZ;
	float mAspect;
	float mFovY;
	float mNearWindowHeight;
	float mFarWindowHeight;

	// �۲�����͸��ͶӰ����
	DirectX::XMFLOAT4X4 mView;
	DirectX::XMFLOAT4X4 mProj;

	// ��ǰ�ӿ�
	D3D11_VIEWPORT mViewPort;

	bool mChanged;
};