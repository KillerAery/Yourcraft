#pragma once
#include <set>
#include "Render.h"
#include "GameObject.h"
#include "Model.h"


class BatchMeshRender :
	public Render
{
public:
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	BatchMeshRender();
	~BatchMeshRender();
	void Init(GameObject* object);
	virtual bool IsAlive();
	virtual bool IsEnabled();
	void Update(ComPtr<ID3D11DeviceContext> deviceContext, BasicEffect & effect);
	void BindGameObject(GameObject* object);
	bool UnbindGameObject(GameObject* object);
	void SetModel(Model&& model);
	void SetModel(const Model& model);

protected:
	void ResizeBuffer(ComPtr<ID3D11Device> device, size_t count);	// 重新设置实例缓冲区可容纳实例的数目
protected:
	std::set<GameObject*> mGameObjects;							// 批量实例的所有对象
	Model mModel;												// 模型
	ComPtr<ID3D11Buffer> mInstancedBuffer;						// 实例缓冲区
	size_t mCapacity;											// 缓冲区容量
};

