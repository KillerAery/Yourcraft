#pragma once
#include <vector>
#include <queue>
#include <array>

template<class T,int MAX_SIZE>
class ObjectPool
{
public:
	ObjectPool();
	~ObjectPool();
	void Update();
	bool DeleteObject(int index);
		template<class FORWARD_T>
	T* AddObject(FORWARD_T && object);
	T* AddObject();
private:
	std::array<T, MAX_SIZE> mObjects;
	std::vector<int> mDeletedIndexs;
	int mSize;
	int mItr;
};

template <class T, int MAX_SIZE>
ObjectPool<T, MAX_SIZE>::ObjectPool():mSize(0), mItr(0)
{
}

template <class T, int MAX_SIZE>
ObjectPool<T, MAX_SIZE>::~ObjectPool()
{
}

template <class T, int MAX_SIZE>
void ObjectPool<T, MAX_SIZE>::Update()
{
	mDeletedIndexs.clear();
	for(int i =0;i<mSize;++i)
	{
		if(!mObjects[i].IsAlive())
		{
			mDeletedIndexs.push_back(i);
		}
		else if(mObjects[i].IsEnabled())
		{
			mObjects[i].Update();
		}
	}

}

template <class T, int MAX_SIZE>
bool ObjectPool<T, MAX_SIZE>::DeleteObject(int index)
{
	if (index >= mSize)return false;
	//ɾ������ֻ��Ҫ�޸Ĵ����
	mObjects[index].SetAlive(false);
	return true;
}

template <class T, int MAX_SIZE>
template <class FORWARD_T>
T* ObjectPool<T, MAX_SIZE>::AddObject(FORWARD_T&& object)
{
	//�ȴ�ɾ��ջ��ȡ���Ѿ���ɾ��������
	if(!mDeletedIndexs.empty())
	{
		mItr = mDeletedIndexs.back();
		mDeletedIndexs.pop_back();
	}
	//�ж�����������������Ƿ���,����������������������
	while(mObjects[mItr].IsAlive()){
		mItr++;
	}
	//�����ڴ浽����
	mObjects[mItr] = T(object);
	//���ʹ�õ��ڴ�����������û�ﵽ���ֵ�����¼����+1.
	if(mItr >= mSize && mSize < MAX_SIZE){mSize++;}

	return &mObjects[mItr];
}

template <class T, int MAX_SIZE>
T* ObjectPool<T, MAX_SIZE>::AddObject()
{
	//�ȴ�ɾ��ջ��ȡ���Ѿ���ɾ��������
	if (!mDeletedIndexs.empty())
	{
		mItr = mDeletedIndexs.back();
		mDeletedIndexs.pop_back();
	}
	//�ж�����������������Ƿ���,����������������������
	while (mObjects[mItr].IsAlive()) {
		mItr++;
	}
	//�Է�����ڴ�������г�ʼ��
	mObjects[mItr].Init();
	//���ʹ�õ��ڴ�����������û�ﵽ���ֵ�����¼����+1.
	if (mItr >= mSize && mSize < MAX_SIZE) { mSize++; }

	return &mObjects[mItr];
}

