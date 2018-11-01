#include "EngineApp.h"
#include "Factory.h"
#include <memory>

EngineApp::EngineApp(HINSTANCE hInstance):GameApp(hInstance){
}

EngineApp::~EngineApp()
{
}

bool EngineApp::Init()
{
	if (!GameApp::Init())return false;

	Factory::SetPool(&mGameObjectPool);
	Factory::SetPool(&mBatchMeshRenderPool);
	Factory::SetPool(&mMeshRenderPool);


	return true;
}

void EngineApp::UpdateScene(float dt)
{
	GameApp::UpdateScene(dt);

	auto i = Factory::CreateGameObject();
	auto c = Factory::CreateMeshRender(i);


	mGameObjectPool.Update();
	mMeshRenderPool.Update(md3dImmediateContext,mBasicEffect);
}

void EngineApp::DrawScene()
{
	GameApp::DrawScene();
}
