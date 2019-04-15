#pragma once

//�ײ���󡪡���Ӧֱ�ӱ�ʹ�ã����Ǳ��̳дӶ�ʹ��
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

