#include "Factory.h"
#include "ComponentHelper.h"

Factory Factory::sFactory = Factory();

Factory::Factory()
{
	rGameObjectPool = nullptr;
}

Factory::~Factory()
{
}

void Factory::SetPhysicsWorld(PhysicsWorld * world)
{
	sFactory.rPhysicsWorld = world;
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

void Factory::SetPool(ObjectPool<Rigidbody, 100>* pool)
{
	sFactory.rRigidbodyPool = pool;
}

void Factory::SetPool(ObjectPool<SkyRender, 3>* pool)
{
	sFactory.rSkyRenderPool = pool;
}

void Factory::SetDevice(ComPtr<ID3D11Device> device)
{
	sFactory.rDevice = device;
}

void Factory::SetDeviceContext(ComPtr<ID3D11DeviceContext> deviceContext)
{
	sFactory.rDeviceContext = deviceContext;
}

GameObject* Factory::CreateGameObject()
{
	return sFactory.rGameObjectPool->AddObject();
}

BatchMeshRender* Factory::CreateBatchMeshRender()
{
	auto component = sFactory.rBatchMeshRenderPool->AddObject();
	int index = sFactory.rBatchMeshRenderPool->GetIndexByPointer(component);
	component->SetIndex_WARNING(index);
	return component;
}

MeshRender* Factory::CreateMeshRender(GameObject* gameobject)
{
	if (gameobject == nullptr)
	{
		return nullptr;
	}
	else
	{
		auto component = sFactory.rMeshRenderPool->AddObject(gameobject);
		int index = sFactory.rMeshRenderPool->GetIndexByPointer(component);
		component->SetIndex_WARNING(index);
		gameobject->AddComponentInfor(static_cast<int>(ComponentType::MeshRender), index);
		return component;
	}
}

Rigidbody* Factory::CreateRigidbody(GameObject * gameobject, ColliderPtr& collider, int mass)
{
	if (gameobject == nullptr)
	{
		return nullptr;
	}
	else
	{
		auto component = sFactory.rRigidbodyPool->AddObject(gameobject,*sFactory.rPhysicsWorld,collider,mass);
		int index = sFactory.rRigidbodyPool->GetIndexByPointer(component);
		component->SetIndex_WARNING(index);
		gameobject->AddComponentInfor(static_cast<int>(ComponentType::Rigidbody), index);
		return component;
	}
}

SkyRender * Factory::CreateSkyRender(GameObject * gameObject, const std::wstring & cubemapFilename, float skySphereRadius, bool generateMips)
{
	if (gameObject == nullptr)
	{
		return nullptr;
	}
	else
	{
		auto component = sFactory.rSkyRenderPool->AddObject(gameObject,sFactory.rDevice,sFactory.rDeviceContext,cubemapFilename,skySphereRadius, generateMips);
		int index = sFactory.rSkyRenderPool->GetIndexByPointer(component);
		component->SetIndex_WARNING(index);
		gameObject->AddComponentInfor(static_cast<int>(ComponentType::SkyRender), index);
		return component;
	}
}

