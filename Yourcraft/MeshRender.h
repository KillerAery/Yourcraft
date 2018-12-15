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
	~MeshRender();
	void Init();

	void BindGameObject(GameObject * gameObject);
	void UnbindGameObject();

	void SetModel(Model&& model);
	void SetModel(const Model& model);

	void Draw(ComPtr<ID3D11DeviceContext> deviceContext, BasicEffect & effect);
protected:
	Model mModel;												// 模型
	ComPtr<ID3D11Buffer> mInstancedBuffer;						// 实例缓冲区
	size_t mCapacity;											// 缓冲区容量
};
