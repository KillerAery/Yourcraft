#pragma once
#include "RefZonePool.h"



//引用指针   :一种智能指针，其分配的计数空间以内存池的形式管理
template<class T>
class Ref
{
public:
	Ref():mPtr(nullptr),mRefZone(nullptr){
	}
	Ref(T* raw) :mPtr(nullptr), mRefZone(nullptr){
		if(raw)
		{
			mPtr = raw;
			mRefZone = RefZonePool::Instance()->Create();
			mRefZone->count = mRefZone->weakcount = 1;
		}
	}
	Ref(const Ref<T>& other) {
		Set(other);
	}

	template<class U>
	Ref(const Ref<U>& other):Ref(other.Get()){

	}

	Ref& operator =(const Ref<T>& other) {
		Set(other);
		return *this;
	}
	~Ref()
	{
		ReduceCount();
		ReduceWeakCount();
	}

	//资源是否存活
	bool IsAlive()
	{
		return mPtr && mRefZone->count > 0;
	}

	//释放资源
	void Release()
	{
		if(mPtr)
		{
			mRefZone->count = 0;
			mPtr->Kill();
			mPtr = nullptr;
		}
	}

	T& operator*() { return *mPtr; }
	T* operator->() { return mPtr; }

	template<class U>
	bool operator == (U right) const {
		return mPtr == right;
	}
	template<class U>
	bool operator != (U right) const {
		return mPtr != right;
	}

	bool operator < (const Ref<T>& other)const{
		return mPtr < other.mPtr;
	}

	T* Get()const { return mPtr; }

	void Set(const Ref<T>& other) {
		if(mPtr)
		{
			ReduceCount();
			ReduceWeakCount();
		}
		mRefZone = other.mRefZone;
		mPtr = other.mPtr;
		AddCount();
		AddWeakCount();
	}

private:
	//减少资源计数
	void ReduceCount() {
		//资源存活才处理
		if (!IsAlive())return;

		mRefZone->count--;
		if (mRefZone->count <= 0) {
			Release();
		}
	}
	//减少计数空间计数
	void ReduceWeakCount()
	{
		//引用空间存在才处理
		if (!mRefZone)return;
		mRefZone->weakcount--;
		if (mRefZone->weakcount <= 0) {
			RefZonePool::Instance()->Release(mRefZone);
		}
	}
	//增加资源计数
	void AddCount() {
		//资源存活才处理
		if (!IsAlive())return;
		mRefZone->count++;
	}
	//增加计数空间计数
	void AddWeakCount()
	{
		//引用空间存在才处理
		if (!mRefZone)return;
		mRefZone->weakcount++;
	}
private:
	T* mPtr;					   //资源指针
	RefZonePool::RefZone* mRefZone;//计数空间指针
};


// 简化传参的命名
template <class T>
using CRef = const Ref<T>&;