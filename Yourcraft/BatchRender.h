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
protected:
	std::set<GameObject*> mGameObjects;							// ����ʵ�������ж���
};

