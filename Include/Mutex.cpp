#pragma once
#include <Mutex.h>

CMutexSingle& CMutexSingle::GetInstance()
{
	static CMutexSingle oMutexSingle;
	return oMutexSingle;
}

BOOL CMutexSingle::Lock(CString MutexName)
{
	if (NULL == m_hLockShareFileMap)
	{
		return FALSE;
	}

	BOOL bWaitSuccess = FALSE;
	DWORD dwRetVal = WaitForSingleObject(m_hLockShareFileMap, INFINITE);
	switch (dwRetVal)
	{
	case WAIT_OBJECT_0:
	case WAIT_ABANDONED:
	{
		Init(MutexName);
		bWaitSuccess = TRUE;
	}
	break;

	case WAIT_TIMEOUT:
		return FALSE;
		break;
	case WAIT_FAILED:
		return FALSE;
		break;
	}

	return bWaitSuccess;
}

void CMutexSingle::UnLock()
{
	if (NULL != m_hLockShareFileMap)
	{
		Uninit();
	}
}

void CMutexSingle::Init(CString MutexName)
{
	if (NULL != m_hLockShareFileMap)
	{
		return;
	}

	SECURITY_ATTRIBUTES saa = { 0 };
	saa.nLength = sizeof(SECURITY_ATTRIBUTES);
	saa.bInheritHandle = FALSE;
	saa.lpSecurityDescriptor = NULL;

	BYTE arSD[SECURITY_DESCRIPTOR_MIN_LENGTH];
	PSECURITY_DESCRIPTOR psd = reinterpret_cast<PSECURITY_DESCRIPTOR>(arSD);
	if (::InitializeSecurityDescriptor(psd, SECURITY_DESCRIPTOR_REVISION))
	{
		if (::SetSecurityDescriptorDacl(psd, TRUE, NULL, FALSE))
		{
			saa.lpSecurityDescriptor = psd;
		}
	}
	m_hLockShareFileMap = ::CreateMutex(&saa, FALSE, MutexName);  //Mutex的名字需要自己重新定义
}

void CMutexSingle::Uninit()
{
	if (NULL != m_hLockShareFileMap)
	{
		::ReleaseMutex(m_hLockShareFileMap);
		CloseHandle(m_hLockShareFileMap);
		m_hLockShareFileMap = NULL;
	}
}

CMutexSingle::CMutexSingle() : m_hLockShareFileMap(NULL)
{

}

CMutexSingle::~CMutexSingle()
{
	if (NULL != m_hLockShareFileMap)
	{
		Uninit();
	}
	delete this;
}