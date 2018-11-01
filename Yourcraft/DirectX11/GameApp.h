#ifndef GAMEAPP_H
#define GAMEAPP_H

#include <DirectXColors.h>
#include "d3dApp.h"
#include "Camera.h"
#include "TestObject.h"
#include "ObjReader.h"
#include "Collision.h"

class GameApp : public D3DApp
{
public:
	// �����ģʽ
	enum class CameraMode { FirstPerson, ThirdPerson, Free };
	
public:
	GameApp(HINSTANCE hInstance);
	~GameApp();

	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();

private:
	bool InitResource();
	
protected:
	
	ComPtr<ID2D1SolidColorBrush> mColorBrush;				// ��ɫ��ˢ
	ComPtr<IDWriteFont> mFont;								// ����
	ComPtr<IDWriteTextFormat> mTextFormat;					// �ı���ʽ

	
	//TestObject mSphere;										// ��
	//TestObject mCube;										// ������
	//TestObject mCylinder;									// Բ����
	//TestObject mHouse;										// ����
	//DirectX::BoundingSphere mBoundingSphere;				// ��İ�Χ��

	std::wstring mPickedObjStr;								// �Ѿ�ʰȡ�Ķ�����

	BasicEffect mBasicEffect;								// ������Ⱦ��Ч����

	std::shared_ptr<Camera> mCamera;						// �����
	CameraMode mCameraMode;									// �����ģʽ

	ObjReader mObjReader;									// ģ�Ͷ�ȡ����
};


#endif