#pragma once
#include "ObjectPool.h"
#include "GameObject.h"


class Factory
{
private:
	Factory();
	static Factory sFactory;
	ObjectPool<GameObject, 500>* rGameObjectPool;
public:
	~Factory();
	static void SetGameObjectPool(ObjectPool<GameObject, 500>* object_pool);
	static Ref<GameObject> CreateGameObject();
	
};

