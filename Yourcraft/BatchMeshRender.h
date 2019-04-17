#pragma once
#include <set>
#include "Render.h"
#include "Model.h"

//批量渲染组件
//特殊地，其生命周期只要游戏对象列表存在一个可用的游戏对象，即可存活
class BatchMeshRender :
	public Render
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

	void SetModel(Model&& model);
	void SetModel(const Model& model);

	void Draw(ComPtr<ID3D11DeviceContext> deviceContext, BasicEffect & effect);
protected:
	void ResizeBuffer(ComPtr<ID3D11Device> device, size_t count);	// 重新设置实例缓冲区可容纳实例的数目
private:
	GameObject* CheckAlivingGameObject();							//检查是否存在一个存活的游戏对象
protected:
	Model mModel;												// 模型
	ComPtr<ID3D11Buffer> mInstancedBuffer;						// 实例缓冲区
	size_t mCapacity;											// 缓冲区容量
	std::set<GameObject*> mGameObjects;							// 批量实例的所有对象
};

