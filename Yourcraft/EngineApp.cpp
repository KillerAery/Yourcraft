#include "EngineApp.h"
#include "Factory.h"
#include <SimpleMath.h>
#include "d3dUtil.h"

#include "BoxCollider.h"
#include "SphereCollider.h"
#include "StaticPlaneCollider.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std::experimental;

EngineApp::EngineApp(HINSTANCE hInstance)
:D3DApp(hInstance),rMainCamera(nullptr){

}

EngineApp::~EngineApp()
{
}

bool EngineApp::Init()
{
	if (!D3DApp::Init())
		return false;

	// 游戏对象初始化
	InitGameObjects();

	// 务必先初始化所有渲染状态，以供下面的特效使用
	RenderStates::InitAll(md3dDevice);

	if (!mBasicEffect.InitAll(md3dDevice))	return false;
	if (!mSkyEffect.InitAll(md3dDevice))	return false;
	if (!mRainEffect.InitAll(md3dDevice,L"RainParticle"))	return false;

	if (!InitResource())
		return false;

	return true;
}

void EngineApp::InitGameObjects()
{
	// 初始化鼠标，键盘不需要
	mMouse->SetWindow(mhMainWnd);
	mMouse->SetMode(DirectX::Mouse::MODE_ABSOLUTE);

	// 初始化物理世界
	mPhysicsWorld.Init();

	// 初始化工厂
	Factory::SetPhysicsWorld(&mPhysicsWorld);
	Factory::SetPool(&mGameObjectPool);
	Factory::SetPool(&mBatchMeshRenderPool);
	Factory::SetPool(&mMeshRenderPool);
	Factory::SetPool(&mRigidbodyPool);
	Factory::SetPool(&mSkyRenderPool);
	Factory::SetPool(&mParticleSystemPool);
	Factory::SetPool(&mCameraPool);

	Factory::SetDevice(md3dDevice);
	Factory::SetDeviceContext(md3dImmediateContext);

	//--------------------------------------------------------------//
	// --------------------- 测试初始化部分 ---------------------	//

	// ---------  世界对象 --------- //
	GameObject* world = Factory::CreateGameObject();
	world->BecomeRoot();

	// ------  摄像机对象 ------- //
	GameObject* cameraObj = Factory::CreateGameObject();
	world->AddChild(cameraObj);
	auto camera = Factory::CreateCamera();
	camera->BindGameObject(cameraObj);
	// 初始化摄像机值
	camera->SetViewPort(0.0f, 0.0f, (float)mClientWidth, (float)mClientHeight);
	camera->SetFrustum(XM_PI / 3, AspectRatio(), 1.0f, 1000.0f);
	camera->LookTo(
		XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f),
		XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	// 设置主摄像机
	rMainCamera = camera;


	// -------  天空对象 ------- //
	GameObject* sky = Factory::CreateGameObject();
	world->AddChild(sky);
	auto skyrender = Factory::CreateSkyRender(L"Texture\\daylight.jpg", 5000.0f);
	skyrender->BindGameObject(sky);
	auto s = Factory::GetComponent<SkyRender>(sky);


	// --------  地面对象 -------- //
	auto ground = Factory::CreateGameObject();
	ground->SetPosition(Vector3(0, -300, 0));
	world->AddChild(ground);
	auto colider = StaticPlaneCollider::Create(0, 1, 0, 1);
	auto groundBody = Factory::CreateRigidbody(colider, 0);
	groundBody->BindGameObject(ground);
	auto groundmesh = Factory::CreateMeshRender();
	groundmesh->BindGameObject(ground);
	groundmesh->SetModel(Model(md3dDevice, MeshData::CreateBox(500, 0.1f, 3000)));


	// -----  雨粒子系统对象 ------- //
	auto rain = Factory::CreateGameObject();
	world->AddChild(rain);
	std::vector<std::wstring> raindrops{ L"Texture\\raindrop.dds" };
	auto particleSystem = Factory::CreateParticleSystem(&mRainEffect, raindrops, 10000);
	particleSystem->BindGameObject(rain);


	// ----- 测试若干个游戏对象任意组合组件 ------- //
	//房屋模型
	mObjReader.Read(L"Model\\house.mbo", L"Model\\house.obj");
	auto model = Model(md3dDevice, mObjReader);
	//批量渲染组件
	auto bmr = Factory::CreateBatchMeshRender();
	bmr->SetModel(model);

	for (int i = 0; i < 10; ++i)
	{
		go[i] = Factory::CreateGameObject();
		go[i]->SetPosition(Vector3(0, 400 - 80 * i, 700 + i * 12.0f));
		go[i]->SetScale(Vector3(0.2, 0.2, 0.2));
		world->AddChild(go[i]);
		//auto meshRender = Factory::CreateMeshRender();
		//meshRender->SetModel(model);
		//meshRender->BindGameObject(go[i]);
		bmr->BindGameObject(go[i]);

		auto gcolider = BoxCollider::Create(30.0f, 30.0f, 30.0f);
		auto rb = Factory::CreateRigidbody(gcolider);
		rb->BindGameObject(go[i]);
	}

	//测试父子对象
	Factory::GetComponent<Rigidbody>(go[7])->UnbindGameObject();
	world->RemoveChild(go[7]);
	go[3]->AddChild(go[7]);
	go[7]->SetScale(Vector3(3, 1, 1));
	go[7]->SetPosition(Vector3(0, 0, 0));

	world->RemoveChild(go[4]);

	//auto tc1 = Factory::GetComponent<Rigidbody>(go[5]);
	//auto tc2 = Factory::GetComponent<Rigidbody>(go[7]);
	//tc2->UnbindGameObject();
	//mWorld->RemoveChild(mGameObject[3]);
	//mWorld->RemoveChild(mGameObject[4]);

	//测试组件反射
	//auto b = Factory::GetComponent<BatchMeshRender>(mGameObject[5]);
	//b->UnbindGameObject(mGameObject[5]);

	//auto r = Factory::GetComponent<Rigidbody>(mGameObject[4]);
	//r->UnbindGameObject();
}


