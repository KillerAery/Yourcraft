#pragma once
#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#include <iostream>

//������ģ��

template<class T>
class Singleton{
private:
	static T * s_instance;
public:
	static T * Instance();
	static bool DeleteInstance();
	
};

template<class T>
T * Singleton<T>::Instance() {
	if (!s_instance)
	{
		s_instance = new (std::nothrow)T();
		//�����ʼ��ʧ�ܣ���ɾ��
		if (s_instance && !s_instance->Init())
		{
			delete s_instance;
			s_instance = nullptr;
		}
	}
	return s_instance;
}

template<class T>
bool Singleton<T>::DeleteInstance() {
	if (s_instance) {
		delete s_instance;
		s_instance = nullptr;
		return true;
	}
	else return false;
}

template<class T>
T* Singleton<T>::s_instance = nullptr;


#endif // !_SINGLETON_H_
