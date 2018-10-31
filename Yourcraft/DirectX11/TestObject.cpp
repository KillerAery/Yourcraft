#include "TestObject.h"
#include "d3dUtil.h"
using namespace DirectX;

struct InstancedData
{
	XMMATRIX world;
	XMMATRIX worldInvTranspose;
};

TestObject::TestObject()
	: mWorldMatrix(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f)
{
}

DirectX::XMFLOAT3 TestObject::GetPosition() const
{
	return XMFLOAT3(mWorldMatrix(3, 0), mWorldMatrix(3, 1), mWorldMatrix(3, 2));
}

BoundingBox TestObject::GetBoundingBox() const
{
	BoundingBox box;
	mModel.boundingBox.Transform(box, XMLoadFloat4x4(&mWorldMatrix));
	return box;
}

DirectX::BoundingOrientedBox TestObject::GetBoundingOrientedBox() const
{
	BoundingOrientedBox box;
	BoundingOrientedBox::CreateFromBoundingBox(box, mModel.boundingBox);
	box.Transform(box, XMLoadFloat4x4(&mWorldMatrix));
	return box;
}


BoundingBox TestObject::GetLocalBoundingBox() const
{
	return mModel.boundingBox;
}

size_t TestObject::GetCapacity() const
{
	return mCapacity;
}

void TestObject::ResizeBuffer(ComPtr<ID3D11Device> device, size_t count)
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




void TestObject::SetModel(Model && model)
{
	std::swap(mModel, model);
	model.modelParts.clear();
	model.boundingBox = BoundingBox();
}

void TestObject::SetModel(const Model & model)
{
	mModel = model;
}

void TestObject::SetWorldMatrix(const XMFLOAT4X4 & world)
{
	mWorldMatrix = world;
}

void TestObject::SetWorldMatrix(FXMMATRIX world)
{
	XMStoreFloat4x4(&mWorldMatrix, world);
}

void TestObject::Draw(ComPtr<ID3D11DeviceContext> deviceContext, BasicEffect & effect)
{
	UINT strides = sizeof(DirectX::VertexPositionNormalTexture);
	UINT offsets = 0;

	for (auto& part : mModel.modelParts)
	{
		// ���ö���/����������
		deviceContext->IASetVertexBuffers(0, 1, part.vertexBuffer.GetAddressOf(), &strides, &offsets);
		deviceContext->IASetIndexBuffer(part.indexBuffer.Get(), part.indexFormat, 0);

		// �������ݲ�Ӧ��
		effect.SetWorldMatrix(XMLoadFloat4x4(&mWorldMatrix));
		effect.SetTextureAmbient(part.texA);
		effect.SetTextureDiffuse(part.texD);
		effect.SetMaterial(part.material);
		effect.Apply(deviceContext);

		deviceContext->DrawIndexed(part.indexCount, 0, 0);
	}
}

void TestObject::DrawInstanced(ComPtr<ID3D11DeviceContext> deviceContext, BasicEffect & effect, const std::vector<DirectX::XMMATRIX>& data)
{
	D3D11_MAPPED_SUBRESOURCE mappedData;
	UINT numInsts = (UINT)data.size();
	//����������ݱ�ʵ��������������Ҫ���·���
	if (numInsts > mCapacity)
	{
		ComPtr<ID3D11Device> device;
		deviceContext->GetDevice(device.GetAddressOf());
		ResizeBuffer(device, numInsts);
	}

	HR(deviceContext->Map(mInstancedBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));

	InstancedData * iter = reinterpret_cast<InstancedData *>(mappedData.pData);
	for (auto& mat : data)
	{
		iter->world = XMMatrixTranspose(mat);
		iter->worldInvTranspose = XMMatrixInverse(nullptr, mat);	// ����ת�õ���
		iter++;
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

