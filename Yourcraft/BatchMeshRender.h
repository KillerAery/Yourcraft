#pragma once
#include <set>
#include "Render.h"
#include "Model.h"

//������Ⱦ���
//����أ�����������ֻҪ��Ϸ�����б����һ�����õ���Ϸ���󣬼��ɴ��
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
	void ResizeBuffer(ComPtr<ID3D11Device> device, size_t count);	// ��������ʵ��������������ʵ������Ŀ
private:
	GameObject* CheckAlivingGameObject();							//����Ƿ����һ��������Ϸ����
protected:
	Model mModel;												// ģ��
	ComPtr<ID3D11Buffer> mInstancedBuffer;						// ʵ��������
	size_t mCapacity;											// ����������
	std::set<GameObject*> mGameObjects;							// ����ʵ�������ж���
};

