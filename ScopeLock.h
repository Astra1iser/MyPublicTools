#pragma once

/*	
	v1.0 by kage
	互斥锁（临界区）封装类

	使用方法：
	MyMutex m_writeLock;
	
	//互斥使用
	m_writeLock.Lock();
	.......  //使用互斥资源
	m_writeLock.Unlock();


	{
		MyScopeLock scopeLock(m_writeLock);
		... ...  //使用互斥资源
	}
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

