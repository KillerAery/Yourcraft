#pragma once
#include <set>
#include "Render.h"
#include "Transform.h"
#include "Model.h"


class BatchMeshRender :
	public Render
{
public:
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	BatchMeshRender();
	virtual void Init();
	virtual ~BatchMeshRender();
	virtual bool IsAlive();

	void Update(ComPtr<ID3D11DeviceContext> deviceContext, BasicEffect & effect);
	void BindTransform(Transform* object);
	bool UnbindTransform(Transform* object);
protected:
	// ��������ʵ��������������ʵ������Ŀ
	void ResizeBuffer(ComPtr<ID3D11Device> device, size_t count);
protected:
	std::set<Transform*> mTransforms;						// ����ʵ�������б任
	Model mModel;												// ģ��
	ComPtr<ID3D11Buffer> mInstancedBuffer;						// ʵ��������
	size_t mCapacity;											// ����������
};

