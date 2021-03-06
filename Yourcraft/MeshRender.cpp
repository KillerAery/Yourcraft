#include "MeshRender.h"
#include "ComponentHelper.h"

using namespace DirectX;

MeshRender::MeshRender()
{
}

MeshRender::~MeshRender()
{
}

void MeshRender::Init()
{
	Render::Init();
	mModel = Model();
}

void MeshRender::BindGameObject(GameObject * gameObject)
{
	if (mGameObject) {
		UnbindGameObject();
	}
	if (gameObject) {
		mGameObject = gameObject;
		mGameObject->AddComponentInfor(static_cast<int>(ComponentType::MeshRender),mIndex);
	}
}

void MeshRender::UnbindGameObject()
{
	if (mGameObject) {
		mGameObject->RemoveComponentInfor(static_cast<int>(ComponentType::MeshRender));
		mGameObject = nullptr;
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



void MeshRender::Draw(ComPtr<ID3D11DeviceContext> deviceContext, BasicEffect & effect, const Camera* camera)
{
	// 是否绘制纹理
	effect.SetTextureUsed(mModel.textureUsed);	

	UINT strides = sizeof(DirectX::VertexPositionNormalTexture);
	UINT offsets = 0;

	// 更新世界矩阵，观察矩阵、投影矩阵
	effect.SetWorldMatrix(XMLoadFloat4x4(&mGameObject->GetWorldMatrix()));
	effect.SetViewMatrix(camera->GetViewXM());
	effect.SetProjMatrix(camera->GetProjXM());

	for (auto& part : mModel.modelParts)
	{
		// 设置顶点/索引缓冲区
		deviceContext->IASetVertexBuffers(0, 1, part.vertexBuffer.GetAddressOf(), &strides, &offsets);
		deviceContext->IASetIndexBuffer(part.indexBuffer.Get(), part.indexFormat, 0);

		// 更新数据并应用
		effect.SetTextureAmbient(part.texA);
		effect.SetTextureDiffuse(part.texD);
		effect.SetMaterial(part.material);
		effect.Apply(deviceContext);

		deviceContext->DrawIndexed(part.indexCount, 0, 0);
	}
}
