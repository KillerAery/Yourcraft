#pragma once
#include "RefZonePool.h"



//����ָ��   :һ������ָ�룬�����ļ����ռ����ڴ�ص���ʽ����
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

	//��Դ�Ƿ���
	bool IsAlive()
	{
		return mPtr && mRefZone->count > 0;
	}

	//�ͷ���Դ
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
	//������Դ����
	void ReduceCount() {
		//��Դ���Ŵ���
		if (!IsAlive())return;

		mRefZone->count--;
		if (mRefZone->count <= 0) {
			Release();
		}
	}
	//���ټ����ռ����
	void ReduceWeakCount()
	{
		//���ÿռ���ڲŴ���
		if (!mRefZone)return;
		mRefZone->weakcount--;
		if (mRefZone->weakcount <= 0) {
			RefZonePool::Instance()->Release(mRefZone);
		}
	}
	//������Դ����
	void AddCount() {
		//��Դ���Ŵ���
		if (!IsAlive())return;
		mRefZone->count++;
	}
	//���Ӽ����ռ����
	void AddWeakCount()
	{
		//���ÿռ���ڲŴ���
		if (!mRefZone)return;
		mRefZone->weakcount++;
	}
private:
	T* mPtr;					   //��Դָ��
	RefZonePool::RefZone* mRefZone;//�����ռ�ָ��
};


// �򻯴��ε�����
template <class T>
using CRef = const Ref<T>&;