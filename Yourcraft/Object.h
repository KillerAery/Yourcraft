#pragma once

class Object
{
protected:
	virtual ~Object();
public:
	Object();
	void Kill();
	bool IsAlive();
	bool IsEnabled();
	void Init();

	void operator delete(void* address) noexcept;
	void* operator new(size_t size) = delete;
	void* operator new[](size_t size) = delete;
	void  operator delete[](void * address) noexcept = delete;
	void* operator new(size_t size,void * address) = delete;
protected:
	bool mAlive;
	bool mEnabled;
};

