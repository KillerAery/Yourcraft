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
	if(gameobject == nullptr)
	{
		return sFactory.rBatchMeshRenderPool->AddObject(nullptr);
	}
	else
	{
		auto component = sFactory.rBatchMeshRenderPool->AddObject(gameobject);
		int index = sFactory.rBatchMeshRenderPool->GetIndexByPointer(component);
		gameobject->AddComponentInfor(static_cast<int>(ComponentType::BatchMeshRender), index);
		return component;
	}
}

MeshRender* Factory::CreateMeshRender(GameObject* gameobject)
{
	if (gameobject == nullptr)
	{
		return sFactory.rMeshRenderPool->AddObject(nullptr);
	}
	else
	{
		auto component = sFactory.rMeshRenderPool->AddObject(gameobject);
		int index = sFactory.rMeshRenderPool->GetIndexByPointer(component);
		gameobject->AddComponentInfor(static_cast<int>(ComponentType::MeshRender), index);
		return component;
	}
}

