#include <VertexTypes.h>
#include "Effects.h"
#include "d3dUtil.h"
#include "EffectHelper.h"
#include "ParticleSystem.h"
#include "Particle.h"

using namespace DirectX;
using namespace std::experimental;

struct CBPerFrame
{
	DirectX::XMFLOAT3 gEyePosW;
	DirectX::XMFLOAT3 gEmitPosW;
	DirectX::XMFLOAT3 gEmitDirW;
	DirectX::XMMATRIX gViewProj;
	float gGameTime;
	float gTimeStep;
};

struct CBParticleSystemChanged
{
	//加速度 默认{ -1.0f, -9.8f, 0.0f };
	DirectX::XMFLOAT3 gAccelW;
	//粒子生命周期
	float gLifeTime;
};

class ParticleEffect::Impl : public AlignedType<ParticleEffect::Impl>
{
public:
	// 必须显式指定
	Impl() = default;
	~Impl() = default;
public:
	ComPtr<ID3D11ShaderResourceView> TexArray;		//2D纹理 数组，用于描绘若干个粒子
	ComPtr<ID3D11ShaderResourceView> RandomTex;		//1D纹理，用于取随机数

	CBufferObject<0, CBPerFrame>cbFrame;					// 每帧的常量缓冲区
	CBufferObject<1, CBParticleSystemChanged> cbParticleSystemChanged;// 每次随粒子系统改变而改变的常量缓冲区

	BOOL isDirty;											// 是否有值变更
	std::vector<CBufferBase*> cBufferPtrs;					// 统一管理上面所有的常量缓冲区

	ComPtr<ID3D11VertexShader> streamOutVS;
	ComPtr<ID3D11GeometryShader> streamOutGS;
	ComPtr<ID3D11VertexShader>  drawVS;
	ComPtr<ID3D11GeometryShader> drawGS;
	ComPtr<ID3D11PixelShader> drawPS;

	ComPtr<ID3D11InputLayout> particleLayout;		// 输入布局
};

//
// ParticleEffect
//

ParticleEffect::ParticleEffect()
{
	pImpl = std::make_unique<ParticleEffect::Impl>();
}

ParticleEffect::~ParticleEffect()
{

}

