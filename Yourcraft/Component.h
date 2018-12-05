#pragma once

#include "Object.h"
#include "GameObject.h"
#include "ObjectPool.h"

class Component : public Object
{
public:
	Component();
	void Init(GameObject* gameObject);
	virtual bool IsAlive();
	virtual bool IsEnabled();
	void BindGameObject(GameObject* gameObject);
	void UnbindGameObject();
	GameObject* GetGameObject()const;
protected:
	~Component();
protected:
	GameObject* mGameObject;
	
};

