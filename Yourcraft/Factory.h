#pragma once
#include "ObjectPool.h"
#include "GameObject.h"
#include "BatchMeshRender.h"
#include "MeshRender.h"
#include "Rigidbody.h"
#include "PhysicsWorld.h"

//获取组件特化模板函数 宏定义
#define GET_COMPONENT(_TYPE_)\
	template<>static _TYPE_* GetComponent<_TYPE_>(GameObject* gameObject) {\
		int index = gameObject->FindComponetIndex(static_cast<int>(ComponentType::_TYPE_));\
		if (index == -1) {return nullptr;}\
		else {return sFactory.r##_TYPE_##Pool->FindObject(index);}\
	}

//工厂
class Factory
{
private:
	Factory();
	static Factory sFactory;
	ObjectPool<GameObject, 500>* rGameObjectPool;
	ObjectPool<BatchMeshRender, 10>* rBatchMeshRenderPool;
	ObjectPool<MeshRender, 100>* rMeshRenderPool;
	ObjectPool<Rigidbody, 100>* rRigidbodyPool;
	PhysicsWorld* rPhysicsWorld;
public:
	enum class ComponentType
	{
		BatchMeshRender,
		MeshRender,
		Rigidbody,
		ComponentType_Max
	};
public:
	~Factory();
	static void SetPool(ObjectPool<GameObject, 500>* pool);
	static void SetPool(ObjectPool<BatchMeshRender, 10>* pool);
	static void SetPool(ObjectPool<MeshRender, 100>* pool);
	static void SetPool(ObjectPool<Rigidbody, 100>* pool);
	static void SetPhysicsWorld(PhysicsWorld* world);

	static GameObject* CreateGameObject();
	static BatchMeshRender* CreateBatchMeshRender(GameObject* gameobject);
	static MeshRender* CreateMeshRender(GameObject* gameobject);
	static Rigidbody* CreateRigidbody(GameObject * gameObject, PhysicsWorld& world, ColliderPtr& collider, int mass = 1);

	template<class T>
	static T* GetComponent(GameObject* gameobject);

	GET_COMPONENT(BatchMeshRender);
	GET_COMPONENT(MeshRender);
	GET_COMPONENT(Rigidbody);
};

template <class T>
T* Factory::GetComponent(GameObject* gameobject)
{
	assert("ERROR COMPNENT TYPE!");
	return nullptr;
}


