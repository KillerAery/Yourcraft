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
	GameObject* GetGameObject()const;
	//设置在对象池的index,仅供factory使用，但是懒得写一堆friend了,靠命名警告吧
	void SetIndex_WARNING(int index);
protected:
	~Component();
protected:
	GameObject* mGameObject;
	int mIndex;
};

