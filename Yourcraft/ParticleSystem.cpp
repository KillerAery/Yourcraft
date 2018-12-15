#include "ParticleSystem.h"
#include "Particle.h"
#include "ComponentHelper.h"

using namespace DirectX;

ParticleSystem::ParticleSystem()
	: mInitVB(0), mDrawVB(0), mStreamOutVB(0), mTexArraySRV(0)
{
	mFirstRun = true;
	mGameTime = 0.0f;
	mTimeStep = 0.0f;
	mAge = 0.0f;

	mEyePosW = Vector3(0.0f, 0.0f, 0.0f);
	mEmitPosW = Vector3(0.0f, 0.0f, 0.0f);
	mEmitDirW = Vector3(0.0f, 1.0f, 0.0f);
}

ParticleSystem::~ParticleSystem()
{

}

void ParticleSystem::Init(ComPtr<ID3D11Device> device, ParticleEffect * effect, ComPtr<ID3D11ShaderResourceView> tex2DArray, UINT maxParticles)
{
	Component::Init();

	mMaxParticles = maxParticles;
	mEffect = effect;
	mTexArraySRV = tex2DArray;

	BuildVB(device);
}

void ParticleSystem::BindGameObject(GameObject * gameObject)
{
	if (mGameObject) {
		UnbindGameObject();
	}
	if (gameObject) {
		mGameObject = gameObject;
		mGameObject->AddComponentInfor(static_cast<int>(ComponentType::ParticleSystem), mIndex);
	}
}

void ParticleSystem::UnbindGameObject()
{
	if (mGameObject) {
		mGameObject->RemoveComponentInfor(static_cast<int>(ComponentType::ParticleSystem));
		mGameObject = nullptr;
	}
}


void ParticleSystem::SetEyePos(const Vector3 & eyePosW)
{
	mEyePosW = eyePosW;
}

void ParticleSystem::SetEmitPos(const Vector3 & emitPosW)
{
	mEmitPosW = emitPosW;
}

void ParticleSystem::SetEmitDir(const Vector3 & emitDirW)
{
	mEmitDirW = emitDirW;
}

void ParticleSystem::Reset()
{
	mFirstRun = true;
	mAge = 0.0f;
}

void ParticleSystem::Update(float dt, float gameTime)
{
	mGameTime = gameTime;
	mTimeStep = dt;
	mAge += dt;
}

void ParticleSystem::Draw(ComPtr<ID3D11DeviceContext> dc, const Camera & cam)
{
	UINT stride = sizeof(Particle);
	UINT offset = 0;

	//
	// Set constants.
	//
	mEffect->SetViewProj(cam.GetViewProjXM());
	mEffect->SetGameTime(mGameTime);
	mEffect->SetTimeStep(mTimeStep);
	mEffect->SetEyePosW(cam.GetPositionXM());
	mEffect->SetEmitPosW(cam.GetPositionXM());
	mEffect->SetEmitDirW(mEmitDirW);
	mEffect->SetTexArray(mTexArraySRV);
	mEffect->SetAccelW({ -1.0f, -9.8f, 0.0f });
	mEffect->SetLifeTime(3.0f);
	//mEffect->SetRandomTex(mRandomTexSRV);

	//---------SO-------------
	// On the first pass, use the initialization VB.  Otherwise, use
	// the VB that contains the current particle list.
	if (mFirstRun)
		mEffect->SetRenderStreamOut(dc, mInitVB, mStreamOutVB);
	else
		mEffect->SetRenderStreamOut(dc, mDrawVB, mStreamOutVB);

	//
	// Draw the current particle list using stream-out only to update them.  
	// The updated vertices are streamed-out to the target VB. 
	//
	mEffect->Apply(dc);

	if (mFirstRun)
	{
		dc->Draw(1, 0);
		mFirstRun = false;
	}
	else
	{
		dc->DrawAuto();
	}

	// ping-pong the vertex buffers
	std::swap(mDrawVB, mStreamOutVB);

	//DRAW
	mEffect->SetRenderDraw(dc);

	//
	// Draw the updated particle system we just streamed-out. 
	//
	auto vb = mDrawVB.Get();
	dc->IASetVertexBuffers(0, 1, &vb, &stride, &offset);
	mEffect->Apply(dc);
	dc->DrawAuto();
}

void ParticleSystem::BuildVB(ComPtr<ID3D11Device> device)
{
//
// Create the buffer to kick-off the particle system.
//
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(Particle) * 1;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	// The initial particle emitter has type 0 and age 0.  The rest
	// of the particle attributes do not apply to an emitter.
	Particle p;
	ZeroMemory(&p, sizeof(Particle));
	p.age = 0.0f;
	p.type = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &p;

	HR(device->CreateBuffer(&vbd, &vinitData, &mInitVB));

	//
	// Create the ping-pong buffers for stream-out and drawing.
	//
	vbd.ByteWidth = sizeof(Particle) * mMaxParticles;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;

	HR(device->CreateBuffer(&vbd, 0, &mDrawVB));
	HR(device->CreateBuffer(&vbd, 0, &mStreamOutVB));
}



