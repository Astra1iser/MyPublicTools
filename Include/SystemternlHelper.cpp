#include "stdafx.h"
#include <OSChecker.cpp>
#include "SystemternlHelper.h"
#include <strsafe.h>
#include <Psapi.h>
#include <Tlhelp32.h>
#include <UserEnv.h>

#pragma comment(lib, "Psapi.lib")
#pragma comment(lib,"Userenv.lib")

#ifndef WRITE_LOG
#define WRITE_LOG
#endif

CString SystemternlHelper::DosPathToRealPath(LPCTSTR pcszSrc)
{
	if(pcszSrc == NULL)
		return _T("");

	TCHAR szBuf[1024] = {0};
	if(!GetLogicalDriveStrings(1024-1, szBuf)) 
		return _T("");

	TCHAR szDrive[3] = _T(" :");
	UINT uSrcLen = _tcslen(pcszSrc);

	for (TCHAR* p = szBuf; *p; p += _tcslen(p)+1)
	{
		*szDrive = *p;
		TCHAR szName[MAX_PATH] = {0};
		if(QueryDosDevice(szDrive, szName, MAX_PATH - 1))
		{
			UINT uNameLen = _tcslen(szName);
			if(uNameLen && uNameLen < uSrcLen && _tcsnicmp(pcszSrc,szName,uNameLen) == 0) 
			{
				TCHAR szTmp[MAX_PATH] = {0};
				_tcscpy_s(szTmp,MAX_PATH - 1,szDrive);
				_tcscat_s(szTmp,pcszSrc + uNameLen);
				return szTmp;
			}
		}
	}
	return _T("");
}

int SystemternlHelper::EnablePrivilege(LPCTSTR lpszPrivilegeName, BOOL bEnable)
{
	int nResult = FALSE;
	int nRetCode = FALSE;
	HANDLE hToken = NULL;
	TOKEN_PRIVILEGES tkp = { 0 };

	do 
	{
		nRetCode = ::OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
		if (!nRetCode)
			break;

		nRetCode = ::LookupPrivilegeValue(NULL, lpszPrivilegeName, &tkp.Privileges[0].Luid);
		if (!nRetCode)
			break;

		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
		nRetCode = ::AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL);
		if (!nRetCode)
			break;

		nResult = TRUE;
	} while (FALSE);

	if (hToken != NULL)
	{
		CloseHandle(hToken);
	}

	return nResult;
}

BOOL SystemternlHelper::GetProcessSession(DWORD dwProcessID,DWORD& dwSessionId)
{
	EnablePrivilege(SE_DEBUG_NAME,TRUE);

	HANDLE hProcess = NULL;
	HANDLE hToken = NULL;
	BOOL bRet = FALSE;

	do
	{
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ|PROCESS_TERMINATE , FALSE, dwProcessID);
		if(!hProcess || INVALID_HANDLE_VALUE == hProcess)
			break;

		if(OpenProcessToken(hProcess,TOKEN_QUERY,&hToken))
		{
			DWORD dwReturnLen = 0;
			DWORD dwTokenSessionId = 0;
			if (!::GetTokenInformation(hToken, TokenSessionId, &dwTokenSessionId, sizeof(DWORD), &dwReturnLen))
				break;

			dwSessionId = dwTokenSessionId;
			bRet = TRUE;
		}

	}while(FALSE);
	
	if(hToken && INVALID_HANDLE_VALUE != hToken)
		CloseHandle(hToken);

	if(hProcess && INVALID_HANDLE_VALUE != hProcess)
		CloseHandle(hProcess);

	return bRet;
}

BOOL SystemternlHelper::GetProcessSession(HANDLE hProcess,CString &strSession )
{
	strSession = L"";
	EnablePrivilege(SE_DEBUG_NAME,TRUE);

	HANDLE hToken = NULL;
	if(OpenProcessToken(hProcess,TOKEN_QUERY,&hToken))
	{
		TOKEN_USER *tokenuser = NULL;
		DWORD dwLen = 0;
		if (GetTokenInformation(hToken, TokenUser, NULL, 0, &dwLen) || GetLastError() == 122)
		{
			tokenuser = (TOKEN_USER*)malloc(dwLen);
			if (tokenuser)
			{
				if (GetTokenInformation(hToken, TokenUser, tokenuser, dwLen, &dwLen))
				{
					TCHAR tczBuf[2048] = {0};
					dwLen = _countof(tczBuf)-1;
					TCHAR tczDoman[2048] = {0};
					DWORD dw = _countof(tczDoman)-1;
					SID_NAME_USE ut;
					LookupAccountSid(NULL,tokenuser->User.Sid,tczBuf,&dwLen,tczDoman,&dw,&ut);

					strSession = tczBuf;
				}
			}
			free(tokenuser);
		}
		CloseHandle(hToken);
	}

	return !strSession.IsEmpty();
};

BOOL SystemternlHelper::GetProcessSessionID(HANDLE hProcess,CString &strSessionID)
{
	strSessionID = L"";
	EnablePrivilege(SE_DEBUG_NAME,TRUE);

	HANDLE hToken = NULL;
	if(OpenProcessToken(hProcess,TOKEN_QUERY,&hToken))
	{
		TOKEN_USER *tokenuser = NULL;
		DWORD dwLen = 0;
		if (GetTokenInformation(hToken, TokenUser, NULL, 0, &dwLen) || GetLastError() == 122)
		{
			tokenuser = (TOKEN_USER*)malloc(dwLen);
			if (tokenuser)
			{
				if (GetTokenInformation(hToken, TokenUser, tokenuser, dwLen, &dwLen))
				{
					typedef BOOL (_stdcall *PtrConvertSidToStringSid)(PSID Sid,LPTSTR *StringSid);

					PtrConvertSidToStringSid dwPtr = (PtrConvertSidToStringSid)GetProcAddress(   
						LoadLibrary(L"Advapi32.dll"), "ConvertSidToStringSidW" );  

					LPTSTR MySid = NULL;  
					dwPtr( tokenuser->User.Sid, &MySid );
					strSessionID=MySid;

					LocalFree( (HLOCAL)MySid );
				}
			}
			free(tokenuser);
		}
		CloseHandle(hToken);
	}

	return !strSessionID.IsEmpty();
}

BOOL SystemternlHelper::GetProcessSession(DWORD dwProcessID,CString &strSession )
{
	strSession = L"";
	EnablePrivilege(SE_DEBUG_NAME,TRUE);

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ|PROCESS_TERMINATE , FALSE, dwProcessID);
	BOOL bRet = GetProcessSession(hProcess,strSession);
	if(hProcess && INVALID_HANDLE_VALUE != hProcess)
		CloseHandle(hProcess);

	return bRet;
};

