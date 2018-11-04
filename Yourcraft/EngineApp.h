#pragma once
#include "GameApp.h"
#include "TextruePool.h"
#include "ObjectPool.h"
#include "GameObject.h"
#include "BatchMeshRender.h"
#include "MeshRender.h"

class EngineApp :public GameApp
{
public:
	EngineApp(HINSTANCE hInstance);
	 ~EngineApp();
	 bool Init();
	 void UpdateScene(float dt)override;
	 void DrawScene()override;
private:
	TextruePool mTextPool;
	ObjectPool<GameObject,500> mGameObjectPool;
	ObjectPool<BatchMeshRender,10> mBatchMeshRenderPool;
	ObjectPool<MeshRender, 100> mMeshRenderPool;

	GameObject* mWorld;
	GameObject* mGameObject;
	MeshRender* mMeshRender;
};

