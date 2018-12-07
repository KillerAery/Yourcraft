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
:D3DApp(hInstance){

}

EngineApp::~EngineApp()
{
}

bool EngineApp::Init()
{
	if (!D3DApp::Init())
		return false;

	// 务必先初始化所有渲染状态，以供下面的特效使用
	RenderStates::InitAll(md3dDevice);

	if (!mBasicEffect.InitAll(md3dDevice))
		return false;

	if (!mSkyEffect.InitAll(md3dDevice))
		return false;

	if (!InitResource())
		return false;

	// 初始化鼠标，键盘不需要
	mMouse->SetWindow(mhMainWnd);
	mMouse->SetMode(DirectX::Mouse::MODE_ABSOLUTE);
	
	//初始化物理世界

	mPhysicsWorld.Init();

	// ---------- 测试初始化部分 ---------------------
	Factory::SetPhysicsWorld(&mPhysicsWorld);
	Factory::SetPool(&mGameObjectPool);
	Factory::SetPool(&mBatchMeshRenderPool);
	Factory::SetPool(&mMeshRenderPool);
	Factory::SetPool(&mRigidbodyPool);
	Factory::SetPool(&mSkyRenderPool);
	Factory::SetDevice(md3dDevice);
	Factory::SetDeviceContext(md3dImmediateContext);


	//世界对象
	GameObject* world = Factory::CreateGameObject();
	world->BecomeRoot();

	//批量示例渲染组件
	mObjReader.Read(L"Model\\house.mbo", L"Model\\house.obj");	//房屋模型
	auto br = Factory::CreateBatchMeshRender();
	br->SetModel(Model(md3dDevice, mObjReader));
	
	//10个游戏对象测试
	GameObject* go[10];
	for(int i =0;i<10;++i)
	{
		go[i] = Factory::CreateGameObject();
		go[i]->SetPosition(Vector3(0, 400-80*i, 700+i*12.0f));
		go[i]->SetScale(Vector3(0.2, 0.2, 0.2));
		//mWorld->AddChild(mGameObject[i]);
		//auto meshRender = Factory::CreateMeshRender(mGameObject[i]);
		//meshRender->SetModel(Model(md3dDevice, mObjReader));
		br->BindGameObject(go[i]);

		auto colider = BoxCollider::Create(30.0f,30.0f,30.0f);
		auto pc = Factory::CreateRigidbody(go[i],colider);
	}

	//测试父子对象
	world->AddChild(go[5]);
	world->AddChild(go[7]);
	//mWorld->RemoveChild(mGameObject[3]);
	//mWorld->RemoveChild(mGameObject[4]);

	//测试组件反射
	//auto b = Factory::GetComponent<BatchMeshRender>(mGameObject[5]);
	//b->UnbindGameObject(mGameObject[5]);

	//auto r = Factory::GetComponent<Rigidbody>(mGameObject[4]);
	//r->UnbindGameObject();

	//创建天空对象
	GameObject* sky = Factory::CreateGameObject();
	world->AddChild(sky);
	auto skyrender = Factory::CreateSkyRender(sky,L"Texture\\daylight.jpg",5000.0f);
	auto s = Factory::GetComponent<SkyRender>(sky);
	//s->UnbindGameObject();

	//创建地面对象
	auto ground = Factory::CreateGameObject();
	ground->SetPosition(Vector3(0, -300, 0));
	world->AddChild(ground);
	auto colider = StaticPlaneCollider::Create(0,1,0,1);
	auto groundBody = Factory::CreateRigidbody(ground,colider,0);
	auto groundmesh = Factory::CreateMeshRender(ground);
	groundmesh->SetModel(Model(md3dDevice,MeshData::CreateBox(500,0,3000)));

	return true;
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
	if (mCamera != nullptr)
	{
		mCamera->SetFrustum(XM_PI / 3, AspectRatio(), 1.0f, 1000.0f);
		mCamera->SetViewPort(0.0f, 0.0f, (float)mClientWidth, (float)mClientHeight);
		mBasicEffect.SetProjMatrix(mCamera->GetProjXM());
	}
}


void EngineApp::UpdateScene(float dt)
{
	// 更新鼠标事件，获取相对偏移量
	Mouse::State mouseState = mMouse->GetState();
	Mouse::State lastMouseState = mMouseTracker.GetLastState();
	mMouseTracker.Update(mouseState);
	Keyboard::State keyState = mKeyboard->GetState();
	mKeyboardTracker.Update(keyState);

	// --------- 测试更新部分 -------------//
	mRigidbodyPool.Update();

	mGameObjectPool.Update();
	//物理世界更新
	mPhysicsWorld.StepWorld(dt);
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
	mBasicEffect.SetTextureUsed(false);	// 绘制纹理
	// 网格渲染组件 全部渲染
	mMeshRenderPool.Update(md3dImmediateContext, mBasicEffect);

	//---------- 按实例批量绘制 ----------//
	mBasicEffect.SetRenderDefault(md3dImmediateContext, BasicEffect::RenderInstance);
	mBasicEffect.SetTextureUsed(true);	// 绘制纹理
	// 批量网格渲染组件 全部渲染
	mBatchMeshRenderPool.Update(md3dImmediateContext, mBasicEffect);

	//--------- 绘制天空盒 --------------//
	mSkyEffect.SetRenderDefault(md3dImmediateContext);
	mSkyRenderPool.Update(md3dImmediateContext, mSkyEffect,*mCamera);

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

bool EngineApp::InitResource()
{
	// ******************
	// 初始化游戏对象
	//

	// ******************
	// 初始化摄像机
	//
	mCameraMode = CameraMode::FirstPerson;
	auto camera = std::make_shared<FirstPersonCamera>();
	mCamera = camera;
	camera->SetViewPort(0.0f, 0.0f, (float)mClientWidth, (float)mClientHeight);
	camera->SetFrustum(XM_PI / 3, AspectRatio(), 1.0f, 1000.0f);
	camera->LookTo(
		XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f),
		XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	// 初始化并更新观察矩阵、投影矩阵(摄像机将被固定)
	camera->UpdateViewMatrix();
	mBasicEffect.SetViewMatrix(camera->GetViewXM());
	mBasicEffect.SetProjMatrix(camera->GetProjXM());


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