void EngineApp::UpdateScene(float dt)
{
	// 更新鼠标事件，获取相对偏移量
	Mouse::State mouseState = mMouse->GetState();
	Mouse::State lastMouseState = mMouseTracker.GetLastState();
	mMouseTracker.Update(mouseState);
	Keyboard::State keyState = mKeyboard->GetState();
	mKeyboardTracker.Update(keyState);

	// ------------------- 测试更新部分 ------------------------//
	mParticleSystemPool.Update(mTimer.GetDeltaTime(), mTimer.TotalTime());
	// -------- 摄像机更新 ----------//
	//rMainCamera->RotateY(0.000005f);	//测试旋转
	mCameraPool.Update();

	//物理世界更新
	mPhysicsWorld.StepWorld(dt);

	mRigidbodyPool.Update();

	mGameObjectPool.Update();
}

void EngineApp::DrawScene()
{
	assert(md3dImmediateContext);
	assert(mSwapChain);

	// ******************
	// 绘制Direct3D部分
	//
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView.Get(), reinterpret_cast<const float*>(&DirectX::Colors::Black));
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//--------- 按对象绘制 ----------//
	mBasicEffect.SetRenderDefault(md3dImmediateContext, BasicEffect::RenderObject);
	// 网格渲染组件 全部渲染
	mMeshRenderPool.Draw(md3dImmediateContext, mBasicEffect, rMainCamera);

	//---------- 按实例批量绘制 ----------//
	mBasicEffect.SetRenderDefault(md3dImmediateContext, BasicEffect::RenderInstance);
	// 批量网格渲染组件 全部渲染
	mBatchMeshRenderPool.Draw(md3dImmediateContext, mBasicEffect, rMainCamera);

	//--------- 绘制天空盒 --------------//
	mSkyEffect.SetRenderDefault(md3dImmediateContext);
	mSkyRenderPool.Draw(md3dImmediateContext, mSkyEffect, rMainCamera);

	//--------- 绘制粒子系统 ---------------//
	mParticleSystemPool.Draw(md3dImmediateContext, rMainCamera);

	// ******************
	// 绘制Direct2D部分
	//
	md2dRenderTarget->BeginDraw();
	std::wstring text = L"测试引擎";

	md2dRenderTarget->DrawTextW(text.c_str(), (UINT32)text.length(), mTextFormat.Get(),
		D2D1_RECT_F{ 0.0f, 0.0f, 600.0f, 200.0f }, mColorBrush.Get());
	HR(md2dRenderTarget->EndDraw());

	HR(mSwapChain->Present(0, 0));
}


void EngineApp::OnResize()
{
	assert(md2dFactory);
	assert(mdwriteFactory);
	// 释放D2D的相关资源
	mColorBrush.Reset();
	md2dRenderTarget.Reset();

	D3DApp::OnResize();

	// 为D2D创建DXGI表面渲染目标
	ComPtr<IDXGISurface> surface;
	HR(mSwapChain->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(surface.GetAddressOf())));
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
	HR(md2dFactory->CreateDxgiSurfaceRenderTarget(surface.Get(), &props, md2dRenderTarget.GetAddressOf()));

	surface.Reset();
	// 创建固定颜色刷和文本格式
	HR(md2dRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::White),
		mColorBrush.GetAddressOf()));
	HR(mdwriteFactory->CreateTextFormat(L"宋体", nullptr, DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 15, L"zh-cn",
		mTextFormat.GetAddressOf()));

	// 摄像机变更显示
	if (rMainCamera != nullptr)
	{
		rMainCamera->SetFrustum(XM_PI / 3, AspectRatio(), 1.0f, 1000.0f);
		rMainCamera->SetViewPort(0.0f, 0.0f, (float)mClientWidth, (float)mClientHeight);
		mBasicEffect.SetProjMatrix(rMainCamera->GetProjXM());
	}
}


bool EngineApp::InitResource()
{
	// ******************
	// 初始化主摄像机
	//

	// 若主摄像机为空，则不初始化渲染
	if (!rMainCamera) {
		return false;
	}

	// 更新摄像机
	rMainCamera->Update();

	// ******************
	// 初始化不会变化的值
	//

	// 方向光
	DirectionalLight dirLight;
	dirLight.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	dirLight.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	dirLight.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 16.0f);
	dirLight.Direction = XMFLOAT3(-0.707f, -0.707f, 0.707f);
	mBasicEffect.SetDirLight(0, dirLight);

	return true;
}

