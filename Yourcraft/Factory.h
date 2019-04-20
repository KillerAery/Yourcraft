#pragma once
#include "ObjectPool.h"
#include "GameObject.h"
#include "ComponentHelper.h"

#include "ParticleSystem.h"
#include "BatchMeshRender.h"
#include "MeshRender.h"
#include "Rigidbody.h"
#include "SkyRender.h"
#include "Camera.h"


//获取组件特化模板函数 宏定义
#define GET_COMPONENT(_TYPE_)\
	template<>static _TYPE_* GetComponent<_TYPE_>(GameObject* gameObject) {\
		int index = gameObject->FindComponetIndex(static_cast<int>(ComponentType::_TYPE_));\
		if (index == -1) {return nullptr;}\
		else {return sFactory.r##_TYPE_##Pool->FindObject(index);}\
	}


const int MAX_GAMEOBJECT_NUM = 1024;

//工厂
class Factory
{
public:
	template <class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;
private:
	Factory();
	static Factory sFactory;
public:
	~Factory();
	static void SetPhysicsWorld(PhysicsWorld* world);

	static void SetPool(ObjectPool<GameObject, MAX_GAMEOBJECT_NUM>* pool);
	static void SetPool(ObjectPool<BatchMeshRender, 10>* pool);
	static void SetPool(ObjectPool<MeshRender, 100>* pool);
	static void SetPool(ObjectPool<Rigidbody, 100>* pool);
	static void SetPool(ObjectPool<SkyRender, 1>* pool);
	static void SetPool(ObjectPool<ParticleSystem, 20>* pool);
	static void SetPool(ObjectPool<Camera, 3>* pool);

	static void SetDevice(ComPtr<ID3D11Device> Device);
	static void SetDeviceContext(ComPtr<ID3D11DeviceContext> DeviceContext);

	static GameObject* CreateGameObject();
	static BatchMeshRender* CreateBatchMeshRender();
	static MeshRender* CreateMeshRender();
	static Rigidbody * CreateRigidbody(ColliderPtr & collider, int mass = 1);
	static SkyRender* CreateSkyRender(const std::wstring& cubemapFilename,float skySphereRadius,bool generateMips = false);
	static ParticleSystem* CreateParticleSystem(ParticleEffect * effect,const std::vector<std::wstring>& tex2DStringArray, UINT maxParticles);
	static Camera* CreateCamera();

	template<class T>
	static T* GetComponent(GameObject* gameobject);

	GET_COMPONENT(BatchMeshRender);
	GET_COMPONENT(MeshRender);
	GET_COMPONENT(Rigidbody);
	GET_COMPONENT(SkyRender);
	GET_COMPONENT(ParticleSystem);
	GET_COMPONENT(Camera);
protected:
	ObjectPool<GameObject, MAX_GAMEOBJECT_NUM>* rGameObjectPool;
	ObjectPool<BatchMeshRender, 10>* rBatchMeshRenderPool;
	ObjectPool<MeshRender, 100>* rMeshRenderPool;
	ObjectPool<Rigidbody, 100>* rRigidbodyPool;
	ObjectPool<SkyRender, 1>* rSkyRenderPool;
	ObjectPool<ParticleSystem,20> * rParticleSystemPool;
	ObjectPool<Camera, 3>* rCameraPool;
	ComPtr<ID3D11Device> rDevice;
	ComPtr<ID3D11DeviceContext> rDeviceContext;
	PhysicsWorld* rPhysicsWorld;
};

template <class T>
T* Factory::GetComponent(GameObject* gameobject)
{
	assert("ERROR COMPNENT TYPE!");
	return nullptr;
}


