#pragma once
#include <mutex>
#include <condition_variable>
using namespace std;
//常用线程锁
/*	
	互斥锁（临界区）封装类

	使用方法：
	MyMutex m_writeLock;
	
	//互斥使用(线程同步)
	m_writeLock.Lock();
	.......  //使用互斥资源
	m_writeLock.Unlock();
*/
class MyMutex
{

public:
	CRITICAL_SECTION m_section;

	MyMutex()
	{
		InitializeCriticalSection(&m_section);
	};

	~MyMutex()
	{
		DeleteCriticalSection(&m_section);
	};

	BOOL IsLocked()
	{
		if( m_section.OwningThread )
			return TRUE;
		else
			return FALSE;

	};

	void Lock()
	{
		EnterCriticalSection(&m_section);
	}

	BOOL TryLock()
	{
		return TryEnterCriticalSection(&m_section);
	}

	void Unlock()
	{
		LeaveCriticalSection(&m_section);
	}
};


//这个不常用,一般是在函数内使用,就是锁住一个MyMutex对象 然后函数执行完后自动析构开锁
/*
{
	MyScopeLock scopeLock(m_writeLock);
	... ...  //使用互斥资源
}
*/
class MyScopeLock
{
public:
	MyScopeLock(CRITICAL_SECTION *pcs)
	{
		m_pcs = pcs;
		EnterCriticalSection(m_pcs);
	};

	MyScopeLock(MyMutex &mutex)
	{
		m_pcs = &mutex.m_section;
		EnterCriticalSection(m_pcs);
	};

	~MyScopeLock()
	{
		LeaveCriticalSection(m_pcs);
	};

private:
	CRITICAL_SECTION *m_pcs;
};


//信号量 信号小于0会阻塞 与上面的类不同的是,wait()函数无论什么时候(就算第一次执行)都会阻塞自己,除非在别的地方进行了signal()操作
/*
Semaphore sem;

sem.signal() //唤醒

sem.wait(); //上锁等待,此时阻塞,只有其他地方进行signal()时才进行操作,注意等待会计数,等待多少次 就需要唤醒多少次

*/

class Semaphore
{
public:
	Semaphore(long count = 0) : count(count) {}
	//V操作，唤醒
	void signal()
	{
		unique_lock<mutex> unique(mt);
		++count;
		if (count <= 0)
			cond.notify_one();
	}
	//P操作，阻塞
	void wait()
	{
		unique_lock<mutex> unique(mt);
		--count;
		if (count < 0)
			cond.wait(unique);
	}

private:
	mutex mt;
	condition_variable cond;
	long count;
};