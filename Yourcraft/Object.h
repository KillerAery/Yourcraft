#pragma once


class Object
{
public:
	Object();
	virtual void Init();
	void Kill();
	virtual bool IsAlive();
	virtual bool IsEnabled();
	void AddRefCount();
	void ReduceRefCount();
protected:
	~Object();
	void Dead();
private:
	int mCount;
protected:
	bool mEnabled;
};

