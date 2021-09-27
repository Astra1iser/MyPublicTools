#pragma once

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


//���������
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


