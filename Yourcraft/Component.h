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
	//�����ڶ���ص�index,����factoryʹ�ã���������дһ��friend��,�����������
	void SetIndex_WARNING(int index);
protected:
	~Component();
protected:
	GameObject* mGameObject;
	int mIndex;
};

