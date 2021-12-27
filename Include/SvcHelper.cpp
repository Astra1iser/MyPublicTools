#include "StdAfx.h"
#include "SvcHelper.h"

SvcHelper::SvcHelper(void)
{
}

SvcHelper::~SvcHelper(void)
{
}

int SvcHelper::IsServiceStopped(const TCHAR* pszServiceName)
{
	if (NULL == pszServiceName || _tcslen(pszServiceName) <= 0)
		return FALSE;

	int nResult = FALSE;
	int nRetCode = FALSE;

	SC_HANDLE hSCHandle = NULL;
	SC_HANDLE hSCManager = NULL;
	SERVICE_STATUS theStatus = { 0 };

	do 
	{
		hSCManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (NULL == hSCManager)
			break;

		hSCHandle = ::OpenService(hSCManager, pszServiceName, SERVICE_ALL_ACCESS);
		if (NULL == hSCHandle)
			break;

		nRetCode = ::QueryServiceStatus(hSCHandle, &theStatus);
		if (!nRetCode)
			break;

		if (SERVICE_STOPPED == theStatus.dwCurrentState)
			nResult = TRUE;

	} while (FALSE);


	if (hSCHandle)
	{
		CloseServiceHandle(hSCHandle);
		hSCHandle = NULL;
	}

	if (hSCManager)
	{
		CloseServiceHandle(hSCManager);
		hSCManager = NULL;
	}

	return nResult;
}

int SvcHelper::StartServiceEx(const TCHAR* pszServiceName)
{
	if (NULL == pszServiceName || _tcslen(pszServiceName) <= 0)
		return FALSE;

	int nResult = FALSE;
	int nRetCode = FALSE;
	int nTimeMsecs = 0;

	SC_HANDLE hSCHandle = NULL;
	SC_HANDLE hSCManager = NULL;
	SERVICE_STATUS theStatus = { 0 };

	do 
	{
		hSCManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (NULL == hSCManager)
			break;

		hSCHandle = ::OpenService(hSCManager, pszServiceName, SERVICE_ALL_ACCESS);
		if (NULL == hSCHandle)
			break;

		nRetCode = ::QueryServiceStatus(hSCHandle, &theStatus);
		if (!nRetCode)
			break;

		if (theStatus.dwCurrentState == SERVICE_RUNNING)
		{
			nResult = TRUE;
			break;
		}

		nRetCode = ::StartService(hSCHandle, 0, NULL);
		if (!nRetCode)
			break;

		do
		{
			nRetCode = ::QueryServiceStatus(hSCHandle, &theStatus);
			if (!nRetCode)
				break;

			if (theStatus.dwCurrentState != SERVICE_START_PENDING)
				break;

			::Sleep(100); 
			nTimeMsecs += 100;

		} while(nTimeMsecs < 30 * 1000);

		if (theStatus.dwCurrentState == SERVICE_RUNNING)
			nResult = TRUE;

	} while (FALSE);

	if (hSCHandle)
	{
		CloseServiceHandle(hSCHandle);
		hSCHandle = NULL;
	}

	if (hSCManager)
	{
		CloseServiceHandle(hSCManager);
		hSCManager = NULL;
	}

	return nResult;
}

int SvcHelper::StopServiceEx(const TCHAR* pszServiceName)
{
	if (NULL == pszServiceName || _tcslen(pszServiceName) <= 0)
		return FALSE;

	int nResult = FALSE;
	int nRetCode = FALSE;
	int nTimeMsecs = 0;

	SC_HANDLE hSCHandle = NULL;
	SC_HANDLE hSCManager = NULL;
	SERVICE_STATUS theStatus = { 0 };

	do 
	{
		hSCManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (NULL == hSCManager)
			break;

		hSCHandle = ::OpenService(hSCManager, pszServiceName, SERVICE_ALL_ACCESS);
		if (NULL == hSCHandle)
			break;

		nRetCode = ::QueryServiceStatus(hSCHandle, &theStatus);
		if (!nRetCode)
			break;

		if (theStatus.dwCurrentState == SERVICE_STOPPED)
		{
			nResult = TRUE;
			break;
		}

		do
		{
			nRetCode = ControlService(hSCHandle, SERVICE_CONTROL_STOP, &theStatus);

			nRetCode = ::QueryServiceStatus(hSCHandle, &theStatus);
			if (!nRetCode)
				break;

			if (theStatus.dwCurrentState != SERVICE_STOP_PENDING)
				break;

			::Sleep(100); 
			nTimeMsecs += 100;

		} while(nTimeMsecs < 30 * 1000);

		if (theStatus.dwCurrentState == SERVICE_STOPPED)
			nResult = TRUE;

	} while (FALSE);

	if (hSCHandle)
	{
		CloseServiceHandle(hSCHandle);
		hSCHandle = NULL;
	}

	if (hSCManager)
	{
		CloseServiceHandle(hSCManager);
		hSCManager = NULL;
	}

	return nResult;
}

