#pragma once

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


/*
����ϵͳ������ ��ͬ���ƵĻ����岻����, ������2ΪFALSEʱ, ������ͬ�Ļ�����ʱ�᷵��FALSE, ������2ΪTRUEʱ, ������ͬ�Ļ������, ��ȴ���һ���������ͷ�(�˷�������Ϊ�߳�ͬ����ʹ��, ��Ч�ʽϵ�, һ����Ϊ��ͬ���̼���ļ���д��ʹ��)
ʹ�÷���:

	InstanceMutex MySystemMutex(L"MutexName",TRUE)//����ϵͳ������
	MySystemMutex.CreateInstanceMutex();
	......��д�ļ� / �����ļ� / �߳�ͬ��......
	MySystemMutex.DeleteInstanceMutex(); //�ͷŻ�����
*/	
class InstanceMutex
{
	public:
		InstanceMutex(CString MutexName,BOOL IsWait = FALSE) :MutexName(L"Global//" + MutexName), IsWait(IsWait), g_hMutex(NULL), MutexReleased(TRUE)
		{
			
		}
		
		BOOL CreateInstanceMutex(BOOL Waitting = FALSE)
		{
			if (FALSE == Waitting && NULL == g_hMutex && TRUE == MutexReleased)
			{
				g_hMutex = CreateMutex(NULL, TRUE, this->MutexName);
				MutexReleased = FALSE;
			}
				
			if (NULL != g_hMutex)
			{
				if (GetLastError() == ERROR_ALREADY_EXISTS && FALSE == Waitting)
				{
					if (TRUE == IsWait)
					{
						WaitForSingleObject(g_hMutex, INFINITE);
						if (CreateInstanceMutex(TRUE))
						{
							return TRUE;
						}
					}
					else
					{
						DeleteInstanceMutex();
						return FALSE;
						//exit(0);
					}
				}
				else
				{
					return TRUE;
				}
			}
			return FALSE;
		}

		BOOL DeleteInstanceMutex()
		{
			if (NULL != g_hMutex && FALSE == MutexReleased)
			{
				ReleaseMutex(g_hMutex);
				CloseHandle(g_hMutex);
				g_hMutex = NULL;
				MutexReleased = TRUE;
				return TRUE;
			}
			return FALSE;
		}

		~InstanceMutex()
		{
			if (!MutexReleased)
			{
				DeleteInstanceMutex();
			}	
		}

	private:
		CString MutexName;
		BOOL IsWait;
		HANDLE g_hMutex;
		BOOL MutexReleased;
};

