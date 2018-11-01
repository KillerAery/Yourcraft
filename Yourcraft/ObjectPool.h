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
	template<typename A1>void Update(const A1& a1);
	template<typename A1>void Update(A1&& a1);
	template<typename A1, typename A2>void Update(const A1& a1, const A2& a2);
	template<typename A1, typename A2>void Update(const A1& a1, A2&& a2);
	template<typename A1, typename A2>void Update(A1&& a1, const A2& a2);
	template<typename A1, typename A2>void Update(A1&& a1,A2&& a2);

	bool DeleteObject(int index);
	template<class FORWARD_T>T* AddObject(FORWARD_T && object);
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
	for (int i = 0; i < mSize; ++i)
	{
		if (!mObjects[i].IsAlive())
		{
			mDeletedIndexs.push_back(i);
		}
		else if (mObjects[i].IsEnabled())
		{
			mObjects[i].Update();
		}
	}
}

template<class T, int MAX_SIZE>
template<typename A1>
inline void ObjectPool<T, MAX_SIZE>::Update(const A1& a1)
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
			mObjects[i].Update(a1);
		}
	}
}

template <class T, int MAX_SIZE>
template <typename A1>
void ObjectPool<T, MAX_SIZE>::Update(A1&& a1)
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
			mObjects[i].Update(a1);
		}
	}
}

template <class T, int MAX_SIZE>
template <typename A1, typename A2>
void ObjectPool<T, MAX_SIZE>::Update(const A1& a1, const A2& a2)
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
			mObjects[i].Update(a1,a2);
		}
	}
}

template<class T, int MAX_SIZE>
template<typename A1, typename A2>
inline void ObjectPool<T, MAX_SIZE>::Update(const A1 & a1, A2 && a2)
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
			mObjects[i].Update(a1, a2);
		}
	}
}

template<class T, int MAX_SIZE>
template<typename A1, typename A2>
inline void ObjectPool<T, MAX_SIZE>::Update(A1 && a1, const A2 & a2)
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
			mObjects[i].Update(a1, a2);
		}
	}
}

template<class T, int MAX_SIZE>
template<typename A1, typename A2>
inline void ObjectPool<T, MAX_SIZE>::Update(A1 && a1, A2 && a2)
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
			mObjects[i].Update(a1, a2);
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
template <class FORWARD_T>
T* ObjectPool<T, MAX_SIZE>::AddObject(FORWARD_T&& object)
{
	//先从删除栈中取得已经被删除的索引
	if(!mDeletedIndexs.empty())
	{
		mItr = mDeletedIndexs.back();
		mDeletedIndexs.pop_back();
	}
	//判断索引所代表的区域是否存活,若存活则继续往下增加索引
	while(mObjects[mItr].IsAlive()){
		mItr++;
	}
	//复制内存到区域
	mObjects[mItr].Init(object);
	//如果使用的内存区域数量还没达到最大值，则记录数量+1.
	if(mItr >= mSize && mSize < MAX_SIZE){mSize++;}

	return &mObjects[mItr];
}

template <class T, int MAX_SIZE>
T* ObjectPool<T, MAX_SIZE>::AddObject()
{
	//先从删除栈中取得已经被删除的索引
	if (!mDeletedIndexs.empty())
	{
		mItr = mDeletedIndexs.back();
		mDeletedIndexs.pop_back();
	}
	//判断索引所代表的区域是否存活,若存活则继续往下增加索引
	while (mObjects[mItr].IsAlive()) {
		mItr++;
	}
	//对分配的内存区域进行初始化
	mObjects[mItr].Init();
	//如果使用的内存区域数量还没达到最大值，则记录数量+1.
	if (mItr >= mSize && mSize < MAX_SIZE) { mSize++; }

	return &mObjects[mItr];
}

