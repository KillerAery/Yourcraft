#include "Camera.h"
#include "ComponentHelper.h"

using namespace DirectX;

Camera::Camera():mChanged(false)
{
}

Camera::~Camera()
{
}

void Camera::Init()
{
}

void Camera::Update()
{
	//只有游戏对象改变了，才去更新矩阵
	if(mGameObject->HasChanged() || mChanged)
		UpdateViewMatrix();
}

void Camera::BindGameObject(GameObject * gameObject)
{
	if (mGameObject) {
		UnbindGameObject();
	}
	if (gameObject) {
		mGameObject = gameObject;
		mGameObject->AddComponentInfor(static_cast<int>(ComponentType::Camera), mIndex);
		UpdateViewMatrix();
	}
}

void Camera::UnbindGameObject()
{
	if (mGameObject) {
		mGameObject->RemoveComponentInfor(static_cast<int>(ComponentType::Camera));
		mGameObject = nullptr;
	}
}

DirectX::XMVECTOR Camera::GetPositionXM() const{
	return mGameObject->GetPosition();
}

DirectX::XMFLOAT3 Camera::GetPosition() const{
	return mGameObject->GetPosition();
}

DirectX::XMVECTOR Camera::GetRightXM() const
{
	return XMLoadFloat3(&mRight);
}

DirectX::XMFLOAT3 Camera::GetRight() const
{
	return mRight;
}

DirectX::XMVECTOR Camera::GetUpXM() const
{
	return XMLoadFloat3(&mUp);
}

DirectX::XMFLOAT3 Camera::GetUp() const
{
	return mUp;
}

DirectX::XMVECTOR Camera::GetLookXM() const
{
	return XMLoadFloat3(&mLook);
}

DirectX::XMFLOAT3 Camera::GetLook() const
{
	return mLook;
}

float Camera::GetNearWindowWidth() const
{
	return mAspect * mNearWindowHeight;
}

float Camera::GetNearWindowHeight() const
{
	return mNearWindowHeight;
}

float Camera::GetFarWindowWidth() const
{
	return mAspect * mFarWindowHeight;
}

float Camera::GetFarWindowHeight() const
{
	return mFarWindowHeight;
}

DirectX::XMMATRIX Camera::GetViewXM() const
{
	return XMLoadFloat4x4(&mView);
}

DirectX::XMMATRIX Camera::GetProjXM() const
{
	return XMLoadFloat4x4(&mProj);
}

DirectX::XMMATRIX Camera::GetViewProjXM() const
{
	return XMLoadFloat4x4(&mView) * XMLoadFloat4x4(&mProj);
}

D3D11_VIEWPORT Camera::GetViewPort() const
{
	return mViewPort;
}

void Camera::SetFrustum(float fovY, float aspect, float nearZ, float farZ)
{
	mFovY = fovY;
	mAspect = aspect;
	mNearZ = nearZ;
	mFarZ = farZ;

	mNearWindowHeight = 2.0f * mNearZ * tanf(0.5f * mFovY);
	mFarWindowHeight = 2.0f * mFarZ * tanf(0.5f * mFovY);

	XMStoreFloat4x4(&mProj, XMMatrixPerspectiveFovLH(mFovY, mAspect, mNearZ, mFarZ));

	mChanged = true;
}

void Camera::SetViewPort(const D3D11_VIEWPORT & viewPort)
{
	mViewPort = viewPort;

	mChanged = true;
}

void Camera::SetViewPort(float topLeftX, float topLeftY, float width, float height, float minDepth, float maxDepth)
{
	mViewPort.TopLeftX = topLeftX;
	mViewPort.TopLeftY = topLeftY;
	mViewPort.Width = width;
	mViewPort.Height = height;
	mViewPort.MinDepth = minDepth;
	mViewPort.MaxDepth = maxDepth;

	mChanged = true;
}


void Camera::LookAt(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR target, DirectX::FXMVECTOR up)
{
	LookTo(pos, target - pos, up);
}

