#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Model.h"

class TestObject
{
public:
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;


	TestObject();

	// ��ȡλ��
	DirectX::XMFLOAT3 GetPosition() const;

	//
	// ��ȡ��Χ��
	//

	DirectX::BoundingBox GetLocalBoundingBox() const;
	DirectX::BoundingBox GetBoundingBox() const;
	DirectX::BoundingOrientedBox GetBoundingOrientedBox() const;

	//
	// ����ʵ��������
	//

	// ��ȡ������������ʵ������Ŀ
	size_t GetCapacity() const;
	// ��������ʵ��������������ʵ������Ŀ
	void ResizeBuffer(ComPtr<ID3D11Device> device, size_t count);
	// ��ȡʵ��������

	//
	// ����ģ��
	//

	void SetModel(Model&& model);
	void SetModel(const Model& model);

	//
	// ���þ���
	//

	void SetWorldMatrix(const DirectX::XMFLOAT4X4& world);
	void SetWorldMatrix(DirectX::FXMMATRIX world);

	//
	// ����
	//
	
	// ���ƶ���
	void Draw(ComPtr<ID3D11DeviceContext> deviceContext, BasicEffect& effect);
	// ����ʵ��
	void DrawInstanced(ComPtr<ID3D11DeviceContext> deviceContext, BasicEffect & effect, const std::vector<DirectX::XMMATRIX>& data);

private:
	Model mModel;												// ģ��
	DirectX::XMFLOAT4X4 mWorldMatrix;							// �������

	ComPtr<ID3D11Buffer> mInstancedBuffer;						// ʵ��������
	size_t mCapacity;											// ����������
};




#endif