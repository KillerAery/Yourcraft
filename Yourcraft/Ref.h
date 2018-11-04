#pragma once
#include "RefZonePool.h"

template<class T>
class Ref;

template<class T>
class WeakRef;

//强引用
//控制生命周期和访问的智能指针，其分配的计数空间以内存池的形式管理
template<class T>
class Ref
{
public:
	Ref() :mPtr(nullptr), mRefZone(nullptr) {
	}
	Ref(T* raw){
		*this = raw;
	}
	template<class U>Ref(U* raw) : Ref() {
		*this = raw;
	}
	Ref(const Ref<T>& other) : Ref() {
		*this = other;
	}
	template<class U>Ref(const Ref<U>& other) : Ref() {
		*this = other;
	}
	Ref(const WeakRef<T>& other) : Ref() {
		*this = other;
	}
	template<class U>Ref(const WeakRef<U>& other) : Ref() {
		*this = other;
	}
	Ref& operator = (T* raw) {
		Set(raw);
		return *this;
	}
	template<class U>Ref& operator = (U* raw) {
		Set(raw);
		return *this;
	}
	Ref& operator =(const Ref<T>& other) {
		Set(other);
		return *this;
	}
	template<class U>Ref& operator =(const Ref<U>& other) {
		Set(other);
		return *this;
	}
	Ref& operator =(const WeakRef<T>& other) {
		Set(other);
		return *this;
	}
	template<class U>Ref& operator =(const WeakRef<U>& other) {
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
		if (mPtr)
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

	bool operator < (const Ref<T>& other)const {
		return mPtr < other.mPtr;
	}

	T* Get() const{ return mPtr; }
	RefZonePool::RefZone* GetZone() const{ return mRefZone; }

	void Set(T* raw) {
		if (mPtr) {
			ReduceCount();
			ReduceWeakCount();
		}
		mPtr = raw;
		if (raw)
		{
			mRefZone = RefZonePool::Instance()->Create();
			mRefZone->count = 1;
			mRefZone->weakcount = 1;
		}
	}
	template<class U>void Set(U* raw) {
		if (mPtr) {
			ReduceCount();
			ReduceWeakCount();
		}
		mPtr = raw;
		if (raw)
		{
			mRefZone = RefZonePool::Instance()->Create();
			mRefZone->count = 1;
			mRefZone->weakcount = 1;
		}
	}
	void Set(const Ref<T>& other) {
		if (mPtr) {
			ReduceCount();
			ReduceWeakCount();
		}
		mPtr = other.Get();
		mRefZone = other.GetZone();
		if (mPtr)
		{
			AddCount();
			AddWeakCount();
		}
	}
	template<class U>void Set(const Ref<U>& other) {
		if (mPtr){
			ReduceCount();
			ReduceWeakCount();
		}
		mPtr = other.Get();
		mRefZone = other.GetZone();
		if (mPtr)
		{
			AddCount();
			AddWeakCount();
		}
	}
	void Set(const WeakRef<T>& other) {
		if (mPtr) {
			ReduceCount();
			ReduceWeakCount();
		}
		mPtr = other.Get();
		mRefZone = other.GetZone();
		if (mPtr)
		{
			AddCount();
			AddWeakCount();
		}
	}
	template<class U>void Set(const WeakRef<U>& other) {
		if (mPtr){
			ReduceCount();
			ReduceWeakCount();
		}
		mPtr = other.Get();
		mRefZone = other.GetZone();
		if (mPtr)
		{
			AddCount();
			AddWeakCount();
		}
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


//弱引用
//控制访问的智能指针，其分配的计数空间以内存池的形式管理
template<class T>
class WeakRef
{
public:
	WeakRef() :mPtr(nullptr), mRefZone(nullptr) {
	}
	WeakRef(T* raw) :WeakRef() {
		*this = raw;
	}
	template<class U>WeakRef(U* raw) :WeakRef() {
		*this = raw;
	}
	WeakRef(const Ref<T>& other) : WeakRef() {
		*this = other;
	}
	template<class U>WeakRef(const Ref<U>& other) : WeakRef() {
		*this = other;
	}
	WeakRef(const WeakRef<T>& other) : WeakRef() {
		*this = other;
	}
	template<class U>WeakRef(const WeakRef<U>& other) : WeakRef() {
		*this = other;
	}
	WeakRef& operator =(T* raw) {
		Set(raw);
		return *this;
	}
	template<class U>WeakRef& operator =(U* raw) {
		Set(raw);
		return *this;
	}
	WeakRef& operator =(const Ref<T>& other) {
		Set(other);
		return *this;
	}
	template<class U>WeakRef& operator =(const Ref<U>& other){
		Set(other);
		return *this;
	}
	WeakRef& operator =(const WeakRef<T>& other) {
		Set(other);
		return *this;
	}
	template<class U>WeakRef& operator =(const WeakRef<U>& other){
		Set(other);
		return *this;
	}

	~WeakRef()
	{
		ReduceWeakCount();
	}

	//资源是否存活
	bool IsAlive(){
		return mPtr && mRefZone->count > 0;
	}

	T& operator*() { return *mPtr; }
	T* operator->() { return mPtr; }

	template<class U>
	WeakRef<U>& As() {
		return reinterpret_cast<WeakRef<U>>(*this);
	}

	template<class U>
	bool operator == (U right) const {
		return mPtr == right;
	}
	template<class U>
	bool operator != (U right) const {
		return mPtr != right;
	}

	bool operator < (const WeakRef<T>& other)const {
		return mPtr < other.mPtr;
	}

	T* Get() const{ return mPtr; }
	RefZonePool::RefZone* GetZone() const{ return mRefZone; }

	void Set(T* raw) {
		if (mPtr) {
			ReduceWeakCount();
		}
		mPtr = raw;
		if (raw)
		{
			mRefZone = RefZonePool::Instance()->Create();
			mRefZone->count = 0;
			mRefZone->weakcount = 1;
		}
	}
	template<class U>void Set(U* raw) {
		if (mPtr) {
			ReduceWeakCount();
		}
		mPtr = raw;
		if (raw)
		{
			mRefZone = RefZonePool::Instance()->Create();
			mRefZone->count = 0;
			mRefZone->weakcount = 1;
		}
	}
	void Set(const Ref<T>& other) {
		if (mPtr) {
			ReduceWeakCount();
		}
		mPtr = other.Get();
		if (mPtr) {
			mRefZone = other.GetZone();
			AddWeakCount();
		}
	}
	template<class U>void Set(const Ref<U>& other) {
		if (mPtr) {
			ReduceWeakCount();
		}
		mPtr = other.Get();
		if (mPtr) {
			mRefZone = other.GetZone();
			AddWeakCount();
		}
	}
	void Set(const WeakRef<T>& other) {
		if (mPtr) {
			ReduceWeakCount();
		}
		mPtr = other.Get();
		if (mPtr) {
			mRefZone = other.GetZone();
			AddWeakCount();
		}
	}
	template<class U>void Set(const WeakRef<U>& other) {
		if (mPtr) {
			ReduceWeakCount();
		}
		mPtr = other.Get();
		if (mPtr) {
			mRefZone = other.GetZone();
			AddWeakCount();
		}
	}
private:
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
using CRef = const WeakRef<T>&;