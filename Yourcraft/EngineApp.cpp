#include "EngineApp.h"
#include "Factory.h"
#include <SimpleMath.h>
#include "d3dUtil.h"
#include "SphereCollider.h"

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

	if (!mBasicEffect.InitAll(md3dDevice))
		return false;

	if (!InitResource())
		return false;

	// ��ʼ����꣬���̲���Ҫ
	mMouse->SetWindow(mhMainWnd);
	mMouse->SetMode(DirectX::Mouse::MODE_ABSOLUTE);
	
	//��ʼ����������

	mPhysicsWorld.Init();

	// ---------- ���Գ�ʼ������ ---------------------
	Factory::SetPool(&mGameObjectPool);
	Factory::SetPool(&mBatchMeshRenderPool);
	Factory::SetPool(&mMeshRenderPool);
	Factory::SetPool(&mRigidbodyPool);

	mWorld = Factory::CreateGameObject();
	mWorld->BecomeRoot();

	mObjReader.Read(L"Model\\house.mbo", L"Model\\house.obj");	//����

	auto br = Factory::CreateBatchMeshRender(nullptr);
	br->SetModel(Model(md3dDevice, mObjReader));

	for(int i =0;i<10;++i)
	{
		mGameObject[i] = Factory::CreateGameObject();
		mGameObject[i]->SetPosition(Vector3(0, 400-80*i, 700));
		mGameObject[i]->SetScale(Vector3(0.2, 0.2, 0.2));
		mWorld->AddChild(mGameObject[i]);
		mMeshRender = Factory::CreateMeshRender(mGameObject[i]);
		mMeshRender->SetModel(Model(md3dDevice, mObjReader));
		//br->BindGameObject(mGameObject[i]);
	}

	auto i = Factory::GetComponent<MeshRender>(mGameObject[4]);
	i->BindGameObject(nullptr);

	auto colider = SphereCollider::Create();
	auto pc = Factory::CreateRigidbody(mGameObject[0], mPhysicsWorld, colider);


	return true;
}

void EngineApp::OnResize()
{
	assert(md2dFactory);
	assert(mdwriteFactory);
	// �ͷ�D2D�������Դ
	mColorBrush.Reset();
	md2dRenderTarget.Reset();

	D3DApp::OnResize();

	// ΪD2D����DXGI������ȾĿ��
	ComPtr<IDXGISurface> surface;
	HR(mSwapChain->GetBuffer(0, __uuidof(IDXGISurface), reinterpret_cast<void**>(surface.GetAddressOf())));
	D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(
		D2D1_RENDER_TARGET_TYPE_DEFAULT,
		D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED));
	HR(md2dFactory->CreateDxgiSurfaceRenderTarget(surface.Get(), &props, md2dRenderTarget.GetAddressOf()));

	surface.Reset();
	// �����̶���ɫˢ���ı���ʽ
	HR(md2dRenderTarget->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::White),
		mColorBrush.GetAddressOf()));
	HR(mdwriteFactory->CreateTextFormat(L"����", nullptr, DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 15, L"zh-cn",
		mTextFormat.GetAddressOf()));

	// ����������ʾ
	if (mCamera != nullptr)
	{
		mCamera->SetFrustum(XM_PI / 3, AspectRatio(), 1.0f, 1000.0f);
		mCamera->SetViewPort(0.0f, 0.0f, (float)mClientWidth, (float)mClientHeight);
		mBasicEffect.SetProjMatrix(mCamera->GetProjXM());
	}
}


void EngineApp::UpdateScene(float dt)
{
	// ��������¼�����ȡ���ƫ����
	Mouse::State mouseState = mMouse->GetState();
	Mouse::State lastMouseState = mMouseTracker.GetLastState();
	mMouseTracker.Update(mouseState);
	Keyboard::State keyState = mKeyboard->GetState();
	mKeyboardTracker.Update(keyState);

	// --------- ���Ը��²��� -------------//
	mGameObjectPool.Update();

	//�����������
	mPhysicsWorld.StepWorld(dt);
}

void EngineApp::DrawScene()
{
	assert(md3dImmediateContext);
	assert(mSwapChain);

	// ******************
	// ����Direct3D����
	//
	md3dImmediateContext->ClearRenderTargetView(mRenderTargetView.Get(), reinterpret_cast<const float*>(&DirectX::Colors::Black));
	md3dImmediateContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	//--------- ��������� ----------//
	mBasicEffect.SetRenderDefault(md3dImmediateContext, BasicEffect::RenderObject);
	mBasicEffect.SetTextureUsed(true);	// ��������
	// ������Ⱦ��� ȫ����Ⱦ
	mMeshRenderPool.Update(md3dImmediateContext, mBasicEffect);

	//---------- ��ʵ���������� ----------//
	mBasicEffect.SetRenderDefault(md3dImmediateContext, BasicEffect::RenderInstance);
	mBasicEffect.SetTextureUsed(true);	// ��������
	// ����������Ⱦ��� ȫ����Ⱦ
	mBatchMeshRenderPool.Update(md3dImmediateContext, mBasicEffect);

	// ******************
	// ����Direct2D����
	//
	md2dRenderTarget->BeginDraw();
	std::wstring text = L"��������";

	md2dRenderTarget->DrawTextW(text.c_str(), (UINT32)text.length(), mTextFormat.Get(),
		D2D1_RECT_F{ 0.0f, 0.0f, 600.0f, 200.0f }, mColorBrush.Get());
	HR(md2dRenderTarget->EndDraw());

	HR(mSwapChain->Present(0, 0));

}

bool EngineApp::InitResource()
{
	// ******************
	// ��ʼ����Ϸ����
	//

	// ******************
	// ��ʼ�������
	//
	mCameraMode = CameraMode::FirstPerson;
	auto camera = std::shared_ptr<FirstPersonCamera>(new FirstPersonCamera);
	mCamera = camera;
	camera->SetViewPort(0.0f, 0.0f, (float)mClientWidth, (float)mClientHeight);
	camera->SetFrustum(XM_PI / 3, AspectRatio(), 1.0f, 1000.0f);
	camera->LookTo(
		XMVectorSet(0.0f, 0.0f, -10.0f, 1.0f),
		XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f),
		XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));
	// ��ʼ�������¹۲����ͶӰ����(����������̶�)
	camera->UpdateViewMatrix();
	mBasicEffect.SetViewMatrix(camera->GetViewXM());
	mBasicEffect.SetProjMatrix(camera->GetProjXM());


	// ******************
	// ��ʼ������仯��ֵ
	//

	// �����
	DirectionalLight dirLight;
	dirLight.Ambient = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	dirLight.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	dirLight.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 16.0f);
	dirLight.Direction = XMFLOAT3(-0.707f, -0.707f, 0.707f);
	mBasicEffect.SetDirLight(0, dirLight);

	return true;
}