void Camera::LookAt(const DirectX::XMFLOAT3 & pos, const DirectX::XMFLOAT3 & target,const DirectX::XMFLOAT3 & up)
{
	LookAt(XMLoadFloat3(&pos), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

void Camera::LookTo(DirectX::FXMVECTOR pos, DirectX::FXMVECTOR to, DirectX::FXMVECTOR up)
{
	XMVECTOR L = XMVector3Normalize(to);
	XMVECTOR R = XMVector3Normalize(XMVector3Cross(up, L));
	XMVECTOR U = XMVector3Cross(L, R);

	mGameObject->SetWorldPosition(pos);
	XMStoreFloat3(&mLook, L);
	XMStoreFloat3(&mRight, R);
	XMStoreFloat3(&mUp, U);

	mChanged = true;
}

void Camera::LookTo(const DirectX::XMFLOAT3 & pos, const DirectX::XMFLOAT3 & to, const DirectX::XMFLOAT3 & up)
{
	LookTo(XMLoadFloat3(&pos), XMLoadFloat3(&to), XMLoadFloat3(&up));
}

void Camera::Strafe(float d)
{
	XMVECTOR Pos = XMLoadFloat3(&mGameObject->GetPosition());
	XMVECTOR Right = XMLoadFloat3(&mRight);
	XMVECTOR Dist = XMVectorReplicate(d);
	// DestPos = Dist * Right + SrcPos
	mGameObject->SetWorldPosition(XMVectorMultiplyAdd(Dist, Right, Pos));

	mChanged = true;
}

void Camera::Walk(float d)
{
	XMVECTOR Pos = XMLoadFloat3(&mGameObject->GetPosition());
	XMVECTOR Right = XMLoadFloat3(&mRight);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	XMVECTOR Front = XMVector3Normalize(XMVector3Cross(Right, Up));
	XMVECTOR Dist = XMVectorReplicate(d);
	// DestPos = Dist * Front + SrcPos
	mGameObject->SetWorldPosition(XMVectorMultiplyAdd(Dist, Front, Pos));

	mChanged = true;
}

void Camera::MoveForward(float d)
{
	XMVECTOR Pos = XMLoadFloat3(&mGameObject->GetPosition());
	XMVECTOR Look = XMLoadFloat3(&mLook);
	XMVECTOR Dist = XMVectorReplicate(d);
	// DestPos = Dist * Look + SrcPos
	mGameObject->SetWorldPosition(XMVectorMultiplyAdd(Dist, Look, Pos));

	mChanged = true;
}

void Camera::Pitch(float rad)
{
	XMMATRIX R = XMMatrixRotationAxis(XMLoadFloat3(&mRight), rad);
	XMVECTOR Up = XMVector3TransformNormal(XMLoadFloat3(&mUp), R);
	XMVECTOR Look = XMVector3TransformNormal(XMLoadFloat3(&mLook), R);
	float cosPhi = XMVectorGetY(Look);
	// 将上下视野角度Phi限制在[2pi/9, 7pi/9]，
	// 即余弦值[-cos(2pi/9), cos(2pi/9)]之间
	if (fabs(cosPhi) > cosf(XM_2PI / 9))
		return;
	
	XMStoreFloat3(&mUp, Up);
	XMStoreFloat3(&mLook, Look);

	mChanged = true;
}

void Camera::RotateY(float rad)
{
	XMMATRIX R = XMMatrixRotationY(rad);

	XMStoreFloat3(&mRight, XMVector3TransformNormal(XMLoadFloat3(&mRight), R));
	XMStoreFloat3(&mUp, XMVector3TransformNormal(XMLoadFloat3(&mUp), R));
	XMStoreFloat3(&mLook, XMVector3TransformNormal(XMLoadFloat3(&mLook), R));

	mChanged = true;
}

void Camera::UpdateViewMatrix()
{
	mChanged = false;

	XMVECTOR R = XMLoadFloat3(&mRight);
	XMVECTOR U = XMLoadFloat3(&mUp);
	XMVECTOR L = XMLoadFloat3(&mLook);
	XMVECTOR P = XMLoadFloat3(&mGameObject->GetPosition());

	// 保持摄像机的轴互为正交，且长度都为1
	L = XMVector3Normalize(L);
	U = XMVector3Normalize(XMVector3Cross(L, R));

	// U, L已经正交化，需要计算对应叉乘得到R
	R = XMVector3Cross(U, L);

	// 填充观察矩阵
	float x = -XMVectorGetX(XMVector3Dot(P, R));
	float y = -XMVectorGetX(XMVector3Dot(P, U));
	float z = -XMVectorGetX(XMVector3Dot(P, L));

	XMStoreFloat3(&mRight, R);
	XMStoreFloat3(&mUp, U);
	XMStoreFloat3(&mLook, L);

	mView = {
		mRight.x, mUp.x, mLook.x, 0.0f,
		mRight.y, mUp.y, mLook.y, 0.0f,
		mRight.z, mUp.z, mLook.z, 0.0f,
		x, y, z, 1.0f
	};
}
