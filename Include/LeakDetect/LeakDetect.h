#pragma once
#include "stdafx.h"

typedef struct tagInstallVul
{
	DWORD	dwKey;
	DWORD	dwRegKey64;

	CString	strDisplayName;
	CString	strUninstallString;

	tagInstallVul()
	{
		dwKey = 0;
		dwRegKey64 = 0;
	}

}INSTALLVUL, *LPINSTALLVUL;

map<CString, INSTALLVUL> m_mapInstVul;
__declspec(selectany) BOOL g_SomGlobal_bWin64 = IsOS(OS_WOW6432);
__declspec(selectany) BOOL g_bSomKBIDVersion = FALSE;		// 是否处理 KBID 的小版本号，目前的逻辑是默认处理小版本号，如果发现加载漏洞库后少于 3 个补丁填了小版本号，就改成不处理小版本号，避免读取已安装的补丁数据和漏洞库的数据不匹配

DWORD LPCTSTR2DWORD(LPCTSTR lpctsrt);

BOOL EntIsWin7AndLater();
BOOL EntIsWin10AndLater();

void ConvertGuidToProuctCode(LPWSTR szMsiKey, LPWSTR szProductCode, DWORD dwProductCodeSize);
BOOL ConvertSid(PSID pSid, LPWSTR TextualSid, LPDWORD lpdwBufferLen);

BOOL GetLocalSid(LPWSTR szSid, DWORD dwSize);
BOOL GetLocalSid(LPWSTR szSid, DWORD dwSize);

DWORD FormatKBID(LPCWSTR lpKBID, LPCWSTR* lppKBIDFirst = NULL, LPCWSTR* lppKBIDLast = NULL, BOOL bRequireKBPrefix = FALSE);

BOOL IsXPHotfix(LPCWSTR szUninstallString);
BOOL IsXPHotfix(LPCWSTR szDisplayName, LPCWSTR szUninstallString);

BOOL EnumAllInstallVul();
BOOL EnumFromInstaller(map<CString, INSTALLVUL> &mapInstVul);
BOOL EnumFromRegUninstall(map<CString, INSTALLVUL> &mapInstVul);
BOOL EnumFromPackages(map<CString, INSTALLVUL> &mapInstVul);
BOOL IsVulInstall(DWORD dwKey);

//导出函数
extern "C" __declspec(dllexport) BOOL IsPatchInstalled(LPCTSTR lpKBNumber);