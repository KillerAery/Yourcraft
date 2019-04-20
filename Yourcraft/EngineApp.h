#pragma once

#include <DirectXColors.h>
#include "d3dApp.h"
#include "Camera.h"
#include "ObjReader.h"
#include "Effects.h"

#include "Factory.h"
#include "TextruePool.h"
#include "ObjectPool.h"

#include "GameObject.h"
#include "BatchMeshRender.h"
#include "MeshRender.h"
#include "Rigidbody.h"
#include "PhysicsWorld.h"
#include "SkyRender.h"
#include "ParticleSystem.h"
#include "Camera.h"

class EngineApp :public D3DApp
{
public:
	EngineApp(HINSTANCE hInstance);
	 ~EngineApp();

	 bool Init();
	 void InitGameObjects();

	 void UpdateScene(float dt)override;
	 void DrawScene()override;
	 void OnResize();
private:
	bool InitResource();
protected:
	ComPtr<ID2D1SolidColorBrush> mColorBrush;				// ��ɫ��ˢ
	ComPtr<IDWriteFont> mFont;								// ����
	ComPtr<IDWriteTextFormat> mTextFormat;					// �ı���ʽ


	BasicEffect mBasicEffect;								// ������Ⱦ��Ч����
	SkyEffect mSkyEffect;									// �����Ⱦ��Ч����
	ParticleEffect mFireEffect;								// ����������Ч
	ParticleEffect mRainEffect;								// ���������Ч

	Camera* rMainCamera;									// ���������
	ObjReader mObjReader;									// ģ�Ͷ�ȡ����

	PhysicsWorld mPhysicsWorld;								// ��������

	TextruePool mTextPool;
	ObjectPool<GameObject,MAX_GAMEOBJECT_NUM> mGameObjectPool;
	ObjectPool<BatchMeshRender,10> mBatchMeshRenderPool;
	ObjectPool<MeshRender, 100> mMeshRenderPool;
	ObjectPool<Rigidbody, 100> mRigidbodyPool;
	ObjectPool<SkyRender, 1> mSkyRenderPool;
	ObjectPool<ParticleSystem, 20> mParticleSystemPool;
	ObjectPool<Camera,3> mCameraPool;

	//10����Ϸ�������
	GameObject* go[10];
};

