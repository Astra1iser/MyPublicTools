#pragma once
#include "stdafx.h"

// 已安装补丁信息
typedef struct _InstalledItem
{
	_InstalledItem()
		: dwKey(0)
	{

	}

	DWORD dwKey;
	CString strKBID;				// ID
	CString strName;				// 漏洞名称
	CString strDate;				// 发布时间

} INSTALLEDITEM, * LPINSTALLEDITEM;


vector<INSTALLEDITEM> vecInstalled;

DWORD LPCTSTR2DWORD(LPCTSTR lpctsrt);

BOOL IsVulInstall(DWORD dwKey);


//导出函数
extern "C" __declspec(dllexport) BOOL IsPatchInstalled(LPCTSTR lpKBNumber);