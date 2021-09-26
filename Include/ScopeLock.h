#pragma once

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


/*
创建系统互斥体 相同名称的互斥体不共存, 当参数2为FALSE时, 创建相同的互斥体时会返回FALSE, 当参数2为TRUE时, 创建相同的互斥体后, 会等待上一个互斥体释放(此方法可作为线程同步锁使用, 但效率较低, 一般作为不同进程间的文件读写锁使用)
使用方法:

	InstanceMutex MySystemMutex(L"MutexName",TRUE)//创建系统互斥体
	MySystemMutex.CreateInstanceMutex();
	......读写文件 / 操作文件 / 线程同步......
	MySystemMutex.DeleteInstanceMutex(); //释放互斥体
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