BOOL SystemternlHelper::GetProcessCmdLine(const DWORD dwPID,CString &strCmdLine)
{
	typedef ULONG PPS_POST_PROCESS_INIT_ROUTINE;
	// Used in PEB struct
	typedef struct _smPEB_LDR_DATA {
		BYTE Reserved1[8];
		PVOID Reserved2[3];
		LIST_ENTRY InMemoryOrderModuleList;
	} PEB_LDR_DATA, *PPEB_LDR_DATA;

#define MAX_UNICODE_PATH	32767L
	// Used in PEB struct
	typedef struct _smRTL_USER_PROCESS_PARAMETERS {
		BYTE Reserved1[16];
		PVOID Reserved2[10];
		UNICODE_STRING ImagePathName;
		UNICODE_STRING CommandLine;
	} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

	typedef struct _smPEB {
		BYTE Reserved1[2];
		BYTE BeingDebugged;
		BYTE Reserved2[1];
		PVOID Reserved3[2];
		PPEB_LDR_DATA Ldr;
		PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
		BYTE Reserved4[104];
		PVOID Reserved5[52];
		PPS_POST_PROCESS_INIT_ROUTINE PostProcessInitRoutine;
		BYTE Reserved6[128];
		PVOID Reserved7[1];
		ULONG SessionId;
	} smPEB, *smPPEB;

	typedef struct _smPROCESS_BASIC_INFORMATION {
		LONG ExitStatus;
		PPEB PebBaseAddress;
		ULONG_PTR AffinityMask;
		LONG BasePriority;
		ULONG_PTR UniqueProcessId;
		ULONG_PTR InheritedFromUniqueProcessId;
	} smPROCESS_BASIC_INFORMATION, *smPPROCESS_BASIC_INFORMATION;

	RTL_USER_PROCESS_PARAMETERS peb_upp	= {0};
	smPEB peb = {0};

	HANDLE hProcess = NULL;
	smPPROCESS_BASIC_INFORMATION pbi = NULL;
	BOOL bRet = FALSE;
	WCHAR *pwszBuffer = NULL;
	TCHAR szCmdLine[MAX_UNICODE_PATH] = {0};

	HMODULE hNtDll = LoadLibrary(L"ntdll.dll");
	if(!hNtDll)
		return FALSE;

	pfnNtQueryInformationProcess func = (pfnNtQueryInformationProcess)GetProcAddress(hNtDll, "NtQueryInformationProcess");
	if(func == NULL) 
		return FALSE;

	do 
	{
		try
		{
			hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, dwPID);
			if(hProcess == INVALID_HANDLE_VALUE) 
				break;

			DWORD dwSize = sizeof(smPROCESS_BASIC_INFORMATION);

			pbi = (smPPROCESS_BASIC_INFORMATION)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,dwSize);
			// Did we successfully allocate memory
			if(!pbi)
				break;

			DWORD dwSizeNeeded = 0;
			NTSTATUS dwStatus = func(hProcess,ProcessBasicInformation,
				pbi,dwSize,&dwSizeNeeded);

			if(dwStatus >= 0 && dwSize < dwSizeNeeded)
			{
				if(pbi)
					HeapFree(GetProcessHeap(), 0, pbi);

				pbi = (smPPROCESS_BASIC_INFORMATION)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,dwSizeNeeded);
				if(!pbi) 
					break;

				dwStatus = func(hProcess,ProcessBasicInformation,
					pbi, dwSizeNeeded, &dwSizeNeeded);
			}

			if(NULL == pbi->PebBaseAddress)
				break;

			DWORD dwBytesRead = 0;
			if(!ReadProcessMemory(hProcess, pbi->PebBaseAddress, &peb, sizeof(peb), (SIZE_T*)&dwBytesRead))
				break;

			dwBytesRead = 0;
			if(!ReadProcessMemory(hProcess,peb.ProcessParameters,&peb_upp,
				sizeof(RTL_USER_PROCESS_PARAMETERS),(SIZE_T*)&dwBytesRead))
				break;

			if(peb_upp.CommandLine.Length <= 0) 
				break;

			pwszBuffer = (WCHAR *)HeapAlloc(GetProcessHeap(),HEAP_ZERO_MEMORY,peb_upp.CommandLine.Length);
			if(NULL == pwszBuffer)
				break;

			if(!ReadProcessMemory(hProcess,
				peb_upp.CommandLine.Buffer,
				pwszBuffer,
				peb_upp.CommandLine.Length,
				(SIZE_T*)&dwBytesRead))
				break;

			// if commandline is larger than our variable, truncate
			DWORD dwBufferSize = 0;

			if(peb_upp.CommandLine.Length >= sizeof(szCmdLine)) 
				dwBufferSize = sizeof(szCmdLine) - sizeof(TCHAR);
			else
				dwBufferSize = peb_upp.CommandLine.Length;

			// Copy CommandLine to our structure variable

#if defined(__AFXWIN_H__)	//如果包含了MFC
	#if defined(_UNICODE)
			StringCbCopyN(szCmdLine, sizeof(szCmdLine),
				pwszBuffer, dwBufferSize);
	#else
			WideCharToMultiByte(CP_ACP, 0, pwszBuffer,
				(int)(dwBufferSize / sizeof(WCHAR)),
				szCmdLine, sizeof(szCmdLine),
				NULL, NULL);
	#endif
#else	//如果没包含MFC
	#if defined(UNICODE)
				StringCbCopyN(szCmdLine, sizeof(szCmdLine),
					pwszBuffer, dwBufferSize);
	#else
				WideCharToMultiByte(CP_ACP, 0, pwszBuffer,
					(int)(dwBufferSize / sizeof(WCHAR)),
					szCmdLine, sizeof(szCmdLine),
					NULL, NULL);
	#endif
#endif

			bRet = TRUE;
			strCmdLine = szCmdLine;

		}
		catch (...)
		{
		}

	} while (FALSE);

	if(hProcess)
		CloseHandle(hProcess);
	hProcess = NULL;

	if(pbi)
		HeapFree(GetProcessHeap(), 0, pbi);
	pbi = NULL;

	if(pwszBuffer)
		HeapFree(GetProcessHeap(), 0, pwszBuffer);
	pwszBuffer = NULL;

	return bRet;
}

BOOL SystemternlHelper::IsSamePath(LPCTSTR pcszSrc, LPCTSTR pcszDest)
{
	if (pcszSrc == NULL || NULL == pcszDest || _tcslen(pcszDest) <= 2 || _tcslen(pcszSrc) <= 2)
		return FALSE;

	TCHAR szDrive[3] = _T(" :");
	TCHAR szDeviceName[MAX_PATH] = { 0 };
	TCHAR szDeviceDest[1024] = { 0 };
	szDrive[0] = pcszDest[0];

	if (0 == ::QueryDosDevice(szDrive, szDeviceName, MAX_PATH))
		return FALSE;

	_tcscpy_s(szDeviceDest,MAX_PATH -1, szDeviceName);
	_tcscat_s(szDeviceDest,MAX_PATH -1, pcszDest + 2);

	if (_tcsicmp(pcszSrc, szDeviceDest) != 0)
		return FALSE;

	return TRUE;
}

HANDLE SystemternlHelper::Findx64Process(LPCTSTR lpcszName, LPCTSTR lpcszFullName, BOOL bCheckFullName, BOOL bNeedTerminate,BOOL bCheckParent,DWORD dwParentID)
{
	EnablePrivilege(SE_DEBUG_NAME, TRUE);
	PFNNtQuerySystemInformation NtQuerySystemInformation = NULL;
	pfnNtQueryInformationProcess NtQueryInformationProcess = NULL;
	HMODULE hDllMod = GetModuleHandle(L"ntdll.dll") ;
	if ( hDllMod == NULL )
	{
		hDllMod = LoadLibrary( L"ntdll.dll" ) ;
		if ( hDllMod == NULL ) 
			return NULL;
	}

	NtQuerySystemInformation = (PFNNtQuerySystemInformation)GetProcAddress (hDllMod,"NtQuerySystemInformation") ;
	NtQueryInformationProcess = (pfnNtQueryInformationProcess)GetProcAddress(hDllMod,"NtQueryInformationProcess");
	if(!NtQuerySystemInformation || !NtQueryInformationProcess)
		return FALSE;

	LPBYTE lpBuf = NULL ;
	UINT nSize = 16 * 1024;
	while ( TRUE )
	{
		if((lpBuf = new BYTE [nSize]) == NULL)
			return NULL;

		//枚举进程信息
		SYSTEM_INFORMATION_CLASS Information = SystemProcessInformation;
		//NTSTATUS status = NtQuerySystemInformation ((SYSTEM_INFORMATION_CLASS)SystemProcessesAndThreadsInformation,lpBuf,nSize,0) ;
		NTSTATUS status = NtQuerySystemInformation (Information,lpBuf,nSize,0) ;
		if (!NT_SUCCESS(status))
		{
			if (status == STATUS_INFO_LENGTH_MISMATCH) // 检测是否返回缓冲区不够大
			{
				nSize += 4 * 1024 ;
				delete []lpBuf ;
				continue ;
			}
			else
			{
				delete []lpBuf;
				return NULL;
			}
		}
		else
			break ;
	}

	HANDLE hProcessTaget = NULL,hProcess = NULL;
	DWORD dwDesiredAccess = PROCESS_QUERY_INFORMATION | PROCESS_VM_READ;
	if (bNeedTerminate)
		dwDesiredAccess |= PROCESS_TERMINATE | SYNCHRONIZE;
	PSYSTEM_PROCESSES pSysProcess = (PSYSTEM_PROCESSES)lpBuf ;

	while(pSysProcess->NextEntryDelta != 0)
	{
		if (pSysProcess->ProcessName.Buffer != NULL && _tcsicmp(pSysProcess->ProcessName.Buffer,lpcszName) == 0)
		{
			if(bCheckParent && pSysProcess->InheritedFromProcessId != dwParentID)
			{
				pSysProcess = (PSYSTEM_PROCESSES)( (DWORD)pSysProcess + pSysProcess->NextEntryDelta ) ;
				continue;
			}

			HANDLE hProcess = NULL;
			if( hProcess = ::OpenProcess(dwDesiredAccess, FALSE, pSysProcess->ProcessId))
			{
				if(!bCheckFullName)
				{
					hProcessTaget = hProcess;
					break;
				}
				else
				{
					TCHAR szFullpath[1024] = {0};
					UNICODE_STRING*  pIMGPath = NULL;
					NtQueryInformationProcess(hProcess, (PROCESSINFOCLASS)ProcessImageFileName,(PVOID)szFullpath,sizeof(szFullpath),NULL);
					pIMGPath = (PUNICODE_STRING)szFullpath;
					if(IsSamePath(pIMGPath->Buffer, lpcszFullName))
					{
						hProcessTaget = hProcess;
						break;
					}
				}

				::CloseHandle(hProcess);
			}
		}
		pSysProcess = (PSYSTEM_PROCESSES)( (DWORD)pSysProcess + pSysProcess->NextEntryDelta ) ;
	}

	delete []lpBuf ;
	return hProcessTaget;
}

