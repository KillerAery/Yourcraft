#pragma once
#include "GameApp.h"
#include "TextruePool.h"
#include "ObjectPool.h"
#include "GameObject.h"

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
};

