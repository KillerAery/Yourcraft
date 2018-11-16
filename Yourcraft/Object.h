#pragma once


class Object
{
public:
	Object();
	void Kill();
	virtual bool IsAlive();
	virtual bool IsEnabled();
	virtual void AddRefCount();
	virtual void ReduceRefCount();
	void SetEnabled(bool enabled);
protected:
	void Init();
	~Object();
	void Dead();
private:
	int mCount;
protected:
	bool mEnabled;
};

