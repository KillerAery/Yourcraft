#pragma once
#include "ObjectPool.h"
#include "GameObject.h"
#include "BatchMeshRender.h"
#include "MeshRender.h"
#include "Rigidbody.h"
#include "SkyRender.h"


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
public:
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
	Factory();
	static Factory sFactory;
	ObjectPool<GameObject, 500>* rGameObjectPool;
	ObjectPool<BatchMeshRender, 10>* rBatchMeshRenderPool;
	ObjectPool<MeshRender, 100>* rMeshRenderPool;
	ObjectPool<Rigidbody, 100>* rRigidbodyPool;
	ObjectPool<SkyRender, 3>* rSkyRenderPool;
	PhysicsWorld* rPhysicsWorld;
public:
	enum class ComponentType
	{
		BatchMeshRender,
		MeshRender,
		Rigidbody,
		ComponentType_Max,
		SkyRender
	};
public:
	~Factory();
	static void SetPool(ObjectPool<GameObject, 500>* pool);
	static void SetPool(ObjectPool<BatchMeshRender, 10>* pool);
	static void SetPool(ObjectPool<MeshRender, 100>* pool);
	static void SetPool(ObjectPool<Rigidbody, 100>* pool);
	static void SetPool(ObjectPool<SkyRender, 3>* pool);

	static GameObject* CreateGameObject();
	static BatchMeshRender* CreateBatchMeshRender(GameObject* gameobject);
	static MeshRender* CreateMeshRender(GameObject* gameobject);
	static Rigidbody* CreateRigidbody(GameObject * gameObject, PhysicsWorld& world, ColliderPtr& collider, int mass = 1);
	static SkyRender* CreateSkyRender(GameObject* gameObject, ComPtr<ID3D11Device> device,ComPtr<ID3D11DeviceContext> deviceContext,const std::wstring& cubemapFilename,float skySphereRadius,bool generateMips = false);

	template<class T>
	static T* GetComponent(GameObject* gameobject);

	GET_COMPONENT(BatchMeshRender);
	GET_COMPONENT(MeshRender);
	GET_COMPONENT(Rigidbody);
	GET_COMPONENT(SkyRender);
};

template <class T>
T* Factory::GetComponent(GameObject* gameobject)
{
	assert("ERROR COMPNENT TYPE!");
	return nullptr;
}


