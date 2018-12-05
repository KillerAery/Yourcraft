#pragma once
#include <set>
#include "GameObject.h"
#include "Component.h"

class BatchRender :
	public Component
{
protected:
	BatchRender();
public:
	~BatchRender();
	void BindGameObject(GameObject* object);
	bool UnbindGameObject(GameObject* object);
protected:
	std::set<GameObject*> mGameObjects;							// ����ʵ�������ж���
};

