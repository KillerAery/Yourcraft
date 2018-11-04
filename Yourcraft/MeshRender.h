#pragma once
#include "Render.h"
#include "GameObject.h"
#include "Model.h"


class MeshRender :
	public Render
{
public:
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	MeshRender();
	virtual void Init(GameObject* object);
	virtual ~MeshRender();
	virtual bool IsAlive();

	void Update(ComPtr<ID3D11DeviceContext> deviceContext, BasicEffect & effect);
	void BindTransform(Transform* transform);
	void UnbindTransform();
	void SetModel(Model&& model);
	void SetModel(const Model& model);
protected:
	Transform* mTransform;										// 变换
	Model mModel;												// 模型
	ComPtr<ID3D11Buffer> mInstancedBuffer;						// 实例缓冲区
	size_t mCapacity;											// 缓冲区容量
};
