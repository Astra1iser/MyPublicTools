#pragma once
#include <mutex>
#include <condition_variable>
using namespace std;
//�����߳���
/*	
	���������ٽ�������װ��

	ʹ�÷�����
	MyMutex m_writeLock;
	
	//����ʹ��(�߳�ͬ��)
	m_writeLock.Lock();
	.......  //ʹ�û�����Դ
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


//���������,һ�����ں�����ʹ��,������סһ��MyMutex���� Ȼ����ִ������Զ���������
/*
{
	MyScopeLock scopeLock(m_writeLock);
	... ...  //ʹ�û�����Դ
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


//�ź��� �ź�С��0������ ��������಻ͬ����,wait()��������ʲôʱ��(�����һ��ִ��)���������Լ�,�����ڱ�ĵط�������signal()����
/*
Semaphore sem;

sem.signal() //����

sem.wait(); //�����ȴ�,��ʱ����,ֻ�������ط�����signal()ʱ�Ž��в���,ע��ȴ������,�ȴ����ٴ� ����Ҫ���Ѷ��ٴ�

*/

class Semaphore
{
public:
	Semaphore(long count = 0) : count(count) {}
	//V����������
	void signal()
	{
		unique_lock<mutex> unique(mt);
		++count;
		if (count <= 0)
			cond.notify_one();
	}
	//P����������
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