HANDLE SystemternlHelper::FindProcess(LPCTSTR lpcszName, LPCTSTR lpcszFullName, BOOL bCheckFullName, BOOL bNeedTerminate,BOOL bCheckParent,DWORD dwParentID)
{
	EnablePrivilege(SE_DEBUG_NAME, TRUE);

	HANDLE hProcessTaget = NULL;
	HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);   
	if(hSnapshot == (HANDLE)-1) 
		return hProcessTaget;

	PROCESSENTRY32 pe = { 0 };
	pe.dwSize = sizeof(pe);
	DWORD dwDesiredAccess = PROCESS_QUERY_INFORMATION | PROCESS_VM_READ;
	if (bNeedTerminate)
		dwDesiredAccess |= PROCESS_TERMINATE | SYNCHRONIZE;

	BOOL bMore = ::Process32First(hSnapshot,&pe);
	while(bMore)
	{   
		if(_tcsicmp(lpcszName, pe.szExeFile) == 0)
		{
			if(bCheckParent && pe.th32ParentProcessID != dwParentID)
			{
				bMore = ::Process32Next(hSnapshot,&pe);   
				continue;
			}

			TCHAR szFullpath[1024] = {0};
			HANDLE hProcess = NULL;
			if (hProcess = ::OpenProcess(dwDesiredAccess, FALSE, pe.th32ProcessID))
			{
				if (bCheckFullName)
				{
					TCHAR szLongPath[MAX_PATH] = {};

					::GetModuleFileNameEx(hProcess, NULL, szFullpath, _countof(szFullpath) - 1);
					if (0 == wcslen(szFullpath))
					{
						GetProcessFullPath(pe.th32ProcessID,szFullpath);
					}

					GetLongPathName(szFullpath, szLongPath, MAX_PATH);
					if(_tcsicmp(lpcszFullName, szFullpath) == 0)
					{
						hProcessTaget = hProcess;
						break;
					}
					else if (_tcsicmp(lpcszFullName, szLongPath) == 0)
					{
						hProcessTaget = hProcess;
						break;
					}
					
					::CloseHandle(hProcess);
				}
				else
				{
					hProcessTaget = hProcess;
					break;
				}
			}
			else
			{
				/*
				CString strErr;
				strErr.Format(_T("OpenProcess Failed!,err=%u"), ::GetLastError());
				::MessageBox(NULL, strErr, _T("Error"), MB_OK);
				*/
			}
		}
		bMore = ::Process32Next(hSnapshot,&pe);   
	}

	::CloseHandle(hSnapshot);
	return hProcessTaget;
}

int SystemternlHelper::FindAllx64Processes(LPCTSTR lpcszName, HANDLE* pHProcesses, int nMax)
{
	int nResult = 0;

	if (NULL == lpcszName || _tcslen(lpcszName) <= 0 || NULL == pHProcesses || 0 >= nMax)
		return nResult;

	EnablePrivilege(SE_DEBUG_NAME, TRUE);
	PFNNtQuerySystemInformation NtQuerySystemInformation = NULL;
	pfnNtQueryInformationProcess NtQueryInformationProcess = NULL;
	HMODULE hDllMod = GetModuleHandle(L"ntdll.dll") ;
	if (NULL == hDllMod)
	{
		hDllMod = LoadLibrary( L"ntdll.dll" ) ;
		if (NULL == hDllMod) 
			return nResult;
	}

	NtQuerySystemInformation = (PFNNtQuerySystemInformation)GetProcAddress (hDllMod,"NtQuerySystemInformation") ;
	NtQueryInformationProcess = (pfnNtQueryInformationProcess)GetProcAddress(hDllMod,"NtQueryInformationProcess");
	if(!NtQuerySystemInformation || !NtQueryInformationProcess)
		return nResult;

	LPBYTE lpBuf = NULL ;
	UINT nSize = 16 * 1024;
	while ( TRUE )
	{
		if((lpBuf = new BYTE [nSize]) == NULL)
			return nResult;

		//枚举进程信息
		SYSTEM_INFORMATION_CLASS Information = SystemProcessInformation;
		//NTSTATUS status = NtQuerySystemInformation ((SYSTEM_INFORMATION_CLASS)SystemProcessesAndThreadsInformation,lpBuf,nSize,0) ;
		NTSTATUS status = NtQuerySystemInformation (Information,lpBuf,nSize,0) ;
		if (!NT_SUCCESS(status))
		{
			if (status == STATUS_INFO_LENGTH_MISMATCH) // 检测是否返回缓冲区不够大
			{
				nSize += 4 * 1024 ;
				delete []lpBuf ;
				continue ;
			}
			else
			{
				delete []lpBuf;
				return nResult;
			}
		}
		else
			break ;
	}

	int nIndex = 0;
	HANDLE hProcess = NULL;
	DWORD dwDesiredAccess = PROCESS_QUERY_INFORMATION | PROCESS_VM_READ;
	PSYSTEM_PROCESSES pSysProcess = (PSYSTEM_PROCESSES)lpBuf ;

	while(pSysProcess->NextEntryDelta != 0)
	{
		if (pSysProcess->ProcessName.Buffer != NULL && _tcsicmp(pSysProcess->ProcessName.Buffer, lpcszName) == 0)
		{
			hProcess = ::OpenProcess(dwDesiredAccess, FALSE, pSysProcess->ProcessId);
			if(NULL != hProcess)
			{
				pHProcesses[nIndex] = hProcess;
				nIndex++;

				if (nIndex >= nMax)
					break;
			}
			else
			{
				CString strMsg;
				strMsg.Format(_T("Open Process %u failed, lasterr=%u\n"), pSysProcess->ProcessId, GetLastError());
				OutputDebugString(strMsg);
			}
		}
		pSysProcess = (PSYSTEM_PROCESSES)( (DWORD)pSysProcess + pSysProcess->NextEntryDelta) ;
	}

	delete []lpBuf ;

	nResult = nIndex;

	return nResult;
}

int SystemternlHelper::FindAllProcesses(LPCTSTR lpcszName, HANDLE* pHProcesses, int nMax)
{
	int nResult = 0;
	if (NULL == lpcszName || _tcslen(lpcszName) <= 0 || NULL == pHProcesses || 0 >= nMax)
		return nResult;

	EnablePrivilege(SE_DEBUG_NAME, TRUE);

	if(COSChecker::IsWow64())
		return FindAllx64Processes(lpcszName, pHProcesses, nMax);

	HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);   
	if(hSnapshot == (HANDLE)-1) 
		return nResult;

	PROCESSENTRY32 pe = { 0 };
	pe.dwSize = sizeof(pe);
	DWORD dwDesiredAccess = PROCESS_QUERY_INFORMATION | PROCESS_VM_READ;

	int nIndex = 0;
	BOOL bMore = ::Process32First(hSnapshot,&pe);
	while(bMore)
	{   
		if(_tcsicmp(lpcszName, pe.szExeFile) == 0)
		{
			HANDLE hProcess = ::OpenProcess(dwDesiredAccess, FALSE, pe.th32ProcessID);
			if (NULL != hProcess)
			{
				pHProcesses[nIndex] = hProcess;
				nIndex++;

				if (nIndex >= nMax)
					break;
			}
			else
			{
				CString strMsg;
				strMsg.Format(_T("Open Process %u failed, lasterr=%u\n"), pe.th32ProcessID, GetLastError());
				OutputDebugString(strMsg);
			}
		}
		bMore = ::Process32Next(hSnapshot, &pe);   
	}

	::CloseHandle(hSnapshot);
	hSnapshot = NULL;

	nResult = nIndex;

	return nResult;
}


