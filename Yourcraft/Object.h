#pragma once


class Object
{
public:
	Object();
	void Init();
	void Kill();
	virtual bool IsAlive();
	virtual bool IsEnabled();
	virtual void AddRefCount();
	virtual void ReduceRefCount();
	void SetEnabled(bool enabled);
protected:
	~Object();
	void Dead();
private:
	int mCount;
protected:
	bool mEnabled;
};

