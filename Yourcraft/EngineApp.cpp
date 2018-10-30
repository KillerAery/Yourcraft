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

	Factory::SetGameObjectPool(&mGameObjectPool);

	return true;
}

void EngineApp::UpdateScene(float dt)
{
	GameApp::UpdateScene(dt);

	//auto i = Factory::CreateGameObject();
	//auto b = i;
	//auto c = i;
	//auto d = b;
	//d = c;
	//c = c;
	//Ref<GameObject> s[100];
	//for(int j = 0 ;j<50;++j)
	//s[j] = Factory::CreateGameObject();
	//mGameObjectPool.Update();

	auto i = std::make_shared<GameObject>();
	auto b = i;
	auto c = i;
	auto d = b;
	d = c;
	c = c;
	std::shared_ptr<GameObject> s[100];
	for (int j = 0; j < 50; ++j)
		s[j] = std::make_shared<GameObject>();


}

void EngineApp::DrawScene()
{
	GameApp::DrawScene();
}