bool ParticleEffect::InitAll(ComPtr<ID3D11Device> device , const std::wstring& hlslname) {
	if (!device)
		return false;

	if (!pImpl->cBufferPtrs.empty())
		return true;

	ComPtr<ID3DBlob> blob;

	// 随机textrue
	SetRandomTex(CreateRandomTexture1DSRV(device));
	// 默认生命时间3
	SetLifeTime(3);

	// -----------StreamOut-------------------//
	//
	HR(CreateShaderFromFile(L"HLSL\\Particle_StreamOut_VS.cso", L"HLSL\\Particle_StreamOut_VS.hlsl", "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()));
	HR(device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, pImpl->streamOutVS.GetAddressOf()));

	HR(device->CreateInputLayout(Particle::InputElements, ARRAYSIZE(Particle::InputElements),
		blob->GetBufferPointer(), blob->GetBufferSize(), pImpl->particleLayout.GetAddressOf()));

	HR(CreateShaderFromFile(L"HLSL\\Particle_StreamOut_GS.cso", L"HLSL\\Particle_StreamOut_GS.hlsl", "GS", "gs_5_0", blob.ReleaseAndGetAddressOf()));

	UINT stride = sizeof(Particle);
	const D3D11_SO_DECLARATION_ENTRY particalLayout[5] = {
		{0, "POSITION",	 0, 0, 3, 0 },
		{0, "VELOCITY",	 0, 0, 3, 0 },
		{0, "SIZE",		 0, 0, 2, 0 },
		{0, "AGE",		 0, 0, 1, 0 },
		{0, "TYPE",		 0, 0, 1, 0 },
	};

	HR(device->CreateGeometryShaderWithStreamOutput(blob->GetBufferPointer(), blob->GetBufferSize(), particalLayout, ARRAYSIZE(particalLayout),
		&stride, 1, D3D11_SO_NO_RASTERIZED_STREAM, nullptr, pImpl->streamOutGS.GetAddressOf()));

	// -----------Draw-------------------//
	//
	HR(CreateShaderFromFile((L"HLSL\\" + hlslname + L"_Draw_VS.cso").c_str(), (L"HLSL\\" + hlslname + L"_Draw_VS.hlsl").c_str(), "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()));
	HR(device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, pImpl->drawVS.GetAddressOf()));

	HR(device->CreateInputLayout(Particle::InputElements, ARRAYSIZE(Particle::InputElements),
		blob->GetBufferPointer(), blob->GetBufferSize(), pImpl->particleLayout.GetAddressOf()));

	HR(CreateShaderFromFile((L"HLSL\\" + hlslname + L"_Draw_GS.cso").c_str(), (L"HLSL\\" + hlslname + L"_Draw_GS.hlsl").c_str(), "GS", "gs_5_0", blob.ReleaseAndGetAddressOf()));
	HR(device->CreateGeometryShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, pImpl->drawGS.GetAddressOf()));

	HR(CreateShaderFromFile((L"HLSL\\" + hlslname + L"_Draw_PS.cso").c_str(), (L"HLSL\\" + hlslname + L"_Draw_PS.hlsl").c_str(), "PS", "ps_5_0", blob.ReleaseAndGetAddressOf()));
	HR(device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, pImpl->drawPS.GetAddressOf()));

	// 初始化
	RenderStates::InitAll(device);

	pImpl->cBufferPtrs.assign({
		&pImpl->cbFrame,
		&pImpl->cbParticleSystemChanged
		});

	// 创建常量缓冲区
	for (auto& pBuffer : pImpl->cBufferPtrs)
	{
		pBuffer->CreateBuffer(device);
	}

	return true;
}

void ParticleEffect::SetRenderStreamOut(ComPtr<ID3D11DeviceContext> deviceContext, ComPtr<ID3D11Buffer> vertexBufferIn, ComPtr<ID3D11Buffer> vertexBufferOut) {

	// 先恢复流输出默认设置，防止顶点缓冲区同时绑定在输入和输出阶段
	UINT stride = sizeof(Particle);
	UINT offset = 0;
	ID3D11Buffer * nullBuffer = nullptr;
	deviceContext->SOSetTargets(1, &nullBuffer, &offset);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	deviceContext->IASetInputLayout(pImpl->particleLayout.Get());

	deviceContext->IASetVertexBuffers(0, 1, vertexBufferIn.GetAddressOf(), &stride, &offset);

	deviceContext->VSSetShader(pImpl->streamOutVS.Get(), nullptr, 0);
	deviceContext->GSSetShader(pImpl->streamOutGS.Get(), nullptr, 0);

	deviceContext->GSSetSamplers(0, 1, RenderStates::SSLinearWrap.GetAddressOf());
	deviceContext->SOSetTargets(1, vertexBufferOut.GetAddressOf(), &offset);
	
	deviceContext->RSSetState(nullptr);
	deviceContext->PSSetShader(nullptr, nullptr, 0);
}

void ParticleEffect::SetRenderDraw(ComPtr<ID3D11DeviceContext> deviceContext){
	deviceContext->IASetInputLayout(pImpl->particleLayout.Get());
	deviceContext->VSSetShader(pImpl->drawVS.Get(), nullptr, 0);
	// 关闭流输出
	UINT offset = 0;
	ID3D11Buffer * nullBuffer = nullptr;
	deviceContext->SOSetTargets(1, &nullBuffer, &offset);

	deviceContext->GSSetShader(pImpl->drawGS.Get(), nullptr, 0);
	deviceContext->RSSetState(nullptr);
	deviceContext->PSSetShader(pImpl->drawPS.Get(), nullptr, 0);

	deviceContext->PSSetSamplers(0, 1, RenderStates::SSLinearWrap.GetAddressOf());

	deviceContext->OMSetDepthStencilState(RenderStates::DSSNoDepthWrite.Get(), 0);
	//deviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
}

