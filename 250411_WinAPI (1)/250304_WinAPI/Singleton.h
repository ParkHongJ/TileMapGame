// Singleton.h
#pragma once

template<typename T>
class Singleton
{
protected:
	static T* instance;

	Singleton() {};
	~Singleton() {};

public:
	static T* GetInstance();
	void ReleaseInstance();
};

/*
	링크 과정에서 중복 정의를 방지, 각 템플릿 인스턴스에
	대한 스태틱 변수가 고유하게 존재하도록 보장
*/
template<typename T>
T* Singleton<T>::instance = nullptr;

template<typename T>
inline T* Singleton<T>::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new T;
	}
	return instance;
}

template<typename T>
inline void Singleton<T>::ReleaseInstance()
{
	if (instance)
	{
		delete instance;
		instance = nullptr;
	}
}
