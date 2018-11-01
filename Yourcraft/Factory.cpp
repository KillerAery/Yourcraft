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

Ref<GameObject> Factory::CreateGameObject()
{
	return Ref<GameObject>(sFactory.rGameObjectPool->AddObject());
}

Ref<BatchMeshRender> Factory::CreateBatchMeshRender()
{
	return Ref<BatchMeshRender>(sFactory.rBatchMeshRenderPool->AddObject());
}

Ref<MeshRender> Factory::CreateMeshRender(const Ref<GameObject>& gameobject)
{
	return Ref<MeshRender>(sFactory.rMeshRenderPool->AddObject(gameobject));
}
