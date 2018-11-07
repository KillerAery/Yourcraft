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
	void ResizeBuffer(ComPtr<ID3D11Device> device, size_t count);	// ��������ʵ��������������ʵ������Ŀ
protected:
	std::set<GameObject*> mGameObjects;							// ����ʵ�������ж���
	Model mModel;												// ģ��
	ComPtr<ID3D11Buffer> mInstancedBuffer;						// ʵ��������
	size_t mCapacity;											// ����������
};