int SystemternlHelper::GetCPUsage()
{
	FILETIME timeIdle1,timeKernel1,timeUser1;
	GetSystemTimes(&timeIdle1,&timeKernel1,&timeUser1);
	__int64 idle1	= timeIdle1.dwLowDateTime	+ ((__int64)timeIdle1.dwHighDateTime << 32);
	__int64 kernel1 = timeKernel1.dwLowDateTime + ((__int64)timeKernel1.dwHighDateTime << 32);
	__int64 user1	= timeUser1.dwLowDateTime	+ ((__int64)timeUser1.dwHighDateTime << 32);

	Sleep(1000);

	FILETIME timeIdle2,timeKernel2,timeUser2;
	GetSystemTimes(&timeIdle2,&timeKernel2,&timeUser2);

	__int64 idle2	= timeIdle2.dwLowDateTime	+ ((__int64)timeIdle2.dwHighDateTime << 32);
	__int64 kernel2 = timeKernel2.dwLowDateTime + ((__int64)timeKernel2.dwHighDateTime << 32);
	__int64 user2	= timeUser2.dwLowDateTime	+ ((__int64)timeUser2.dwHighDateTime << 32);

	__int64 usr = user2 - user1;
	__int64 ker = kernel2 - kernel1;
	__int64 idl = idle2 - idle1;

	__int64 sys = ker + usr;

	return int( (sys - idl) *100 / sys );
}

BOOL SystemternlHelper::IsSystemAccount()
{
	char szUsername[256] = { 0 };
	DWORD dwUsername = 256;
	if(!GetUserNameA(szUsername, &dwUsername))
		return FALSE;

	return (stricmp(szUsername, "SYSTEM") == 0);
}


void SystemternlHelper::ClearTrayIcon()
{
	HWND hWnd =	FindWindowEx(FindWindowEx(FindWindowEx(FindWindow(L"Shell_TrayWnd", NULL), 0, L"TrayNotifyWnd", NULL), 0, L"SysPager", NULL), 0, L"ToolbarWindow32", NULL);
	if (hWnd == NULL)  
		return;  

	RECT rTrayToolBar;
	GetClientRect(hWnd, &rTrayToolBar); 
	for(int x=0; x < rTrayToolBar.right; x+=5) 
		for(int y=1; y < rTrayToolBar.bottom; y+=5) 
			SendMessage(hWnd , WM_MOUSEMOVE, 0, MAKELPARAM(x,y)); 


	/*
	DWORD dwPId = 0;  
	GetWindowThreadProcessId(hWnd, &dwPId);  
	if (dwPId == NULL)  
		return;  
	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_ALL_ACCESS, true, dwPId);  
	if (hProcess == NULL)  
		return;  

	int nCnt = ::SendMessage(hWnd, TB_BUTTONCOUNT, NULL, NULL);

	LPVOID lpData = VirtualAllocEx(hProcess, NULL, sizeof(TBBUTTON), MEM_COMMIT, PAGE_READWRITE);
	if(lpData == NULL || nCnt < 1)
		goto Exit0;

	for (int i = 0; i < nCnt; i++)  
	{  
		DWORD dwBytesRead = 0;
		TBBUTTON buttonData;
		SendMessage(hWnd, TB_GETBUTTON, i, (LPARAM)lpData);
		ReadProcessMemory(hProcess, lpData, &buttonData, sizeof(TBBUTTON), &dwBytesRead);
		if (dwBytesRead < sizeof(TBBUTTON))
			continue;

		DWORD dwExtraData[2] = {0, 0};
		ReadProcessMemory(hProcess, (LPVOID)buttonData.dwData, dwExtraData, sizeof(dwExtraData), &dwBytesRead);
		if (dwBytesRead < sizeof(dwExtraData))
			continue;

		DWORD dwTrayPId = 0;
		GetWindowThreadProcessId((HWND)dwExtraData[0], &dwTrayPId);   
		if (dwTrayPId == NULL)
			::SendMessage(hWnd, TB_DELETEBUTTON, i, 0);
	} 

Exit0:
	if (lpData && hProcess)
	{
		VirtualFreeEx(hProcess, lpData, NULL, MEM_RELEASE);
		lpData = NULL;
	}

	if (hProcess)
	{
		CloseHandle(hProcess);
		hProcess = NULL;
	}*/
}

PSECURITY_ATTRIBUTES SystemternlHelper::CreateLowSecurityAttribute(void)
{
	BOOL bResult = FALSE;
	BOOL bRetCode = FALSE;
	PSECURITY_ATTRIBUTES psa = NULL;
	PSECURITY_DESCRIPTOR psd = NULL;

	do 
	{		
		psd = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR,sizeof(SECURITY_DESCRIPTOR));
		if(NULL == psd)
			break;

		bRetCode = ::InitializeSecurityDescriptor(psd,SECURITY_DESCRIPTOR_REVISION);
		if (!bRetCode)
			break;

		bRetCode = ::SetSecurityDescriptorDacl(psd,TRUE,NULL,FALSE);
		if (!bRetCode)
			break;

		psa = (PSECURITY_ATTRIBUTES)LocalAlloc(LPTR,sizeof(SECURITY_ATTRIBUTES));
		if (NULL == psa)
			break;

		psa->nLength = sizeof(SECURITY_ATTRIBUTES);
		psa->bInheritHandle = TRUE;
		psa->lpSecurityDescriptor = psd;
		bResult = TRUE;

	}while(FALSE);

	if(!bResult)
	{
		if(psd) 
		{ 
			LocalFree(psd);
			psd = NULL;
		}

		if(psa) 
		{ 
			LocalFree(psa);
			psa = NULL;
		}
	}

	return psa;
}

BOOL SystemternlHelper::MYFreeSecurityAttribute(PSECURITY_ATTRIBUTES psa)
{
	if(psa)
	{
		LocalFree(psa->lpSecurityDescriptor);
		LocalFree(psa);
	}

	return TRUE;
}

BOOL SystemternlHelper::GetTokenByName(HANDLE &hToken,LPTSTR lpName)
{
	hToken = NULL;
	if(!lpName)
		return FALSE;

	HANDLE hProcess = FindProcess(lpName,NULL);
	if(!hProcess)
		return FALSE;

	BOOL bRet = OpenProcessToken(hProcess,TOKEN_ALL_ACCESS,&hToken);
	CloseHandle(hProcess);
	return bRet;
}

BOOL SystemternlHelper::GetTokenByPid(HANDLE& hToken, DWORD dwProcessID)
{
	hToken = NULL;
	if (0 == dwProcessID)
		return FALSE;
	//进程句柄
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);

	//线程句柄
	//HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, dwThreadID);

	if (!hProcess)
		return FALSE;

	BOOL bRet = OpenProcessToken(hProcess, TOKEN_ALL_ACCESS, &hToken);
	CloseHandle(hProcess);
	return bRet;
}

BOOL SystemternlHelper::GetTokenByPid_thread(HANDLE& hToken, DWORD dwThreadID)
{
	hToken = NULL;
	if (0 == dwThreadID)
		return FALSE;
	//进程句柄
	//HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);

	//线程句柄
	HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, dwThreadID);

	if (!hThread)
		return FALSE;

	BOOL bRet = OpenProcessToken(hThread, TOKEN_ALL_ACCESS, &hToken);
	CloseHandle(hThread);
	return bRet;
}

int SystemternlHelper::GetAllTokensByName(LPCTSTR lpcszName, HANDLE* pTokens, int nMax)
{
	int nResult = 0;
	if (NULL == lpcszName || _tcslen(lpcszName) <= 0 || NULL == pTokens || 0 >= nMax)
		return nResult;

	HANDLE *phProcesses = new HANDLE[nMax + 2];
	if (NULL == phProcesses)
		return nResult;

	do 
	{
		memset(phProcesses, 0, (nMax + 2) * sizeof(HANDLE));

		int nRet = FindAllProcesses(lpcszName, phProcesses, nMax);
		if (nRet <= 0 || nRet > nMax)
			break;

		int nGetToken = 0;
		for (int nIndex = 0; nIndex < nRet; nIndex++)
		{
			if (NULL == phProcesses[nIndex])
				continue;

			HANDLE hToken = NULL;
			BOOL bRet = OpenProcessToken(phProcesses[nIndex], TOKEN_ALL_ACCESS, &hToken);
			CloseHandle(phProcesses[nIndex]);
			phProcesses[nIndex] = NULL;

			if (NULL != hToken && bRet)
			{
				pTokens[nGetToken] = hToken;
				nGetToken++;
			}
		}

		nResult = nGetToken;

	} while (FALSE);

	if (NULL != phProcesses)
	{
		delete []phProcesses;
		phProcesses = NULL;
	}

	return nResult;	
}

