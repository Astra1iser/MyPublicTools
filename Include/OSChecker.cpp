#pragma once
#include "stdafx.h"
#include "OSChecker.h"

OSVERSIONINFOEX COSChecker::m_sOSInfo;
BOOL COSChecker::m_bInited = 0;
int  COSChecker::m_nWow64 = -1;
COSChecker::LPFN_ISWOW64PROCESS COSChecker::m_fnIsWow64Process = NULL;
COSChecker::LPFN_Wow64DisableWow64FsRedirection COSChecker::m_fnWow64DisableWow64FsRedirection = NULL;
COSChecker::LPFN_Wow64RevertWow64FsRedirection COSChecker::m_fnWow64RevertWow64FsRedirection = NULL;
PVOID COSChecker::m_old_redirect = NULL;
MyMutex COSChecker::m_cs;

BOOL COSChecker::GetCurrentVer(DWORD& dwMain,DWORD& dwMinor)
{
	MyScopeLock lck(m_cs);

	if(!m_bInited && !_Init())
		return FALSE;

	dwMain  = m_sOSInfo.dwMajorVersion;
	dwMinor = m_sOSInfo.dwMinorVersion;
	return TRUE;
}

BOOL COSChecker::GetVersionStruct(OSVERSIONINFOEX& info)
{
	MyScopeLock lck(m_cs);

	if(!m_bInited && !_Init())
		return FALSE;

	memcpy(&info,&m_sOSInfo,sizeof(OSVERSIONINFOEX));
	return TRUE;
}

CString COSChecker::GetOSString()
{
	MyScopeLock lck(m_cs);

	if(!m_bInited && !_Init())
		return L"";

	BOOL bIs64 = IsWow64();
	if(m_sOSInfo.dwMajorVersion == 6 && m_sOSInfo.dwMinorVersion == 1)	//把常见的放在前面，少点比较次数吧
	{
		if(m_sOSInfo.wProductType == VER_NT_WORKSTATION)		//2088 R2的版本也是6.1
			return L"win7";
		else
			return L"win2008";
	}
	else if(m_sOSInfo.dwMajorVersion == 5 && m_sOSInfo.dwMinorVersion == 1)
		return L"winxp";
	else if(m_sOSInfo.dwMajorVersion == 5 && m_sOSInfo.dwMinorVersion == 2)
	{
		if(m_sOSInfo.wProductType == VER_NT_WORKSTATION && bIs64)
			return L"winxp";
		else
			return L"win2003";
	}
	else if(m_sOSInfo.dwMajorVersion == 6 && m_sOSInfo.dwMinorVersion == 0)
	{
		if(m_sOSInfo.wProductType == VER_NT_WORKSTATION)
			return L"vista";
		else
			return L"win2008";
	}
	else if(m_sOSInfo.dwMajorVersion == 5 && m_sOSInfo.dwMinorVersion == 0)
		return L"win2000";
	else if(m_sOSInfo.dwMajorVersion == 6 && m_sOSInfo.dwMinorVersion == 2)
		return L"win8";
	else if(m_sOSInfo.dwMinorVersion <=4)
		return L"winnt";
	else
		return L"-";
}

BOOL COSChecker::IsWow64()
{
	MyScopeLock lck(m_cs);

	if(m_nWow64 != -1)
		return m_nWow64 != 0;

	if (m_fnIsWow64Process == NULL)
	{
		HMODULE hKernel = GetModuleHandle(_T("kernel32"));
		m_fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(hKernel, "IsWow64Process");
	}

	if(!m_fnIsWow64Process)
		return FALSE;

	BOOL bIsWow64 = FALSE;
	if (!m_fnIsWow64Process(GetCurrentProcess(), &bIsWow64))
		return FALSE;  		// handle error.default as false

	m_nWow64 = bIsWow64 ? 1 : 0;
	return bIsWow64;
}

BOOL COSChecker::IsWindowsXPLater()
{
	MyScopeLock lck(m_cs);
	if(!m_bInited && !_Init())
		return FALSE;

	if ((m_sOSInfo.dwMajorVersion>5) || (m_sOSInfo.dwMajorVersion==5 && m_sOSInfo.dwMinorVersion>=1))
	{
		return TRUE;
	}
	return FALSE;
}

BOOL COSChecker::DisableRedirect()
{
	MyScopeLock lck(m_cs);

	if(!m_fnWow64DisableWow64FsRedirection)
	{
		HMODULE hKernel = GetModuleHandle(_T("kernel32"));
		m_fnWow64DisableWow64FsRedirection = (LPFN_Wow64DisableWow64FsRedirection)GetProcAddress(hKernel, "Wow64DisableWow64FsRedirection");
	}

	if(!m_fnWow64DisableWow64FsRedirection)
		return FALSE;

	m_fnWow64DisableWow64FsRedirection(&m_old_redirect); 
	return TRUE;
}

// 恢复以前的重定向
BOOL COSChecker::RestoreRedirect()
{
	MyScopeLock lck(m_cs);

	if(!m_fnWow64RevertWow64FsRedirection)
	{
		HMODULE hKernel = GetModuleHandle(_T("kernel32"));
		m_fnWow64RevertWow64FsRedirection = (LPFN_Wow64RevertWow64FsRedirection)GetProcAddress(hKernel, "Wow64RevertWow64FsRedirection");
	}

	if(!m_fnWow64RevertWow64FsRedirection)
		return FALSE;

	m_fnWow64RevertWow64FsRedirection(m_old_redirect); 
	return TRUE;
}

BOOL COSChecker::_Init()
{
	memset(&m_sOSInfo,0,sizeof(OSVERSIONINFOEX));
	m_sOSInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if (!GetVersionEx((OSVERSIONINFO*)&m_sOSInfo))
		return FALSE;

	return TRUE;
}

BOOL COSChecker::IsXp()
{
	MyScopeLock lck(m_cs);
	if(!m_bInited && !_Init())
		return FALSE;

	return m_sOSInfo.dwMajorVersion == 5 && m_sOSInfo.dwMinorVersion == 1;
}

BOOL COSChecker::IsServerOS()
{
	OSVERSIONINFOEX osver;
	memset(&osver, 0, sizeof(OSVERSIONINFOEX));
	osver.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if(::GetVersionEx((LPOSVERSIONINFOW)&osver))
	{
		if (osver.wProductType == VER_NT_DOMAIN_CONTROLLER || osver.wProductType == VER_NT_SERVER)
		{
			return TRUE;
		}
	}

	return FALSE;
}