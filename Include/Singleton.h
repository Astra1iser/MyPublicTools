#pragma once

/*
	单实例封装类

	使用方法：
	class CSingletonTest
	{
	public:
		CSingletonTest():a(0)
		{

		}
		int a;
		void Init();
	};

	typedef SimpleLoki::SingletonHolder<CSingletonTest> SingletonTest1;
	typedef SimpleLoki::SingletonHolder<CSingletonTest> SingletonTest2;

	//调用方法
	SingletonTest1::Instance().Init();


	使用此类的目的:
	SingletonTest1::Instance().a = 100;
	SingletonTest2::Instance().a = 101;

	cout<< SingletonTest::Instance().a;

	此时输出的是101,即SingletonTest1和SingletonTest2其实是同一块内存,只是名字不同,所以,把一个类声明在一个头文件中,并给这个类声明一个单实例名,就可以在任意引用这个头文件的cpp中调用同一个实例对象
*/
#ifndef _SINGLETON_H_
#define _SINGLETON_H_

class ILockerModeEx
{
public:
	virtual void Lock(void) = 0;
	virtual void Ulock(void) = 0;
};


class LockerCSEx : public ILockerModeEx
{
public:
	LockerCSEx() { ::InitializeCriticalSection(&m_CS); }
	~LockerCSEx() { ::DeleteCriticalSection(&m_CS); }

	void Lock(void) 
	{	
		::EnterCriticalSection(&m_CS); 
	}

	void Ulock(void) 
	{ 
		::LeaveCriticalSection(&m_CS); 
	}

private:
	CRITICAL_SECTION m_CS;
};

class LockerEx
{
public:
	LockerEx(ILockerModeEx* pLock)
	{
		m_pLock = pLock;
		if (m_pLock != NULL)		{
			m_pLock->Lock(); 
		}

	}

	~LockerEx(void)
	{
		if (m_pLock != NULL)		m_pLock->Ulock();	
	}

private:
	ILockerModeEx* m_pLock;
};


namespace SimpleLoki
{
	typedef void (*atexit_pfn_t)();

	template <class T>
	struct DefaultLifetime
	{
		static void ScheduleDestruction(T*, atexit_pfn_t pFun)
		{ std::atexit(pFun); }

		static void OnDeadReference()
		{ throw std::logic_error("Dead Reference Detected"); }
	};

	template<typename T>
	class SingletonHolder
	{
	public:

		typedef T ObjectType;

		static T& Instance();

		static LockerCSEx* GetLock()
		{
			return &m_lockCS;
		}

	private:
		static void MakeInstance();
		static void  DestroySingleton();

		SingletonHolder();

		typedef typename T* PtrInstanceType;
		static PtrInstanceType pInstance_;
		static bool destroyed_;
		static LockerCSEx m_lockCS;
	};

	template<typename T>
	void SimpleLoki::SingletonHolder<T>::DestroySingleton()
	{
		if (pInstance_)
		{
			delete pInstance_;
			pInstance_ = NULL;
			destroyed_ = true;
		}
	}

	template<typename T>
	void SingletonHolder<T>::MakeInstance()
	{
		LockerEx locker(&m_lockCS);

		if (!pInstance_)
		{
			if (destroyed_)
			{
				destroyed_ = false;
				DefaultLifetime<T>::OnDeadReference();
			}

			pInstance_ = new T();
			DefaultLifetime<T>::ScheduleDestruction(pInstance_, &DestroySingleton);
		}
	}

	template<typename T>
	T& SingletonHolder<T>::Instance()
	{
		if (!pInstance_)
		{
			MakeInstance();
		}
		return *pInstance_;
	}

	template<typename T>
	typename SingletonHolder<T>::PtrInstanceType  SingletonHolder<T>::pInstance_ = NULL;

	template<typename T>
	bool SingletonHolder<T>::destroyed_;

	template<typename T>
	LockerCSEx SingletonHolder<T>::m_lockCS;

};
#endif