BOOL SystemternlHelper::LaunchProcess(LPWSTR lpCmdLine,LPWSTR lpWorkDir,int& nErrCode)
{
	if(!lpCmdLine || _tcslen(lpCmdLine) <= 0)
		return FALSE;

	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));

	if( !CreateProcess( NULL,   // No module name (use command line)
		lpCmdLine,        // Command line
		NULL,           // Process handle not inheritable
		NULL,           // Thread handle not inheritable
		FALSE,          // Set handle inheritance to FALSE
		0,              // No creation flags
		NULL,           // Use parent's environment block
		lpWorkDir,           // Use parent's starting directory 
		&si,            // Pointer to STARTUPINFO structure
		&pi)           // Pointer to PROCESS_INFORMATION structure
		) 
	{
		nErrCode = GetLastError();
		return FALSE;
	}

	if (NULL != pi.hThread && INVALID_HANDLE_VALUE != pi.hThread)
	{
		CloseHandle(pi.hThread);
	}

	if (NULL != pi.hProcess && INVALID_HANDLE_VALUE != pi.hProcess)
	{
		CloseHandle(pi.hProcess);
	}

	return TRUE;
}

BOOL SystemternlHelper::LaunchProcessAsUser(LPWSTR lpCmdLine, int& nErrCode,HANDLE* pHandle)
{
	if(!lpCmdLine || _tcslen(lpCmdLine) <= 0)
		return FALSE;

	OSVERSIONINFO info = { 0 };
	info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if(!GetVersionEx(&info))
		return FALSE;

	HANDLE hToken = NULL;
	CString csExplorer = L"explorer.exe";
	if(!GetTokenByName(hToken, csExplorer.GetBuffer()))
	{
		nErrCode = GetLastError();
		WRITE_LOG(_T("获取用户进程explorer.exe token 失败！GetLastError=%d \n "),nErrCode);
		return FALSE;
	}
	else if (info.dwMajorVersion >= 6)
	{
		//防止VISTA之后的系统UAC时explorer.exe token是一个受限TOKEN,后续引起出错,检查有无TokenLinkedToken,有则就使用之			
		TOKEN_LINKED_TOKEN RAdminToken = { 0 };
		DWORD dwSize = sizeof(TOKEN_LINKED_TOKEN);
		if (GetTokenInformation(hToken, TokenLinkedToken, &RAdminToken, dwSize, &dwSize) && NULL != RAdminToken.LinkedToken)
		{
			CloseHandle(hToken);
			hToken = RAdminToken.LinkedToken;
		}
	}

	LPVOID lpEnv = NULL;
	if(!CreateEnvironmentBlock(&lpEnv, hToken, FALSE))
	{
		nErrCode = GetLastError();
		WRITE_LOG(_T("CreateEnvironmentBlock 失败！GetLastError=%d \n "),nErrCode);
		return FALSE;
	}

	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb= sizeof(STARTUPINFO);
	CString textcsbuffer = TEXT("winsta0\\default");
	si.lpDesktop = textcsbuffer.GetBuffer();
	BOOL bResult = CreateProcessAsUser(hToken, NULL, lpCmdLine, NULL, NULL,
		FALSE,NORMAL_PRIORITY_CLASS | CREATE_UNICODE_ENVIRONMENT, lpEnv, NULL, &si, &pi);
	if (!bResult)
	{
		nErrCode = GetLastError();
		WRITE_LOG(_T("CreateProcessAsUser 失败！GetLastError=%d \n "),nErrCode);
	}

	if(lpEnv)
		DestroyEnvironmentBlock(lpEnv);
	
	if(hToken)
		CloseHandle(hToken);
	
	if(pi.hThread)
		CloseHandle(pi.hThread);
	
	if(pHandle)
		*pHandle = pi.hProcess;
	else if(pi.hProcess)
		CloseHandle(pi.hProcess);

	return bResult;
}

BOOL SystemternlHelper::LaunchSystemProcessWithUserEnv(LPWSTR lpCmdLine)
{
	LPVOID lpEnv = NULL;
	HANDLE hSysToken = NULL;
	HANDLE hUserToken = NULL;
	HANDLE hProcessToken = NULL;

	BOOL bResult = FALSE;
	DWORD dwSessionID = 0;
	DWORD dwSize = sizeof(DWORD);
	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	si.cb = sizeof(STARTUPINFO);
	CString textcsbuffer = TEXT("winsta0\\default");
	si.lpDesktop = textcsbuffer.GetBuffer();

	CString strMsg;
	do
	{
		//获取system进程Token
		if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ALL_ACCESS, &hSysToken))
		{
			strMsg.Format(_T("获取system进程Token 失败！GetLastError=%d \n "), GetLastError());
			WRITE_LOG(strMsg);
			break;
		}

		//从system toke复制出进程token
		if (!DuplicateTokenEx(hSysToken, MAXIMUM_ALLOWED, NULL, SecurityIdentification, TokenPrimary, &hProcessToken))
		{
			strMsg.Format(_T("从system toke复制出进程token 失败！GetLastError=%d \n "), GetLastError());
			WRITE_LOG(strMsg);
			break;
		}

		//获取用户进程token
		CString csExplorer = _T("explorer.exe");
		if(!GetTokenByName(hUserToken, csExplorer.GetBuffer()))
		{
			strMsg.Format(_T("获取用户进程explorer.exe token 失败！GetLastError=%d \n "), GetLastError());
			WRITE_LOG(strMsg);
			break;
		}		

		//获取用户session id
		if (!GetTokenInformation(hUserToken, TokenSessionId, &dwSessionID, dwSize, &dwSize))
		{
			strMsg.Format(_T("获取用户session id 失败！GetLastError=%d \n "), GetLastError());
			break;
		}

		//修改进程session id
		if (!SetTokenInformation(hProcessToken, TokenSessionId, &dwSessionID, sizeof(DWORD)))
		{
			strMsg.Format(_T("修改进程session id 失败！GetLastError=%d \n "), GetLastError());
			WRITE_LOG(strMsg);
			break;
		}

		//获取用户进程环境
		if(!CreateEnvironmentBlock(&lpEnv, hUserToken, FALSE))
		{
			strMsg.Format(_T("获取用户进程环境 失败！GetLastError=%d \n "), GetLastError());
			WRITE_LOG(strMsg);
			break;
		}

		bResult = CreateProcessAsUser(hProcessToken, NULL, lpCmdLine, NULL, NULL,
			FALSE, NORMAL_PRIORITY_CLASS | CREATE_UNICODE_ENVIRONMENT, lpEnv, NULL, &si, &pi);
		if (!bResult)
		{
			strMsg.Format(_T("CreateProcessAsUser 失败！GetLastError=%d \n "), GetLastError());
			WRITE_LOG(strMsg);
		}

	}while (FALSE);


	if(lpEnv)
		DestroyEnvironmentBlock(lpEnv);

	if(hSysToken)
		CloseHandle(hSysToken);

	if(hUserToken)
		CloseHandle(hUserToken);

	if(hProcessToken)
		CloseHandle(hProcessToken);

	if(pi.hThread)
		CloseHandle(pi.hThread);

	if(pi.hProcess)
		CloseHandle(pi.hProcess);

	return bResult;
}

BOOL SystemternlHelper::LUAIsElevatedToken(HANDLE hToken, PBOOL pbElevated)
{
	DWORD dwReturnBytes = 0;
	DWORD dwElevateionType = 0;
	BOOL bElevated = FALSE;

	if(hToken && pbElevated)
	{
		if( GetTokenInformation(hToken, TokenElevationType, &dwElevateionType, sizeof(dwElevateionType), &dwReturnBytes) )
		{
			if(dwElevateionType == TokenElevationTypeFull)
				bElevated = TRUE;
			else if(dwElevateionType == TokenElevationTypeDefault)
			{
				TOKEN_ELEVATION te;
				ZeroMemory(&te, sizeof(te));

				if( GetTokenInformation(hToken, TokenElevation, &te, sizeof(te), &dwReturnBytes))
				{
					if(te.TokenIsElevated)
						bElevated = TRUE;
				}
			}
		}

		if(pbElevated)
			*pbElevated = bElevated;

		return TRUE;
	}

	return FALSE;
}

