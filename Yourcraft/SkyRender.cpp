#include "SkyRender.h"
#include "ComponentHelper.h"
#include "MeshData.h"

using namespace DirectX;
using namespace Microsoft::WRL;

SkyRender::SkyRender()
{
}


SkyRender::~SkyRender()
{
}

void SkyRender::Init(GameObject * gameObject, ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, const std::wstring & cubemapFilename, float skySphereRadius, bool generateMips)
{
	Render::Init(gameObject);

	// ��պ��������
	if (cubemapFilename.substr(cubemapFilename.size() - 3) == L"dds")
	{
		HR(CreateDDSTextureFromFile(
			device.Get(),
			cubemapFilename.c_str(),
			nullptr,
			mTextureCubeSRV.GetAddressOf()
		));
	}
	else
	{
		mTextureCubeSRV = CreateWICTextureCubeFromFile(
			device,
			deviceContext,
			cubemapFilename,
			generateMips
		);
	}
	InitResource(device, skySphereRadius);
}

void SkyRender::Init(GameObject * gameObject, ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, const std::vector<std::wstring>& cubemapFilenames, float skySphereRadius, bool generateMips)
{
	Render::Init(gameObject);

	// ��պ��������
	mTextureCubeSRV = CreateWICTextureCubeFromFile(
		device,
		deviceContext,
		cubemapFilenames,
		generateMips
	);

	InitResource(device, skySphereRadius);
}

void SkyRender::BindGameObject(GameObject * gameObject)
{
	if (mGameObject) {
		UnbindGameObject();
	}
	if (gameObject) {
		mGameObject = gameObject;
		mGameObject->AddComponentInfor(static_cast<int>(ComponentType::SkyRender),mIndex);
	}
}

void SkyRender::UnbindGameObject()
{
	if (mGameObject) {
		mGameObject->RemoveComponentInfor(static_cast<int>(ComponentType::SkyRender));
		mGameObject = nullptr;
	}
}

void SkyRender::Update(ComPtr<ID3D11DeviceContext> deviceContext, SkyEffect & skyEffect, const Camera & camera)
{
	Draw(deviceContext, skyEffect, camera);
}

void SkyRender::InitResource(ComPtr<ID3D11Device> device, float skySphereRadius)
{
	auto sphere = MeshData::CreateSphere(skySphereRadius);
	size_t size = sphere.vertexVec.size();
	std::vector<XMFLOAT3> vertices(size);
	for (size_t i = 0; i < size; ++i)
	{
		vertices[i] = sphere.vertexVec[i].position;
	}

	// ���㻺��������
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(XMFLOAT3) * (UINT)vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = vertices.data();

	HR(device->CreateBuffer(&vbd, &InitData, &mVertexBuffer));

	// ��������������
	mIndexCount = (UINT)sphere.indexVec.size();

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(WORD) * mIndexCount;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.StructureByteStride = 0;
	ibd.MiscFlags = 0;

	InitData.pSysMem = sphere.indexVec.data();

	HR(device->CreateBuffer(&ibd, &InitData, &mIndexBuffer));
}

void SkyRender::Draw(ComPtr<ID3D11DeviceContext> deviceContext, SkyEffect & skyEffect, const Camera & camera)
{
	UINT strides[1] = { sizeof(XMFLOAT3) };
	UINT offsets[1] = { 0 };
	deviceContext->IASetVertexBuffers(0, 1, mVertexBuffer.GetAddressOf(), strides, offsets);
	deviceContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	XMFLOAT3 pos = camera.GetPosition();
	skyEffect.SetWorldViewProjMatrix(XMMatrixTranslation(pos.x, pos.y, pos.z) * camera.GetViewProjXM());
	skyEffect.SetTextureCube(mTextureCubeSRV);
	skyEffect.Apply(deviceContext);
	deviceContext->DrawIndexed(mIndexCount, 0, 0);
}
