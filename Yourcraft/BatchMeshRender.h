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
	// 重新设置实例缓冲区可容纳实例的数目
	void ResizeBuffer(ComPtr<ID3D11Device> device, size_t count);
protected:
	std::set<Transform*> mTransforms;						// 批量实例的所有变换
	Model mModel;												// 模型
	ComPtr<ID3D11Buffer> mInstancedBuffer;						// 实例缓冲区
	size_t mCapacity;											// 缓冲区容量
};