void ParticleEffect::SetViewProj(DirectX::FXMMATRIX M) {
	auto& cBuffer = pImpl->cbFrame;
	cBuffer.data.gViewProj = M;
	pImpl->isDirty = cBuffer.isDirty = true;
}

void ParticleEffect::SetGameTime(float f) {
	auto& cBuffer = pImpl->cbFrame;
	cBuffer.data.gGameTime = f;
	pImpl->isDirty = cBuffer.isDirty = true;
}

void ParticleEffect::SetTimeStep(float f) {
	auto& cBuffer = pImpl->cbFrame;
	cBuffer.data.gTimeStep = f;
	pImpl->isDirty = cBuffer.isDirty = true;
}

void ParticleEffect::SetEyePosW(DirectX::FXMVECTOR v) {
	auto& cBuffer = pImpl->cbFrame;
	DirectX::XMStoreFloat3(&cBuffer.data.gEyePosW,v);
	pImpl->isDirty = cBuffer.isDirty = true;
}

void ParticleEffect::SetEmitPosW(DirectX::FXMVECTOR v) {
	auto& cBuffer = pImpl->cbFrame;
	DirectX::XMStoreFloat3(&cBuffer.data.gEmitPosW, v);
	pImpl->isDirty = cBuffer.isDirty = true;
}

void ParticleEffect::SetEmitDirW(DirectX::FXMVECTOR v) {
	auto& cBuffer = pImpl->cbFrame;
	DirectX::XMStoreFloat3(&cBuffer.data.gEmitDirW, v);
	pImpl->isDirty = cBuffer.isDirty = true;
}

void ParticleEffect::SetAccelW(DirectX::FXMVECTOR v) {
	auto& cBuffer = pImpl->cbParticleSystemChanged;
	DirectX::XMStoreFloat3(&cBuffer.data.gAccelW, v);
	pImpl->isDirty = cBuffer.isDirty = true;
}

void ParticleEffect::SetLifeTime(float lifetime) {
	auto& cBuffer = pImpl->cbParticleSystemChanged;
	cBuffer.data.gLifeTime = lifetime;
	pImpl->isDirty = cBuffer.isDirty = true;
}

void ParticleEffect::SetTexArray(ComPtr<ID3D11ShaderResourceView> tex) {
	pImpl->TexArray = tex;
}
void ParticleEffect::SetRandomTex(ComPtr<ID3D11ShaderResourceView> tex) {
	pImpl->RandomTex = tex;
}

void ParticleEffect::Apply(ComPtr<ID3D11DeviceContext> deviceContext) {
	auto& pCBuffers = pImpl->cBufferPtrs;
	// 将缓冲区绑定到渲染管线上
	pCBuffers[0]->BindVS(deviceContext);
	pCBuffers[1]->BindVS(deviceContext);

	pCBuffers[0]->BindGS(deviceContext);
	pCBuffers[1]->BindGS(deviceContext);

	// 设置SRV
	deviceContext->GSSetShaderResources(0, 1, pImpl->TexArray.GetAddressOf());
	deviceContext->GSSetShaderResources(1, 1, pImpl->RandomTex.GetAddressOf());

	deviceContext->PSSetShaderResources(0, 1, pImpl->TexArray.GetAddressOf());
	deviceContext->PSSetShaderResources(1, 1, pImpl->RandomTex.GetAddressOf());

	if (pImpl->isDirty)
	{
		pImpl->isDirty = false;
		for (auto& pCBuffer : pCBuffers)
		{
			pCBuffer->UpdateBuffer(deviceContext);
		}
	}
}