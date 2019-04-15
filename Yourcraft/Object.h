#pragma once

//底层对象——不应直接被使用，而是被继承从而使用
class Object
{
public:
	void SetEnabled(bool enabled);

	void AddRefCount();
	void ReduceRefCount();

	void Kill();
	void Dead();
protected:
	Object();
	~Object();

	void Init();

	bool IsAlive();
	bool IsEnabled();
private:
	int mCount;
protected:
	bool mEnabled;
};

