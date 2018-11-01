#include "MeshRender.h"

using namespace DirectX;

MeshRender::MeshRender()
{
}

void MeshRender::Init(CRef<Transform> object)
{
	Render::Init();
	BindTransform(object);
}


MeshRender::~MeshRender()
{
}

void MeshRender::Update(ComPtr<ID3D11DeviceContext> deviceContext, BasicEffect & effect)
{
	UINT strides = sizeof(DirectX::VertexPositionNormalTexture);
	UINT offsets = 0;

	for (auto& part : mModel.modelParts)
	{
		// ���ö���/����������
		deviceContext->IASetVertexBuffers(0, 1, part.vertexBuffer.GetAddressOf(), &strides, &offsets);
		deviceContext->IASetIndexBuffer(part.indexBuffer.Get(), part.indexFormat, 0);

		// �������ݲ�Ӧ��
		effect.SetWorldMatrix(XMLoadFloat4x4(&mTransform->GetWorldMatrix()));
		effect.SetTextureAmbient(part.texA);
		effect.SetTextureDiffuse(part.texD);
		effect.SetMaterial(part.material);
		effect.Apply(deviceContext);

		deviceContext->DrawIndexed(part.indexCount, 0, 0);
	}
}

void MeshRender::BindTransform(CRef<Transform> object)
{
	mTransform = object;
}

void MeshRender::UnbindTransform()
{
	mTransform = nullptr;
}