int SvcHelper::DeleteServiceEx(const TCHAR* pszServiceName)
{
	if (NULL == pszServiceName || _tcslen(pszServiceName) <= 0)
		return FALSE;

	int nResult = FALSE;
	int nRetCode = FALSE;

	SC_HANDLE hScManagerHandle = NULL;
	SC_HANDLE hOpenServiceHandle = NULL;

	TCHAR szServiceKey[260] = _T("SYSTEM\\CurrentControlSet\\Services\\");

	do 
	{
		hScManagerHandle = ::OpenSCManager(
			NULL,
			NULL,
			GENERIC_WRITE);
		if (NULL == hScManagerHandle)
			break;

		hOpenServiceHandle = ::OpenService(
			hScManagerHandle,
			pszServiceName,
			SERVICE_ALL_ACCESS);
		if (NULL == hOpenServiceHandle)
		{
			nResult = TRUE;
			break;
		}

		nRetCode = StopServiceEx(pszServiceName);
		if (!nRetCode)
			break;

		nRetCode = ::DeleteService(hOpenServiceHandle);
		if (!nRetCode)
			break;

		nResult = TRUE;
		Sleep(1000);

	} while (FALSE);

	if (hOpenServiceHandle)
	{
		CloseServiceHandle(hOpenServiceHandle);
		hOpenServiceHandle = NULL;
	}

	if (hScManagerHandle)
	{
		CloseServiceHandle(hScManagerHandle);
		hScManagerHandle = NULL;
	}

	_tcsncat(szServiceKey, pszServiceName, sizeof(szServiceKey));
	::RegDeleteKey(
		HKEY_LOCAL_MACHINE,
		szServiceKey);

	return nResult;
}

int SvcHelper::CreateServiceEx(const TCHAR* pszServiceName, const TCHAR* pszServiceDisplayName, const TCHAR* pszServiceFileName, const TCHAR* pcszServiceDes, BOOL bInterActive)
{
	if (NULL == pszServiceName || _tcslen(pszServiceName) <= 0)
		return FALSE;

	int nResult = FALSE;

	SC_HANDLE hScManagerHandle = NULL;
	SC_HANDLE hOpenServiceHandle = NULL;
	DWORD dwServiceType = SERVICE_WIN32_OWN_PROCESS;
	if (bInterActive) 
		dwServiceType |= SERVICE_INTERACTIVE_PROCESS;	

	do 
	{
		hScManagerHandle = OpenSCManager(
			NULL,
			NULL,
			GENERIC_WRITE);
		if (NULL == hScManagerHandle)	
			break;

		hOpenServiceHandle = ::OpenService(
			hScManagerHandle,
			pszServiceName,
			SERVICE_ALL_ACCESS);
		if (NULL != hOpenServiceHandle)
		{
			nResult = TRUE;
			break;
		}

		hOpenServiceHandle = ::CreateService(
			hScManagerHandle,
			pszServiceName,
			pszServiceDisplayName,
			SERVICE_ALL_ACCESS,
			dwServiceType,
			SERVICE_AUTO_START,
			SERVICE_ERROR_NORMAL,
			pszServiceFileName,
			NULL,
			NULL,
			NULL,
			NULL,
			NULL);
		if (NULL == hOpenServiceHandle)
			break;

		if (NULL != pcszServiceDes && _tcslen(pcszServiceDes) > 0 && _tcslen(pcszServiceDes) < MAX_PATH)
		{
			TCHAR szDesc[MAX_PATH] = { 0 };
			SERVICE_DESCRIPTION sd = { 0 };

			_tcscpy(szDesc, pcszServiceDes);
			sd.lpDescription = szDesc;

			::ChangeServiceConfig2(hOpenServiceHandle, SERVICE_CONFIG_DESCRIPTION, &sd);
		}

		nResult = TRUE;
		
	} while (FALSE);
	

	if (hOpenServiceHandle)
	{
		CloseServiceHandle(hOpenServiceHandle);
		hOpenServiceHandle = NULL;
	}

	if (hScManagerHandle)
	{
		CloseServiceHandle(hScManagerHandle);
		hScManagerHandle = NULL;
	}

	return nResult;
}

