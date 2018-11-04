#include "EngineApp.h"
#include "Factory.h"

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


	mWorld = Factory::CreateGameObject();

	mGameObject = Factory::CreateGameObject();
	mMeshRender = Factory::CreateMeshRender(mGameObject);

	mWorld->AddChild(mGameObject);
	mGameObject->SetParent(mWorld);
	mGameObject->SetPosition(Vector3(0,-100,700));
	mGameObject->SetScale(Vector3(2,1,1));

	//房屋
	mObjReader.Read(L"Model\\house.mbo", L"Model\\house.obj");
	mMeshRender->SetModel(Model(md3dDevice, mObjReader));

	return true;
}

void EngineApp::UpdateScene(float dt)
{
	GameApp::UpdateScene(dt);
	mGameObjectPool.Update();
}

void EngineApp::DrawScene()
{
	GameApp::DrawScene();

	assert(md3dImmediateContext);
	assert(mSwapChain);

	// ******************
	// 绘制Direct3D部分
	//
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView.Get(), reinterpret_cast<const float*>(&DirectX::Colors::Black));
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//--------- 按对象绘制 ----------//
	mBasicEffect.SetRenderDefault(md3dImmediateContext, BasicEffect::RenderObject);

	// 绘制纹理
	mBasicEffect.SetTextureUsed(true);
	// 网格渲染组件 全部渲染
	mMeshRenderPool.Update(md3dImmediateContext, mBasicEffect);

	//---------- 按实例批量绘制 ----------//
	mBasicEffect.SetRenderDefault(md3dImmediateContext, BasicEffect::RenderInstance);


	// ******************
	// 绘制Direct2D部分
	//
	md2dRenderTarget->BeginDraw();
	std::wstring text = L"当前拾取物体: " + mPickedObjStr;

	md2dRenderTarget->DrawTextW(text.c_str(), (UINT32)text.length(), mTextFormat.Get(),
		D2D1_RECT_F{ 0.0f, 0.0f, 600.0f, 200.0f }, mColorBrush.Get());
	HR(md2dRenderTarget->EndDraw());

	HR(mSwapChain->Present(0, 0));

}
