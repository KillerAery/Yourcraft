#pragma once

#include "Object.h"
#include "GameObject.h"
#include "ObjectPool.h"

class Component : public Object
{
public:
	Component();
	bool IsAlive();
	bool IsEnabled();
	GameObject* GetGameObject()const;
	//�����ڶ���ص�index,����factoryʹ�ã���������дһ��friend��,�����������
	void SetIndex_WARNING(int index);
protected:
	~Component();
	void Init();
protected:
	GameObject* mGameObject;
	int mIndex;
};

