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
	ComPtr<ID2D1SolidColorBrush> mColorBrush;				// 单色笔刷
	ComPtr<IDWriteFont> mFont;								// 字体
	ComPtr<IDWriteTextFormat> mTextFormat;					// 文本格式


	BasicEffect mBasicEffect;								// 对象渲染特效管理
	SkyEffect mSkyEffect;									// 天空渲染特效管理
	ParticleEffect mFireEffect;								// 火焰粒子特效
	ParticleEffect mRainEffect;								// 雨滴粒子特效

	Camera* rMainCamera;									// 主体摄像机
	ObjReader mObjReader;									// 模型读取对象

	PhysicsWorld mPhysicsWorld;								// 物理世界

	TextruePool mTextPool;
	ObjectPool<GameObject,MAX_GAMEOBJECT_NUM> mGameObjectPool;
	ObjectPool<BatchMeshRender,10> mBatchMeshRenderPool;
	ObjectPool<MeshRender, 100> mMeshRenderPool;
	ObjectPool<Rigidbody, 100> mRigidbodyPool;
	ObjectPool<SkyRender, 1> mSkyRenderPool;
	ObjectPool<ParticleSystem, 20> mParticleSystemPool;
	ObjectPool<Camera,3> mCameraPool;

	//10个游戏对象测试
	GameObject* go[10];
};

