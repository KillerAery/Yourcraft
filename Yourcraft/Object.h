#pragma once

class Object
{
public:
	Object();
	void Kill();
	bool IsAlive();
	bool IsEnabled();
protected:
	~Object();
	virtual void Init();
	void Dead();
protected:
	bool mAlive;
	bool mEnabled;
};

