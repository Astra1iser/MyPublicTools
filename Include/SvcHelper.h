#pragma once
#include <WinSvc.h>

class SvcHelper
{
public:
	SvcHelper(void);
	~SvcHelper(void);

	static int IsServiceStopped(const TCHAR* pszServiceName);
	static int StartServiceEx(const TCHAR* pszServiceName);
	static int PauseServiceEx(const TCHAR* pszServiceName);
	static int ContinueServiceEx(const TCHAR* pszServiceName);
	static int StopServiceEx(const TCHAR* pszServiceName);
	static int DeleteServiceEx(const TCHAR* pszServiceName);
	static int CreateServiceEx(const TCHAR* pszServiceName, const TCHAR* pszServiceDisplayName, const TCHAR* pszServiceFileName, const TCHAR* pcszServiceDes, BOOL bInterActive = TRUE);
	static int GetServiceInfo(const TCHAR* pszServiceName, DWORD& dwStartType, TCHAR* pszServiceFileName, int nLen);
	static int ChangServiceStartType(const TCHAR* pszServiceName, const DWORD dwStartType);
	static int QueryServiceStatus( const TCHAR* pszServiceName, LPSERVICE_STATUS lpServiceStatus );
};

