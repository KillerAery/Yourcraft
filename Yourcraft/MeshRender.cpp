#include "MeshRender.h"

using namespace DirectX;

MeshRender::MeshRender()
{
}

void MeshRender::Init(GameObject* object)
{
	Render::Init(object);
	mModel = Model();

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
		effect.SetWorldMatrix(XMLoadFloat4x4(&mGameObject->GetWorldMatrix()));
		effect.SetTextureAmbient(part.texA);
		effect.SetTextureDiffuse(part.texD);
		effect.SetMaterial(part.material);
		effect.Apply(deviceContext);

		deviceContext->DrawIndexed(part.indexCount, 0, 0);
	}
}


void MeshRender::SetModel(Model&& model)
{
	std::swap(mModel, model);
	model.modelParts.clear();
	model.boundingBox = BoundingBox();
}

void MeshRender::SetModel(const Model& model)
{
	mModel = model;
}

const Model& MeshRender::GetModel() const
{
	return mModel;
}
