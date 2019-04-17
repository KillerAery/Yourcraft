#include "BatchMeshRender.h"
#include "ComponentHelper.h"
using namespace DirectX;

struct InstancedData
{
	XMMATRIX world;
	XMMATRIX worldInvTranspose;
};

BatchMeshRender::BatchMeshRender()
{
}

BatchMeshRender::~BatchMeshRender()
{

}

void BatchMeshRender::Init()
{
	Render::Init();
	mGameObjects.clear();
}

void BatchMeshRender::BindGameObject(GameObject* object)
{
	if (object) {
		auto toBind = mGameObjects.find(object);
		//��û�ҵ�һ����Ŀ�꣬�����
		if (toBind == mGameObjects.end()) {
			mGameObjects.insert(object);
			object->AddComponentInfor(static_cast<int>(ComponentType::BatchMeshRender), mIndex);
			//����mGameObject ���Ʊ�����ػ���
			mGameObject = CheckAlivingGameObject();
		}
	}
}


bool BatchMeshRender::UnbindGameObject(GameObject* object)
{
	auto toDelete = mGameObjects.find(object);
	//��û�ҵ�Ҫɾ����Ŀ�꣬�򷵻�ʧ��
	if (toDelete == mGameObjects.end()) {
		return false;
	}
	(*toDelete)->RemoveComponentInfor(static_cast<int>(ComponentType::BatchMeshRender));
	mGameObjects.erase(toDelete);
	//����Ӧ����mGameObject ���Ʊ�����ػ���
	mGameObject = CheckAlivingGameObject();

	return true;
}

void BatchMeshRender::SetModel(Model&& model)
{
	std::swap(mModel, model);
	model.modelParts.clear();
	model.boundingBox = BoundingBox();
}

void BatchMeshRender::SetModel(const Model& model)
{
	mModel = model;
}

void BatchMeshRender::ResizeBuffer(ComPtr<ID3D11Device> device, size_t count)
{	
	// ����ʵ������������
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = (UINT)count * sizeof(XMMATRIX) * 2;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	mCapacity = count;
	// ����ʵ��������
	HR(device->CreateBuffer(&vbd, nullptr, mInstancedBuffer.ReleaseAndGetAddressOf()));
}

GameObject * BatchMeshRender::CheckAlivingGameObject()
{
	for (auto go : mGameObjects){
		if (go->IsAlive()) {
			return go;
		}
	}
	return nullptr;
}

void BatchMeshRender::Draw(ComPtr<ID3D11DeviceContext> deviceContext, BasicEffect & effect)
{
	if (mGameObjects.empty())return;

	D3D11_MAPPED_SUBRESOURCE mappedData;
	UINT numInsts = (UINT)mGameObjects.size();
	//����������ݱ�ʵ��������������Ҫ���·���
	if (numInsts > mCapacity)
	{
		ComPtr<ID3D11Device> device;
		deviceContext->GetDevice(device.GetAddressOf());
		ResizeBuffer(device, numInsts);
	}

	HR(deviceContext->Map(mInstancedBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));

	InstancedData * iter = reinterpret_cast<InstancedData*>(mappedData.pData);


	for (auto go : mGameObjects)
	{
		if (go->IsAlive() && go->IsEnabled()) {
			//������Ҫ��const&��������˴˴���constǿ��ת��
			XMMATRIX matrix = XMLoadFloat4x4(&go->GetWorldMatrix());
			iter->world = XMMatrixTranspose(matrix);
			iter->worldInvTranspose = XMMatrixInverse(nullptr, matrix);	// ����ת�õ���
			iter++;
		}
	}

	deviceContext->Unmap(mInstancedBuffer.Get(), 0);

	UINT strides[2] = { sizeof(DirectX::VertexPositionNormalTexture), sizeof(InstancedData) };
	UINT offsets[2] = { 0, 0 };
	ID3D11Buffer * buffers[2] = { nullptr, mInstancedBuffer.Get() };
	for (auto& part : mModel.modelParts)
	{
		buffers[0] = part.vertexBuffer.Get();

		// ���ö���/����������
		deviceContext->IASetVertexBuffers(0, 2, buffers, strides, offsets);
		deviceContext->IASetIndexBuffer(part.indexBuffer.Get(), part.indexFormat, 0);

		// �������ݲ�Ӧ��
		effect.SetTextureAmbient(part.texA);
		effect.SetTextureDiffuse(part.texD);
		effect.SetMaterial(part.material);
		effect.Apply(deviceContext);

		deviceContext->DrawIndexedInstanced(part.indexCount, numInsts, 0, 0, 0);
	}
}