BOOL SystemternlHelper::LUAGetElevatedToken(HANDLE hToken, PHANDLE phNewToken, PBOOL pbElevated)
{
	if (NULL == phNewToken)
		return FALSE;

	BOOL bElevated = FALSE;
	LUAIsElevatedToken(hToken, &bElevated);

	if (NULL != pbElevated) *pbElevated = bElevated;

	if(bElevated)
	{
		return DuplicateTokenEx(hToken, TOKEN_ALL_ACCESS, NULL, SecurityImpersonation, TokenPrimary, phNewToken);
	}
	else
	{
		DWORD dwReturnBytes = 0;
		return GetTokenInformation(hToken, TokenLinkedToken, phNewToken, sizeof(HANDLE), &dwReturnBytes);
	}

	return FALSE;
}

typedef BOOL (STDMETHODCALLTYPE FAR * LPFNCREATEENVIRONMENTBLOCK) ( LPVOID  *lpEnvironment, HANDLE  hToken, BOOL    bInherit );
typedef BOOL (STDMETHODCALLTYPE FAR * LPFNDESTROYENVIRONMENTBLOCK) ( LPVOID lpEnvironment );

BOOL SystemternlHelper::CreateProcessByToken(HANDLE hToken, HANDLE hEnvToken, LPCTSTR pszCmd, LPCTSTR pszCwd)
{
	if (NULL == pszCmd)
		return FALSE;

	int nCmdLen = wcslen(pszCmd);
	if (nCmdLen < 4)
		return FALSE;

	STARTUPINFO si = {sizeof(si)};
	PROCESS_INFORMATION pi = {0};

	CString textcsbuffer = _T("Winsta0\\Default");
	si.lpDesktop = textcsbuffer.GetBuffer();
	DWORD  dwCreationFlag = NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE;
	LPVOID pEnvironment = NULL;
	LPFNCREATEENVIRONMENTBLOCK  lpfnCreateEnvironmentBlock  = NULL;
	LPFNDESTROYENVIRONMENTBLOCK lpfnDestroyEnvironmentBlock = NULL;
	HMODULE hUserEnvLib = NULL;
	hUserEnvLib = LoadLibrary( _T("userenv.dll") );
	if ( NULL != hUserEnvLib )
	{ 
		lpfnCreateEnvironmentBlock  = (LPFNCREATEENVIRONMENTBLOCK)GetProcAddress( hUserEnvLib, "CreateEnvironmentBlock" );
		lpfnDestroyEnvironmentBlock = (LPFNDESTROYENVIRONMENTBLOCK)GetProcAddress( hUserEnvLib, "DestroyEnvironmentBlock" );
	}

	if ( NULL != lpfnCreateEnvironmentBlock )
	{ 
		if ( lpfnCreateEnvironmentBlock( &pEnvironment, hEnvToken, FALSE ) )
		{ 
			dwCreationFlag |= CREATE_UNICODE_ENVIRONMENT; // must specify				
		} 
		else 
			pEnvironment = NULL;				
	}

	CString strCmd = pszCmd;
	if (pszCmd[0] == L'\"' && pszCmd[nCmdLen-1] != L'\"')
	{
		strCmd += L"\"";
	}
	else if(pszCmd[0] != L'\"' && pszCmd[nCmdLen-1] == L'\"')
	{
		strCmd = CString(L"\"") + strCmd;
	}
	else if(pszCmd[0] != L'\"' && pszCmd[nCmdLen-1] != L'\"')
	{
		strCmd = CString(L"\"") + strCmd + CString(L"\"");
	}

	BOOL bRet = CreateProcessAsUser( 
		hToken, NULL, (LPTSTR)strCmd.GetBuffer(), NULL, NULL, FALSE, dwCreationFlag, pEnvironment, pszCwd, &si, &pi );

	if( pi.hProcess != NULL )
		CloseHandle( pi.hProcess );
	if( pi.hThread != NULL )
		CloseHandle( pi.hThread );
	if ( NULL != lpfnDestroyEnvironmentBlock )
		lpfnDestroyEnvironmentBlock( pEnvironment );
	if ( NULL != hUserEnvLib )
		FreeLibrary( hUserEnvLib );

	return bRet;
}

BOOL SystemternlHelper::CreateProcessAsSystemBySession(LPCTSTR pszCmd, LPCTSTR pszCwd, DWORD dwSession, HANDLE hEnvToken)
{
	if (pszCmd == NULL)
		return FALSE;


	BOOL bRet = FALSE;

	HANDLE hTokenThis = NULL; 
	bRet = OpenProcessToken(GetCurrentProcess(), TOKEN_DUPLICATE | TOKEN_QUERY, &hTokenThis);
	if (!bRet || hTokenThis == NULL)
		return FALSE;

	HANDLE hTokenDup = NULL;
	bRet = DuplicateTokenEx(hTokenThis, MAXIMUM_ALLOWED, NULL, SecurityIdentification, TokenPrimary, &hTokenDup);
	CloseHandle(hTokenThis);
	if (!bRet || hTokenDup == NULL)
		return FALSE;

	if (!SetTokenInformation(hTokenDup, TokenSessionId, &dwSession, sizeof(DWORD))) 
	{		
		CloseHandle(hTokenDup);
		return FALSE;
	}

	bRet = CreateProcessByToken(hTokenDup, hEnvToken, pszCmd, pszCwd);
	CloseHandle(hTokenDup);

	return bRet;
}

BOOL GetTokenInfo(HANDLE hToken, 
				  TOKEN_INFORMATION_CLASS Tic, 
				  PVOID *pTokenInfoOut, 
				  DWORD* pdwOutSize)
{
	BOOL	bRetCode= TRUE;
	DWORD	dwSize	= 0;
	PVOID	pv		= NULL;

	if(NULL == pTokenInfoOut)
		bRetCode = FALSE;
	else if(!GetTokenInformation(hToken, Tic, 0, 0, &dwSize) 
		&& GetLastError() != ERROR_INSUFFICIENT_BUFFER) 
		bRetCode = FALSE;
	else if(NULL == (pv = malloc(dwSize)))
		bRetCode = FALSE;
	else if(!GetTokenInformation(hToken, Tic, pv, dwSize, &dwSize))
	{		
		free(pv);
		*pTokenInfoOut	= 0;
		bRetCode		= FALSE;
	}
	else
	{
		*pTokenInfoOut = pv;

		if (pdwOutSize)
			*pdwOutSize = dwSize;

		bRetCode = TRUE; 
	}

	return bRetCode;
}

void FreeTokenInfo(PVOID pTokenInfo)
{
	free(pTokenInfo);
}

bool IsSidEqual(WELL_KNOWN_SID_TYPE Type, PSID pSid)
{
	bool bEqual = false;

	DWORD SidSize = SECURITY_MAX_SID_SIZE;
	PSID pSidNew = LocalAlloc(LMEM_FIXED, SidSize);
	if (NULL == pSidNew)
		goto Exit0;

	if (!CreateWellKnownSid(Type, NULL, pSidNew, &SidSize))
		goto Exit0;

	if (!::EqualSid(pSid, pSidNew))
		goto Exit0;

	bEqual = true;

Exit0:
	if (pSidNew)
		LocalFree(pSidNew);

	return bEqual;
}

int GetProcessMandatoryLevel(HANDLE hToken)
{
	int Level = 0;
	PTOKEN_MANDATORY_LABEL pTML = NULL;
	DWORD dwSize = 0;
	if (!GetTokenInfo(hToken, TokenIntegrityLevel, (void**)&pTML, &dwSize))
	{
		Level = WinHighLabelSid;
		goto Exit0;
	}

	typedef struct _MANDATORY_LEVEL
	{
		WELL_KNOWN_SID_TYPE	Type;
		int					Level;
	} MANDATORY_LEVEL;

	static const MANDATORY_LEVEL MandatoryLevel[] = 
	{
		{WinHighLabelSid, WinHighLabelSid},
		{WinSystemLabelSid, WinSystemLabelSid},
		{WinMediumLabelSid, WinMediumLabelSid},
		{WinLowLabelSid, WinLowLabelSid}
	};

	for (int i = 0; i < sizeof(MandatoryLevel) / sizeof(MANDATORY_LEVEL); i++)
	{
		if (!IsSidEqual(MandatoryLevel[i].Type, pTML->Label.Sid))
			continue;

		Level = MandatoryLevel[i].Level;
		break;
	}


Exit0:
	if (pTML)
		FreeTokenInfo(pTML);

	return Level;
}

