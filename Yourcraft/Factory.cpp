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

void Factory::SetPool(ObjectPool<Rigidbody, 100>* pool)
{
	sFactory.rRigidbodyPool = pool;
}

void Factory::SetPool(ObjectPool<SkyRender, 3>* pool)
{
	sFactory.rSkyRenderPool = pool;
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
		return nullptr;
	}
	else
	{
		auto component = sFactory.rMeshRenderPool->AddObject(gameobject);
		int index = sFactory.rMeshRenderPool->GetIndexByPointer(component);
		gameobject->AddComponentInfor(static_cast<int>(ComponentType::MeshRender), index);
		return component;
	}
}

Rigidbody* Factory::CreateRigidbody(GameObject * gameobject, PhysicsWorld& world, ColliderPtr& collider, int mass)
{
	if (gameobject == nullptr)
	{
		return nullptr;
	}
	else
	{
		auto component = sFactory.rRigidbodyPool->AddObject(gameobject,world,collider,mass);
		int index = sFactory.rRigidbodyPool->GetIndexByPointer(component);
		gameobject->AddComponentInfor(static_cast<int>(ComponentType::Rigidbody), index);
		return component;
	}
}

SkyRender * Factory::CreateSkyRender(GameObject * gameObject, ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> deviceContext, const std::wstring & cubemapFilename, float skySphereRadius, bool generateMips)
{
	if (gameObject == nullptr)
	{
		return nullptr;
	}
	else
	{
		auto component = sFactory.rSkyRenderPool->AddObject(gameObject, device,deviceContext,cubemapFilename,skySphereRadius, generateMips);
		int index = sFactory.rSkyRenderPool->GetIndexByPointer(component);
		gameObject->AddComponentInfor(static_cast<int>(ComponentType::SkyRender), index);
		return component;
	}
}