int SvcHelper::GetServiceInfo(const TCHAR* pszServiceName, DWORD& dwStartType,TCHAR* pszServiceFileName, int nLen)
{
	if (NULL == pszServiceName || _tcslen(pszServiceName) <= 0)
		return FALSE;

	int nResult = FALSE;
	int nRetCode = FALSE;

	DWORD dwBytesNeeded = 0;
	SC_HANDLE hSCHandle = NULL;
	SC_HANDLE hSCManager = NULL;
	SERVICE_STATUS theStatus = { 0 };
	LPQUERY_SERVICE_CONFIG lpsc = NULL; 

	dwStartType = 0;
	do 
	{
		hSCManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (NULL == hSCManager)
			break;

		hSCHandle = ::OpenService(hSCManager, pszServiceName, SERVICE_ALL_ACCESS);
		if (NULL == hSCHandle)
			break;

		if(!QueryServiceConfig( 
			hSCHandle, 
			NULL, 
			0, 
			&dwBytesNeeded))
		{
			if( ERROR_INSUFFICIENT_BUFFER != ::GetLastError())
				break;	

			lpsc = (LPQUERY_SERVICE_CONFIG)LocalAlloc(LMEM_FIXED, dwBytesNeeded);		
		}

		if (NULL == lpsc)
			break;

		memset(lpsc, 0, dwBytesNeeded);
		if(!QueryServiceConfig( 
			hSCHandle, 
			lpsc, 
			dwBytesNeeded, 
			&dwBytesNeeded)) 
			break;

		if (NULL != pszServiceFileName)
		{
			if ((int)(_tcslen(lpsc->lpBinaryPathName)) >= nLen)
				break;

			_tcscpy(pszServiceFileName, lpsc->lpBinaryPathName);
		}

		dwStartType = lpsc->dwStartType;
		nResult = TRUE;

	} while (FALSE);

	if (NULL != lpsc)
	{
		LocalFree(lpsc); 
		lpsc = NULL;
	}

	if (hSCHandle)
	{
		CloseServiceHandle(hSCHandle);
		hSCHandle = NULL;
	}

	if (hSCManager)
	{
		CloseServiceHandle(hSCManager);
		hSCManager = NULL;
	}

	return nResult;

}

int SvcHelper::PauseServiceEx(const TCHAR* pszServiceName)
{
	if (NULL == pszServiceName || _tcslen(pszServiceName) <= 0)
		return FALSE;

	int nResult = FALSE;
	int nRetCode = FALSE;
	int nTimeMsecs = 0;

	SC_HANDLE hSCHandle = NULL;
	SC_HANDLE hSCManager = NULL;
	SERVICE_STATUS theStatus = { 0 };

	do 
	{
		hSCManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (NULL == hSCManager)
			break;

		hSCHandle = ::OpenService(hSCManager, pszServiceName, SERVICE_ALL_ACCESS);
		if (NULL == hSCHandle)
			break;

		nRetCode = ::QueryServiceStatus(hSCHandle, &theStatus);
		if (!nRetCode)
			break;		

		do
		{
			nRetCode = ControlService(hSCHandle, SERVICE_CONTROL_PAUSE, &theStatus);

			nRetCode = ::QueryServiceStatus(hSCHandle, &theStatus);
			if (!nRetCode)
				break;

			if (theStatus.dwCurrentState != SERVICE_PAUSE_PENDING)
				break;

			::Sleep(100); 
			nTimeMsecs += 100;

		} while(nTimeMsecs < 30 * 1000);

		if (theStatus.dwCurrentState == SERVICE_PAUSED)
			nResult = TRUE;

	} while (FALSE);

	if (hSCHandle)
	{
		CloseServiceHandle(hSCHandle);
		hSCHandle = NULL;
	}

	if (hSCManager)
	{
		CloseServiceHandle(hSCManager);
		hSCManager = NULL;
	}

	return nResult;
}

