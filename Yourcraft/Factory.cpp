#include "Factory.h"

Factory Factory::sFactory = Factory();

Factory::Factory()
{
	rGameObjectPool = nullptr;
}

Factory::~Factory()
{
}

void Factory::SetGameObjectPool(ObjectPool<GameObject, 500>* object_pool)
{
	sFactory.rGameObjectPool = object_pool;
}

Ref<GameObject> Factory::CreateGameObject()
{
	return Ref<GameObject>(sFactory.rGameObjectPool->AddObject());
}
