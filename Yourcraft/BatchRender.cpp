#include "BatchRender.h"



BatchRender::BatchRender()
{
}


BatchRender::~BatchRender()
{
}

void BatchRender::BindGameObject(GameObject* object)
{
	if (object)
		mGameObjects.insert(object);
}


bool BatchRender::UnbindGameObject(GameObject* object)
{
	auto toDelete = mGameObjects.find(object);
	//若没找到要删除的目标，则返还失败
	if (toDelete == mGameObjects.end()) {
		return false;
	}
	mGameObjects.erase(toDelete);
	return true;
}