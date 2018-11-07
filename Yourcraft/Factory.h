#pragma once
#include "ObjectPool.h"
#include "GameObject.h"
#include "BatchMeshRender.h"
#include "MeshRender.h"

class Factory
{
private:
	Factory();
	static Factory sFactory;
	ObjectPool<GameObject, 500>* rGameObjectPool;
	ObjectPool<BatchMeshRender, 10>* rBatchMeshRenderPool;
	ObjectPool<MeshRender, 100>* rMeshRenderPool;
public:
	~Factory();
	static void SetPool(ObjectPool<GameObject, 500>* pool);
	static void SetPool(ObjectPool<BatchMeshRender, 10>* pool);
	static void SetPool(ObjectPool<MeshRender, 100>* pool);


	static GameObject* CreateGameObject();
	static BatchMeshRender* CreateBatchMeshRender(GameObject* gameobject);
	static MeshRender* CreateMeshRender(GameObject* gameobject);
};