int SvcHelper::ContinueServiceEx(const TCHAR* pszServiceName)
{
	if (NULL == pszServiceName || _tcslen(pszServiceName) <= 0)
		return FALSE;

	int nResult = FALSE;
	int nRetCode = FALSE;
	int nTimeMsecs = 0;

	SC_HANDLE hSCHandle = NULL;
	SC_HANDLE hSCManager = NULL;
	SERVICE_STATUS theStatus = { 0 };

	do 
	{
		hSCManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (NULL == hSCManager)
			break;

		hSCHandle = ::OpenService(hSCManager, pszServiceName, SERVICE_ALL_ACCESS);
		if (NULL == hSCHandle)
			break;

		nRetCode = ::QueryServiceStatus(hSCHandle, &theStatus);
		if (!nRetCode)
			break;		

		do
		{
			nRetCode = ControlService(hSCHandle, SERVICE_CONTROL_CONTINUE, &theStatus);

			nRetCode = ::QueryServiceStatus(hSCHandle, &theStatus);
			if (!nRetCode)
				break;

			if (theStatus.dwCurrentState != SERVICE_CONTINUE_PENDING)
				break;

			::Sleep(100); 
			nTimeMsecs += 100;

		} while(nTimeMsecs < 30 * 1000);

		if (theStatus.dwCurrentState == SERVICE_RUNNING)
			nResult = TRUE;

	} while (FALSE);

	if (hSCHandle)
	{
		CloseServiceHandle(hSCHandle);
		hSCHandle = NULL;
	}

	if (hSCManager)
	{
		CloseServiceHandle(hSCManager);
		hSCManager = NULL;
	}

	return nResult;
}

int SvcHelper::ChangServiceStartType(const TCHAR* pszServiceName, const DWORD dwStartType)
{
	if (NULL == pszServiceName || _tcslen(pszServiceName) <= 0 || dwStartType <= SERVICE_BOOT_START || dwStartType > SERVICE_DISABLED)
		return FALSE;

	int nResult = FALSE;
	int nRetCode = FALSE;

	DWORD dwBytesNeeded = 0;
	SC_HANDLE hSCHandle = NULL;
	SC_HANDLE hSCManager = NULL;
	SERVICE_STATUS theStatus = { 0 };
	LPQUERY_SERVICE_CONFIG lpsc = NULL; 

	do 
	{
		hSCManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (NULL == hSCManager)
			break;

		hSCHandle = ::OpenService(hSCManager, pszServiceName, SERVICE_ALL_ACCESS);
		if (NULL == hSCHandle)
			break;

		if(!QueryServiceConfig( 
			hSCHandle, 
			NULL, 
			0, 
			&dwBytesNeeded))
		{
			if( ERROR_INSUFFICIENT_BUFFER != ::GetLastError())
				break;	

			lpsc = (LPQUERY_SERVICE_CONFIG)LocalAlloc(LMEM_FIXED, dwBytesNeeded);		
		}

		if (NULL == lpsc)
			break;

		memset(lpsc, 0, dwBytesNeeded);
		if(!QueryServiceConfig( 
			hSCHandle, 
			lpsc, 
			dwBytesNeeded, 
			&dwBytesNeeded)) 
			break;

		if (dwStartType == lpsc->dwStartType)
		{
			nResult = TRUE;
			break;
		}

		if (!ChangeServiceConfig( 
			hSCHandle,        // handle of service 
			SERVICE_NO_CHANGE, // service type: no change 
			dwStartType,  // service start type 
			SERVICE_NO_CHANGE, // error control: no change 
			NULL,              // binary path: no change 
			NULL,              // load order group: no change 
			NULL,              // tag ID: no change 
			NULL,              // dependencies: no change 
			NULL,              // account name: no change 
			NULL,              // password: no change 
			NULL))
			break;
		
		nResult = TRUE;

	} while (FALSE);

	if (NULL != lpsc)
	{
		LocalFree(lpsc); 
		lpsc = NULL;
	}

	if (hSCHandle)
	{
		CloseServiceHandle(hSCHandle);
		hSCHandle = NULL;
	}

	if (hSCManager)
	{
		CloseServiceHandle(hSCManager);
		hSCManager = NULL;
	}

	return nResult;

}

int SvcHelper::QueryServiceStatus( const TCHAR* pszServiceName, LPSERVICE_STATUS lpServiceStatus )
{
	int nResult = 0;
	int nRetCode = FALSE;

	SC_HANDLE hSCHandle = NULL;
	SC_HANDLE hSCManager = NULL;

	if (pszServiceName == NULL || lpServiceStatus == NULL)
	{
		return nResult;
	}

	do 
	{
		hSCManager = ::OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
		if (NULL == hSCManager)
			break;

		hSCHandle = ::OpenService(hSCManager, pszServiceName, SERVICE_ALL_ACCESS);
		if (NULL == hSCHandle)
			break;

		nRetCode = ::QueryServiceStatus(hSCHandle, lpServiceStatus);
		if (!nRetCode)
			break;
		
		nResult = TRUE;

	} while (FALSE);

	if (hSCHandle)
	{
		CloseServiceHandle(hSCHandle);
		hSCHandle = NULL;
	}

	if (hSCManager)
	{
		CloseServiceHandle(hSCManager);
		hSCManager = NULL;
	}

	return nResult;
}
