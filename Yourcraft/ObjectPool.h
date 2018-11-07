#pragma once
#include <vector>
#include <array>

template<class T,int MAX_SIZE>
class ObjectPool
{
public:
	ObjectPool();
	~ObjectPool();

	template <class ...Args>
	void Update(Args&&... args);

	bool DeleteObject(int index);

	template <class ...Args>
	T* AddObject(Args&&... args);

private:
	std::array<T, MAX_SIZE> mObjects;
	std::vector<int> mDeletedIndexs;
	int mSize;
};

template <class T, int MAX_SIZE>
ObjectPool<T, MAX_SIZE>::ObjectPool():
mSize(0)
{
}

template <class T, int MAX_SIZE>
ObjectPool<T, MAX_SIZE>::~ObjectPool()
{
}


template <class T, int MAX_SIZE>
template <class ... Args>
void ObjectPool<T, MAX_SIZE>::Update(Args&&... args)
{
	mDeletedIndexs.clear();
	for (int i = 0; i < mSize; ++i)
	{
		if (!mObjects[i].IsAlive())
		{
			mDeletedIndexs.push_back(i);
		}
		else if (mObjects[i].IsEnabled())
		{
			mObjects[i].Update(std::forward<Args>(args)...);
		}
	}
}


template <class T, int MAX_SIZE>
bool ObjectPool<T, MAX_SIZE>::DeleteObject(int index)
{
	if (index >= mSize)return false;
	mObjects[index].Kill();
	return true;
}

template <class T, int MAX_SIZE>
template <class ...Args>
T* ObjectPool<T, MAX_SIZE>::AddObject(Args&&... args)
{
	int itr = 0;
	//�ȴ�ɾ��ջ��ȡ���Ѿ���ɾ��������
	if(!mDeletedIndexs.empty())
	{
		itr = mDeletedIndexs.back();
		mDeletedIndexs.pop_back();
	}
	else
	{
		//�ж�����������������Ƿ���,����������������������
		while (mObjects[itr].IsAlive()) {
			itr++;
		}
	}
	//�����ڴ浽����
	mObjects[itr].Init(std::forward<Args>(args)...);
	//���ʹ�õ��ڴ�����������û�ﵽ���ֵ�����¼����+1.
	if((itr+1) >= mSize && mSize < MAX_SIZE){mSize++;}

	return &mObjects[itr];
}