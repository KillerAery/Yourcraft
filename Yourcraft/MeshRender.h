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
	void Init(GameObject* gameObject);

	void BindGameObject(GameObject * gameObject);
	void UnbindGameObject();

	void Update(ComPtr<ID3D11DeviceContext> deviceContext, BasicEffect & effect);
	void SetModel(Model&& model);
	void SetModel(const Model& model);
	const Model& GetModel()const;
protected:
	Model mModel;												// ģ��
	ComPtr<ID3D11Buffer> mInstancedBuffer;						// ʵ��������
	size_t mCapacity;											// ����������
};
