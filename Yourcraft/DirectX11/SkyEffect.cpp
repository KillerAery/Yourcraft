#include "Effects.h"
#include "d3dUtil.h"
#include "EffectHelper.h"	// ��������Effects.h��d3dUtil.h����
#include <VertexTypes.h>

using namespace DirectX;
using namespace std::experimental;

//
// ��Щ�ṹ���ӦHLSL�Ľṹ�壬�������ļ�ʹ�á���Ҫ��16�ֽڶ���
//

struct CBChangesEveryFrame
{
	DirectX::XMMATRIX worldViewProj;
};


//
// SkyEffect::Impl ��Ҫ����SkyEffect�Ķ���
//

class SkyEffect::Impl : public AlignedType<SkyEffect::Impl>
{
public:
	// ������ʽָ��
	Impl() = default;
	~Impl() = default;

public:
	CBufferObject<0, CBChangesEveryFrame>	cbFrame;	// ÿ֡���Ƶĳ���������

	BOOL isDirty;										// �Ƿ���ֵ���
	std::vector<CBufferBase*> cBufferPtrs;				// ͳһ�����������еĳ���������

	ComPtr<ID3D11VertexShader> skyVS;
	ComPtr<ID3D11PixelShader> skyPS;

	ComPtr<ID3D11InputLayout> vertexPosLayout;

	ComPtr<ID3D11ShaderResourceView> textureCube;			// ��պ�����
};

//
// SkyEffect
//

namespace
{
	// SkyEffect����
	static SkyEffect * pInstance = nullptr;
}

SkyEffect::SkyEffect()
{
	if (pInstance)
		throw std::exception("BasicEffect is a singleton!");
	pInstance = this;
	pImpl = std::make_unique<SkyEffect::Impl>();
}

SkyEffect::~SkyEffect()
{
}

SkyEffect::SkyEffect(SkyEffect && moveFrom)
{
	pImpl.swap(moveFrom.pImpl);
}

SkyEffect & SkyEffect::operator=(SkyEffect && moveFrom)
{
	pImpl.swap(moveFrom.pImpl);
	return *this;
}

SkyEffect & SkyEffect::Get()
{
	if (!pInstance)
		throw std::exception("BasicEffect needs an instance!");
	return *pInstance;
}

bool SkyEffect::InitAll(ComPtr<ID3D11Device> device)
{
	if (!device)
		return false;

	if (!pImpl->cBufferPtrs.empty())
		return true;

	ComPtr<ID3DBlob> blob;
	
	// ******************
	// ����������ɫ��
	//

	HR(CreateShaderFromFile(L"HLSL\\Sky_VS.vso", L"HLSL\\Sky_VS.hlsl", "VS", "vs_5_0", blob.ReleaseAndGetAddressOf()));
	HR(device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, pImpl->skyVS.GetAddressOf()));
	// �������㲼��
	HR(device->CreateInputLayout(DirectX::VertexPosition::InputElements, ARRAYSIZE(DirectX::VertexPosition::InputElements),
		blob->GetBufferPointer(), blob->GetBufferSize(), pImpl->vertexPosLayout.GetAddressOf()));

	// ******************
	// ����������ɫ��
	//

	HR(CreateShaderFromFile(L"HLSL\\Sky_PS.pso", L"HLSL\\Sky_PS.hlsl", "PS", "ps_5_0", blob.ReleaseAndGetAddressOf()));
	HR(device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, pImpl->skyPS.GetAddressOf()));

	// ��ʼ��
	RenderStates::InitAll(device);

	pImpl->cBufferPtrs.assign({
		&pImpl->cbFrame,
	});

	// ��������������
	for (auto& pBuffer : pImpl->cBufferPtrs)
	{
		pBuffer->CreateBuffer(device);
	}

	return true;
}

void SkyEffect::SetRenderDefault(ComPtr<ID3D11DeviceContext> deviceContext)
{
	deviceContext->IASetInputLayout(pImpl->vertexPosLayout.Get());
	deviceContext->VSSetShader(pImpl->skyVS.Get(), nullptr, 0);
	deviceContext->PSSetShader(pImpl->skyPS.Get(), nullptr, 0);

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	deviceContext->GSSetShader(nullptr, nullptr, 0);
	deviceContext->RSSetState(RenderStates::RSNoCull.Get());

	deviceContext->PSSetSamplers(0, 1, RenderStates::SSLinearWrap.GetAddressOf());
	deviceContext->OMSetDepthStencilState(RenderStates::DSSLessEqual.Get(), 0);
	deviceContext->OMSetBlendState(nullptr, nullptr, 0xFFFFFFFF);
}

void XM_CALLCONV SkyEffect::SetWorldViewProjMatrix(DirectX::FXMMATRIX W, DirectX::CXMMATRIX V, DirectX::CXMMATRIX P)
{
	auto& cBuffer = pImpl->cbFrame;
	cBuffer.data.worldViewProj = XMMatrixTranspose(W * V * P);
	pImpl->isDirty = cBuffer.isDirty = true;
}

void XM_CALLCONV SkyEffect::SetWorldViewProjMatrix(DirectX::FXMMATRIX WVP)
{
	auto& cBuffer = pImpl->cbFrame;
	cBuffer.data.worldViewProj = XMMatrixTranspose(WVP);
	pImpl->isDirty = cBuffer.isDirty = true;
}

void SkyEffect::SetTextureCube(ComPtr<ID3D11ShaderResourceView> textureCube)
{
	pImpl->textureCube = textureCube;
}

void SkyEffect::Apply(ComPtr<ID3D11DeviceContext> deviceContext)
{
	auto& pCBuffers = pImpl->cBufferPtrs;
	// ���������󶨵���Ⱦ������
	pCBuffers[0]->BindVS(deviceContext);
	
	// ����SRV
	deviceContext->PSSetShaderResources(0, 1, pImpl->textureCube.GetAddressOf());

	if (pImpl->isDirty)
	{
		pImpl->isDirty = false;
		for (auto& pCBuffer : pCBuffers)
		{
			pCBuffer->UpdateBuffer(deviceContext);
		}
	}
}

