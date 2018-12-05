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
	//��û�ҵ�Ҫɾ����Ŀ�꣬�򷵻�ʧ��
	if (toDelete == mGameObjects.end()) {
		return false;
	}
	mGameObjects.erase(toDelete);
	return true;
}