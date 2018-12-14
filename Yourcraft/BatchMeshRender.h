#pragma once

#include "BatchRender.h"
#include "Model.h"


class BatchMeshRender :
	public BatchRender
{
public:
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	BatchMeshRender();
	~BatchMeshRender();
	void Init();

	void BindGameObject(GameObject* object);
	bool UnbindGameObject(GameObject* object);

	virtual bool IsAlive();
	virtual bool IsEnabled();
	void Update(ComPtr<ID3D11DeviceContext> deviceContext, BasicEffect & effect);

	void SetModel(Model&& model);
	void SetModel(const Model& model);
protected:
	void ResizeBuffer(ComPtr<ID3D11Device> device, size_t count);	// ��������ʵ��������������ʵ������Ŀ
	void Draw(ComPtr<ID3D11DeviceContext> deviceContext, BasicEffect & effect);
protected:
	Model mModel;												// ģ��
	ComPtr<ID3D11Buffer> mInstancedBuffer;						// ʵ��������
	size_t mCapacity;											// ����������
};