BOOL SystemternlHelper::IsAdminToken(HANDLE hTokenThis)
{
	bool bResult = false;
	SID_IDENTIFIER_AUTHORITY ntAuth = SECURITY_NT_AUTHORITY;
	PSID pAdminSid = NULL;
	PTOKEN_GROUPS pGroupList = NULL; 
	DWORD dwSize = 0;


	if (!::AllocateAndInitializeSid(&ntAuth, 2, SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &pAdminSid))
	{		
		goto Exit0;
	}


	if (!GetTokenInfo(hTokenThis, TokenGroups, (void**)&pGroupList, &dwSize))
		goto Exit0;


	for (DWORD i = 0; i < pGroupList->GroupCount; i++)
	{
		if (::EqualSid(pAdminSid, pGroupList->Groups[i].Sid))
		{
			bResult = true;
			break;
		}
	}

	if (GetProcessMandatoryLevel(hTokenThis) < WinHighLabelSid)
	{
		bResult = false;
	}

Exit0:

	if (pGroupList)
		FreeTokenInfo(pGroupList);

	if (pAdminSid)
		FreeSid(pAdminSid);

	return bResult;
}

BOOL SystemternlHelper::IsLogonUserAdmin()
{		
	BOOL bFound = 0;
	PSID pAdminSid = NULL;
	HANDLE hProcess = NULL;
	HANDLE hToken = NULL,hLinkedToken = NULL;
	SID_IDENTIFIER_AUTHORITY ntAuth = SECURITY_NT_AUTHORITY;

	do
	{
		hProcess = SystemternlHelper::FindProcess(L"explorer.exe",NULL);
		if(!hProcess)
			break;

		if(!OpenProcessToken(hProcess,TOKEN_QUERY,&hToken))
			break;

		// normally, I should get the size of the group list first, but ...
		byte rawGroupList[4096];
		TOKEN_GROUPS& groupList = *((TOKEN_GROUPS *)rawGroupList);
		DWORD dwLen = sizeof(rawGroupList);

		if(!GetTokenInformation(hToken,TokenGroups,&groupList,dwLen,&dwLen))
			break;

		if (!AllocateAndInitializeSid( &ntAuth, 2, SECURITY_BUILTIN_DOMAIN_RID,
			DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &pAdminSid))
		{
			break;
		}

		for(size_t i = 0; i < groupList.GroupCount; ++ i )
		{
			if(EqualSid(pAdminSid,groupList.Groups[i].Sid))
			{
				bFound = 1;
				break;
			}
		}

		if(!bFound)
		{
			dwLen = sizeof(HANDLE);
			if(!GetTokenInformation(hToken,TokenLinkedToken,(LPVOID)&hLinkedToken,sizeof(HANDLE),&dwLen))
				break;

			dwLen = sizeof rawGroupList;
			if (!hLinkedToken ||  !GetTokenInformation(hLinkedToken,TokenGroups,&groupList,dwLen,&dwLen))
				break;

			for(size_t i = 0; i < groupList.GroupCount; ++i)
			{
				if(EqualSid(pAdminSid,groupList.Groups[i].Sid))
				{
					bFound = 1;
					break;
				}
			}
		}

	}while(FALSE);

	FreeSid( pAdminSid);
	if(hToken) CloseHandle(hToken);
	if(hLinkedToken) CloseHandle(hLinkedToken);

	return bFound;
}

BOOL SystemternlHelper::IsUserLogon()
{
	HANDLE h = SystemternlHelper::FindProcess(L"explorer.exe",NULL);
	if(h)
	{
		CloseHandle(h);
		return TRUE;
	}

	return FALSE;
}

HANDLE SystemternlHelper::GetProcessFullNameList(LPCTSTR lpcszName, list<CString>& strFullNameLst)
{
	EnablePrivilege(SE_DEBUG_NAME, TRUE);

	if(COSChecker::IsWow64())
		return Get64ProcessFullNameLst(lpcszName,strFullNameLst);

	HANDLE hProcessTaget = NULL;
	HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);   
	if(hSnapshot == (HANDLE)-1) 
		return hProcessTaget;

	PROCESSENTRY32 pe = { 0 };
	pe.dwSize = sizeof(pe);
	DWORD dwDesiredAccess = PROCESS_QUERY_INFORMATION | PROCESS_VM_READ;

	BOOL bMore = ::Process32First(hSnapshot,&pe);
	while(bMore)
	{   
		if(_tcsicmp(lpcszName, pe.szExeFile) == 0)
		{
			TCHAR szFullpath[1024] = {0};
			HANDLE hProcess = NULL;
			if (hProcess = ::OpenProcess(dwDesiredAccess, FALSE, pe.th32ProcessID))
			{
				TCHAR szLongPath[MAX_PATH] = {};

				::GetModuleFileNameEx(hProcess, NULL, szFullpath, _countof(szFullpath) - 1);
				GetLongPathName(szFullpath, szLongPath, MAX_PATH);
				strFullNameLst.push_back(szLongPath);

				::CloseHandle(hProcess);
			}
		}
		bMore = ::Process32Next(hSnapshot,&pe);   
	}

	::CloseHandle(hSnapshot);
	return hProcessTaget;

}

HANDLE SystemternlHelper::Get64ProcessFullNameLst(LPCTSTR lpcszName, list<CString>& strFullNameLst)
{
	EnablePrivilege(SE_DEBUG_NAME, TRUE);
	PFNNtQuerySystemInformation NtQuerySystemInformation = NULL;
	pfnNtQueryInformationProcess NtQueryInformationProcess = NULL;
	HMODULE hDllMod = GetModuleHandle(L"ntdll.dll") ;
	if ( hDllMod == NULL )
	{
		hDllMod = LoadLibrary( L"ntdll.dll" ) ;
		if ( hDllMod == NULL ) 
			return NULL;
	}

	NtQuerySystemInformation = (PFNNtQuerySystemInformation)GetProcAddress (hDllMod,"NtQuerySystemInformation") ;
	NtQueryInformationProcess = (pfnNtQueryInformationProcess)GetProcAddress(hDllMod,"NtQueryInformationProcess");
	if(!NtQuerySystemInformation || !NtQueryInformationProcess)
		return FALSE;

	LPBYTE lpBuf = NULL ;
	UINT nSize = 16 * 1024;
	while ( TRUE )
	{
		if((lpBuf = new BYTE [nSize]) == NULL)
			return NULL;

		//枚举进程信息
		SYSTEM_INFORMATION_CLASS Information = SystemProcessInformation;
		//NTSTATUS status = NtQuerySystemInformation ((SYSTEM_INFORMATION_CLASS)SystemProcessesAndThreadsInformation,lpBuf,nSize,0) ;
		NTSTATUS status = NtQuerySystemInformation (Information,lpBuf,nSize,0) ;
		if (!NT_SUCCESS(status))
		{
			if (status == STATUS_INFO_LENGTH_MISMATCH) // 检测是否返回缓冲区不够大
			{
				nSize += 4 * 1024 ;
				delete []lpBuf ;
				continue ;
			}
			else
			{
				delete []lpBuf;
				return NULL;
			}
		}
		else
			break ;
	}

	HANDLE hProcessTaget = NULL,hProcess = NULL;
	DWORD dwDesiredAccess = PROCESS_QUERY_INFORMATION | PROCESS_VM_READ;

	PSYSTEM_PROCESSES pSysProcess = (PSYSTEM_PROCESSES)lpBuf ;

	while(pSysProcess->NextEntryDelta != 0)
	{
		if (pSysProcess->ProcessName.Buffer != NULL && _tcsicmp(pSysProcess->ProcessName.Buffer,lpcszName) == 0)
		{
			HANDLE hProcess = NULL;
			if( hProcess = ::OpenProcess(dwDesiredAccess, FALSE, pSysProcess->ProcessId))
			{

				TCHAR szFullpath[1024] = {0};
				UNICODE_STRING*  pIMGPath = NULL;
				NtQueryInformationProcess(hProcess, (PROCESSINFOCLASS)ProcessImageFileName,(PVOID)szFullpath,sizeof(szFullpath),NULL);
				pIMGPath = (PUNICODE_STRING)szFullpath;
				strFullNameLst.push_back(DosPathToRealPath(pIMGPath->Buffer));

				::CloseHandle(hProcess);
			}
		}
		pSysProcess = (PSYSTEM_PROCESSES)( (DWORD)pSysProcess + pSysProcess->NextEntryDelta ) ;
	}

	delete []lpBuf ;
	return hProcessTaget;
}

