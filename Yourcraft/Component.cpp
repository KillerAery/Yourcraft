#include "Component.h"



Component::Component():mGameObject(nullptr)
{
}

Component::~Component()
{
}

void Component::Init(GameObject* gameObject)
{
	Object::Init();
	mGameObject = gameObject;
}

bool Component::IsEnabled()
{
	return Object::IsEnabled() && mGameObject && mGameObject->IsAlive() && mGameObject->IsEnabled();
}

GameObject* Component::GetGameObject() const
{
	return mGameObject;
}

void Component::SetGameObject(GameObject * gameObject)
{
	mGameObject = gameObject;
}



