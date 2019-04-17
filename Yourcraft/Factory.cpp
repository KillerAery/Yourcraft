#include "Factory.h"
#include "d3dUtil.h"

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

void Factory::SetPool(ObjectPool<GameObject,MAX_GAMEOBJECT_NUM>* pool)
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

void Factory::SetPool(ObjectPool<ParticleSystem, 20>* pool)
{
	sFactory.rParticleSystemPool = pool;
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

MeshRender* Factory::CreateMeshRender()
{
	auto component = sFactory.rMeshRenderPool->AddObject();
	int index = sFactory.rMeshRenderPool->GetIndexByPointer(component);
	component->SetIndex_WARNING(index);
	return component;
}

Rigidbody* Factory::CreateRigidbody(ColliderPtr& collider, int mass)
{
	auto component = sFactory.rRigidbodyPool->AddObject(*sFactory.rPhysicsWorld, collider, mass);
	int index = sFactory.rRigidbodyPool->GetIndexByPointer(component);
	component->SetIndex_WARNING(index);
	return component;
}

SkyRender * Factory::CreateSkyRender(const std::wstring & cubemapFilename, float skySphereRadius, bool generateMips)
{
	auto component = sFactory.rSkyRenderPool->AddObject(sFactory.rDevice, sFactory.rDeviceContext, cubemapFilename, skySphereRadius, generateMips);
	int index = sFactory.rSkyRenderPool->GetIndexByPointer(component);
	component->SetIndex_WARNING(index);
	return component;
}

ParticleSystem * Factory::CreateParticleSystem(ParticleEffect * effect, const std::vector<std::wstring>& tex2DStringArray, UINT maxParticles)
{
	ComPtr<ID3D11ShaderResourceView> tex2DArray = CreateDDSTexture2DArrayFromFile(sFactory.rDevice, sFactory.rDeviceContext,tex2DStringArray);
	auto component = sFactory.rParticleSystemPool->AddObject(sFactory.rDevice, effect, tex2DArray, maxParticles);
	int index = sFactory.rParticleSystemPool->GetIndexByPointer(component);
	component->SetIndex_WARNING(index);
	return component;
}