HANDLE SystemternlHelper::GetProcessFullName(LPCTSTR lpcszName, CString& strFullName)
{
	EnablePrivilege(SE_DEBUG_NAME, TRUE);

	if(COSChecker::IsWow64())
		return Get64ProcessFullName(lpcszName,strFullName);

	HANDLE hProcessTaget = NULL;
	HANDLE hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);   
	if(hSnapshot == (HANDLE)-1) 
		return hProcessTaget;

	PROCESSENTRY32 pe = { 0 };
	pe.dwSize = sizeof(pe);
	DWORD dwDesiredAccess = PROCESS_QUERY_INFORMATION | PROCESS_VM_READ;

	BOOL bMore = ::Process32First(hSnapshot,&pe);
	while(bMore)
	{   
		if(_tcsicmp(lpcszName, pe.szExeFile) == 0)
		{
			TCHAR szFullpath[1024] = {0};
			HANDLE hProcess = NULL;
			if (hProcess = ::OpenProcess(dwDesiredAccess, FALSE, pe.th32ProcessID))
			{
				TCHAR szLongPath[MAX_PATH] = {};

				::GetModuleFileNameEx(hProcess, NULL, szFullpath, _countof(szFullpath) - 1);
				GetLongPathName(szFullpath, szLongPath, MAX_PATH);
				strFullName = szLongPath;

				::CloseHandle(hProcess);
				break;
			}
			else
			{
				/*
				CString strErr;
				strErr.Format(_T("OpenProcess Failed!,err=%u"), ::GetLastError());
				::MessageBox(NULL, strErr, _T("Error"), MB_OK);
				*/
			}
		}
		bMore = ::Process32Next(hSnapshot,&pe);   
	}

	::CloseHandle(hSnapshot);
	return hProcessTaget;
}

HANDLE SystemternlHelper::Get64ProcessFullName(LPCTSTR lpcszName, CString& strFullName)
{
	EnablePrivilege(SE_DEBUG_NAME, TRUE);
	PFNNtQuerySystemInformation NtQuerySystemInformation = NULL;
	pfnNtQueryInformationProcess NtQueryInformationProcess = NULL;
	HMODULE hDllMod = GetModuleHandle(L"ntdll.dll") ;
	if ( hDllMod == NULL )
	{
		hDllMod = LoadLibrary( L"ntdll.dll" ) ;
		if ( hDllMod == NULL ) 
			return NULL;
	}

	NtQuerySystemInformation = (PFNNtQuerySystemInformation)GetProcAddress (hDllMod,"NtQuerySystemInformation") ;
	NtQueryInformationProcess = (pfnNtQueryInformationProcess)GetProcAddress(hDllMod,"NtQueryInformationProcess");
	if(!NtQuerySystemInformation || !NtQueryInformationProcess)
		return FALSE;

	LPBYTE lpBuf = NULL ;
	UINT nSize = 16 * 1024;
	while ( TRUE )
	{
		if((lpBuf = new BYTE [nSize]) == NULL)
			return NULL;

		//枚举进程信息
		SYSTEM_INFORMATION_CLASS Information = SystemProcessInformation;
		NTSTATUS status = NtQuerySystemInformation (Information,lpBuf,nSize,0) ;
		if (!NT_SUCCESS(status))
		{
			if (status == STATUS_INFO_LENGTH_MISMATCH) // 检测是否返回缓冲区不够大
			{
				nSize += 4 * 1024 ;
				delete []lpBuf ;
				continue ;
			}
			else
			{
				delete []lpBuf;
				return NULL;
			}
		}
		else
			break ;
	}

	HANDLE hProcessTaget = NULL,hProcess = NULL;
	DWORD dwDesiredAccess = PROCESS_QUERY_INFORMATION | PROCESS_VM_READ;
	
	PSYSTEM_PROCESSES pSysProcess = (PSYSTEM_PROCESSES)lpBuf ;

	while(pSysProcess->NextEntryDelta != 0)
	{
		if (pSysProcess->ProcessName.Buffer != NULL && _tcsicmp(pSysProcess->ProcessName.Buffer,lpcszName) == 0)
		{
			HANDLE hProcess = NULL;
			if( hProcess = ::OpenProcess(dwDesiredAccess, FALSE, pSysProcess->ProcessId))
			{
				
				TCHAR szFullpath[1024] = {0};
				UNICODE_STRING*  pIMGPath = NULL;
				NtQueryInformationProcess(hProcess, (PROCESSINFOCLASS)ProcessImageFileName,(PVOID)szFullpath,sizeof(szFullpath),NULL);
				pIMGPath = (PUNICODE_STRING)szFullpath;
				strFullName = DosPathToRealPath(pIMGPath->Buffer);

				::CloseHandle(hProcess);
				break;
			}
		}
		pSysProcess = (PSYSTEM_PROCESSES)( (DWORD)pSysProcess + pSysProcess->NextEntryDelta ) ;
	}

	delete []lpBuf ;
	return hProcessTaget;
}

BOOL SystemternlHelper::DosPathToNtPath( LPTSTR pszDosPath, LPTSTR pszNtPath )
{
	TCHAR			szDriveStr[500];
	TCHAR			szDrive[3];
	TCHAR			szDevName[100];
	INT				cchDevName;
	INT				i;

	//检查参数
	if(!pszDosPath || !pszNtPath )
		return FALSE;

	//获取本地磁盘字符串
	if(GetLogicalDriveStrings(sizeof(szDriveStr), szDriveStr))
	{
		for(i = 0; szDriveStr[i]; i += 4)
		{
			if(!lstrcmpi(&(szDriveStr[i]), _T("A:\\")) || !lstrcmpi(&(szDriveStr[i]), _T("B:\\")))
				continue;

			szDrive[0] = szDriveStr[i];
			szDrive[1] = szDriveStr[i + 1];
			szDrive[2] = '\0';
			if(!QueryDosDevice(szDrive, szDevName, 100))//查询 Dos 设备名
				return FALSE;

			cchDevName = lstrlen(szDevName);
			if(_tcsnicmp(pszDosPath, szDevName, cchDevName) == 0)//命中
			{
				lstrcpy(pszNtPath, szDrive);//复制驱动器
				lstrcat(pszNtPath, pszDosPath + cchDevName);//复制路径

				return TRUE;
			}			
		}
	}

	lstrcpy(pszNtPath, pszDosPath);

	return FALSE;
}

BOOL SystemternlHelper::GetProcessFullPath( DWORD dwPID, TCHAR* pszFullPath)
{
	TCHAR       szImagePath[MAX_PATH];  
	HANDLE      hProcess;  

	if(!pszFullPath)  
		return FALSE;  

	pszFullPath[0] = '\0';  
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, 0, dwPID);  
	if(!hProcess)  
		return FALSE;  

	if(!GetProcessImageFileName(hProcess, szImagePath, MAX_PATH))  
	{  
		CloseHandle(hProcess);  
		return FALSE;  
	}  

	if(!DosPathToNtPath(szImagePath, pszFullPath))  
	{  
		CloseHandle(hProcess);  
		return FALSE;  
	}  

	CloseHandle(hProcess);  

	return TRUE;  
}


int SystemternlHelper::GetAllTokensByNameAndPId(LPCTSTR lpcszName, HANDLE* pTokens, int nMax, DWORD dwPid)
{
	int nResult = 0;
	CString strMsg;
	if (NULL == lpcszName || _tcslen(lpcszName) <= 0 || NULL == pTokens || 0 >= nMax)
		return nResult;

	HANDLE *phProcesses = new HANDLE[nMax + 2];
	if (NULL == phProcesses)
		return nResult;

	do 
	{
		memset(phProcesses, 0, (nMax + 2) * sizeof(HANDLE));

		int nRet = FindAllProcesses(lpcszName, phProcesses, nMax);
		if (nRet <= 0 || nRet > nMax)
			break;

		int nGetToken = 0;
		for (int nIndex = 0; nIndex < nRet; nIndex++)
		{
			if (NULL == phProcesses[nIndex])
				continue;
			if(dwPid != GetProcessId(phProcesses[nIndex]))
			{
				CloseHandle(phProcesses[nIndex]);
				continue;
			}
			HANDLE hToken = NULL;
			BOOL bRet = OpenProcessToken(phProcesses[nIndex], TOKEN_ALL_ACCESS, &hToken);
			CloseHandle(phProcesses[nIndex]);
			phProcesses[nIndex] = NULL;
			if (NULL != hToken && bRet)
			{
				pTokens[nGetToken] = hToken;
				nGetToken++;
			}
		}

		nResult = nGetToken;

	} while (FALSE);

	if (NULL != phProcesses)
	{
		delete []phProcesses;
		phProcesses = NULL;
	}

	return nResult;	
}