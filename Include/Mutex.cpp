#pragma once
#include <Mutex.h>

//CMutexSingle& CMutexSingle::GetInstance()
//{
//	static CMutexSingle oMutexSingle;
//	return oMutexSingle;
//}
//
//BOOL CMutexSingle::Lock(CString MutexName)
//{
//	if (NULL == m_hLockShareFileMap)
//	{
//		return FALSE;
//	}
//
//	BOOL bWaitSuccess = FALSE;
//	DWORD dwRetVal = WaitForSingleObject(m_hLockShareFileMap, INFINITE);
//	switch (dwRetVal)
//	{
//	case WAIT_OBJECT_0:
//	case WAIT_ABANDONED:
//	{
//		Init(MutexName);
//		bWaitSuccess = TRUE;
//	}
//	break;
//
//	case WAIT_TIMEOUT:
//		return FALSE;
//		break;
//	case WAIT_FAILED:
//		return FALSE;
//		break;
//	}
//
//	return bWaitSuccess;
//}
//
//void CMutexSingle::UnLock()
//{
//	if (NULL != m_hLockShareFileMap)
//	{
//		Uninit();
//	}
//}
//
//void CMutexSingle::Init(CString MutexName)
//{
//	if (NULL != m_hLockShareFileMap)
//	{
//		return;
//	}
//
//	SECURITY_ATTRIBUTES saa = { 0 };
//	saa.nLength = sizeof(SECURITY_ATTRIBUTES);
//	saa.bInheritHandle = FALSE;
//	saa.lpSecurityDescriptor = NULL;
//
//	BYTE arSD[SECURITY_DESCRIPTOR_MIN_LENGTH];
//	PSECURITY_DESCRIPTOR psd = reinterpret_cast<PSECURITY_DESCRIPTOR>(arSD);
//	if (::InitializeSecurityDescriptor(psd, SECURITY_DESCRIPTOR_REVISION))
//	{
//		if (::SetSecurityDescriptorDacl(psd, TRUE, NULL, FALSE))
//		{
//			saa.lpSecurityDescriptor = psd;
//		}
//	}
//	m_hLockShareFileMap = ::CreateMutex(&saa, FALSE, MutexName);  //Mutex的名字需要自己重新定义
//}
//
//void CMutexSingle::Uninit()
//{
//	if (NULL != m_hLockShareFileMap)
//	{
//		::ReleaseMutex(m_hLockShareFileMap);
//		CloseHandle(m_hLockShareFileMap);
//		m_hLockShareFileMap = NULL;
//	}
//}
//
//CMutexSingle::CMutexSingle() : m_hLockShareFileMap(NULL)
//{
//
//}
//
//CMutexSingle::~CMutexSingle()
//{
//	if (NULL != m_hLockShareFileMap)
//	{
//		Uninit();
//	}
//	delete this;
//}


BOOL  MutexLock::Lock(BOOL Waitting)
{
	//if (FALSE == Waitting && NULL == g_hMutex && TRUE == MutexReleased)
	//{
	//	g_hMutex = CreateMutex(NULL, TRUE, this->MutexName);
	//	MutexReleased = FALSE;
	//}

	//if (NULL != g_hMutex)
	//{
	//	if (GetLastError() == ERROR_ALREADY_EXISTS && FALSE == Waitting)
	//	{
	//		if (TRUE == NeedWait)
	//		{
	//			WaitForSingleObject(g_hMutex, INFINITE);
	//			if (Lock(TRUE))
	//			{
	//				return TRUE;
	//			}
	//		}
	//		else
	//		{
	//			Unlock();
	//			return FALSE;
	//			//exit(0);
	//		}
	//	}
	//	else
	//	{
	//		return TRUE;
	//	}
	//}
	//return FALSE;



	m_hMutex = NULL;

	if (FALSE == NeedWait)
	{
		if (NULL == g_hMutex && TRUE == MutexReleased)
		{
			g_hMutex = CreateMutex(NULL, TRUE, this->MutexName);
			m_hMutex = g_hMutex;
			MutexReleased = FALSE;
		}
		if (NULL == m_hMutex)
		{
			return FALSE;
		}
		else
		{
			return TRUE;
		}
	}

	if (TRUE == NeedWait)
	{
		m_hMutex = CreateMutex(NULL, TRUE, this->MutexName);
		if (GetLastError() == ERROR_ALREADY_EXISTS)
		{
			if (WaitForSingleObject(m_hMutex, INFINITE) == WAIT_OBJECT_0)
			{
				Unlock(m_hMutex);
				return FALSE;
			}
			else
			{
				g_hMutex = m_hMutex;
				MutexReleased = FALSE;
				return TRUE;
			}
		}
		else
		{
			if (NULL == m_hMutex)
			{
				return FALSE;
			}
			else
			{
				g_hMutex = m_hMutex;
				MutexReleased = FALSE;
				return TRUE;
			}
		}
	}
}

BOOL  MutexLock::Unlock(HANDLE hd)
{
	if (NULL != hd)
	{
		ReleaseMutex(hd);
		CloseHandle(hd);
		return TRUE;
	}

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

MutexLock::~MutexLock()
{
	if (!MutexReleased)
	{
		Unlock();
	}
}