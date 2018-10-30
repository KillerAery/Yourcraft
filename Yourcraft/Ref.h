#pragma once
#include "RefZonePool.h"

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
			mRefZone = RefZonePool::Create();
			mRefZone->count = mRefZone->weakcount = 1;
		}
	}
	Ref(const Ref<T>& other) {
		Set(other);
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
	bool IsAlive()
	{
		return mPtr && mRefZone->count > 0;
	}
	void Release()
	{
		if(mPtr)
		{
			mRefZone->count = 0;
			delete mPtr;
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

	T* Get() { return mRefZone->mPtr; }
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
	void ReduceCount() {
		//资源存活才处理
		if (!IsAlive())return;

		mRefZone->count--;
		if (mRefZone->count <= 0) {
			Release();
		}
	}
	void ReduceWeakCount()
	{
		//引用空间存在才处理
		if (!mRefZone)return;

		mRefZone->weakcount--;
		if (mRefZone->weakcount <= 0) {
			RefZonePool::Release(mRefZone);
		}
		
	}
	void AddCount() {
		//资源存活才处理
		if (!IsAlive())return;
		mRefZone->count++;
	}
	void AddWeakCount()
	{
		//引用空间存在才处理
		if (!mRefZone)return;
		mRefZone->weakcount++;
	}
private:
	T* mPtr;
	RefZonePool::RefZone* mRefZone;
};





