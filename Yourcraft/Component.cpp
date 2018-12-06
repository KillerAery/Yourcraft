#include "Component.h"



Component::Component():mGameObject(nullptr),mIndex(-1)
{
}

Component::~Component()
{
}


void Component::Init(GameObject* gameObject)
{
	Object::Init();
	mGameObject = gameObject;
	mIndex = -1;
}

bool Component::IsAlive()
{
	return Object::IsAlive() || (mGameObject && mGameObject->IsAlive());
}

bool Component::IsEnabled()
{
	return Object::IsEnabled() && mGameObject && mGameObject->IsAlive() && mGameObject->IsEnabled();
}

GameObject* Component::GetGameObject() const
{
	return mGameObject;
}

void Component::SetIndex_WARNING(int index)
{
	mIndex = index;
}

