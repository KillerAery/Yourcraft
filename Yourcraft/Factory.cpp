#include "Factory.h"

Factory Factory::sFactory = Factory();

Factory::Factory()
{
	rGameObjectPool = nullptr;
}

Factory::~Factory()
{
}

void Factory::SetPool(ObjectPool<GameObject, 500>* pool)
{
	sFactory.rGameObjectPool = pool;
}

void Factory::SetPool(ObjectPool<BatchMeshRender, 10>* pool)
{
	sFactory.rBatchMeshRenderPool = pool;
}

void Factory::SetPool(ObjectPool<MeshRender, 100>* pool)
{
	sFactory.rMeshRenderPool = pool;
}

GameObject* Factory::CreateGameObject()
{
	return sFactory.rGameObjectPool->AddObject();
}

BatchMeshRender* Factory::CreateBatchMeshRender(GameObject* gameobject)
{
	return sFactory.rBatchMeshRenderPool->AddObject(gameobject);
}

MeshRender* Factory::CreateMeshRender(GameObject* gameobject)
{
	return sFactory.rMeshRenderPool->AddObject(gameobject);
}
