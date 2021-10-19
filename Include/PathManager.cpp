#pragma once
#include "stdafx.h"
#include <PathManager.h>

CString PathManager::MyExeFullPath()
{
	TCHAR szBuffer[1024] = { 0 };
	::GetModuleFileName(NULL, szBuffer, MAX_PATH);
	return szBuffer;
}

CString PathManager::MyExeName(BOOL bRemoveExt)
{
	TCHAR szBuffer[1024] = { 0 };
	::GetModuleFileName(NULL, szBuffer, MAX_PATH);

	CString s = szBuffer;
	int nPos = s.ReverseFind(_T('\\'));
	if (nPos > 0)
	{
		s = s.Mid(nPos + 1);
	}
	else
	{
		s = _T("");
	}

	if (bRemoveExt)
	{
		nPos = s.ReverseFind(_T('.'));
		if (nPos > 0)
			s = s.Left(nPos);
	}

	return s;
}

CString PathManager::MyExePath(LPCTSTR strCombinePath)
{
	TCHAR szBuffer[1024] = {0};
	::GetModuleFileName(NULL, szBuffer, MAX_PATH);
	::PathRemoveFileSpec(szBuffer);

	TCHAR szDest[2048] = {0};
	::PathCombine(szDest, szBuffer, strCombinePath);
	return szDest;
}


CString PathManager::MyCombinePath(LPCTSTR strParentPath, LPCTSTR strCombinePath)
{
	TCHAR szDest[2048] = {0};
	::PathCombine(szDest, strParentPath, strCombinePath);
	return szDest;
}

//MyMutex g_InstallPathLock;
CString PathManager::GetProgramInstallPath(HKEY RegDir, LPCTSTR strRegPath, LPCTSTR strKeyName)
{
	//MyScopeLock lock(g_InstallPathLock);

	static CString strInsPath = _T("");
	if (!strInsPath.IsEmpty())
		return strInsPath;


	DWORD dwTypeSZ = REG_SZ;
	TCHAR tczPath[MAX_PATH] = { 0 };
	DWORD dwSize = sizeof(tczPath);
	SHGetValue(RegDir, strRegPath, strKeyName, &dwTypeSZ, tczPath, &dwSize);

	if (PathIsDirectory(tczPath))
		strInsPath = tczPath;
	else
		strInsPath = MyExePath(L"");

	return strInsPath;
}


CString PathManager::GetCurModulePath(HMODULE hMod,LPCTSTR strCombinePath)
{
	WCHAR czFileName[1024] = {0};
	GetModuleFileName(hMod,czFileName,_countof(czFileName)-1);
	PathRemoveFileSpec(czFileName);

	return MyCombinePath(czFileName, strCombinePath);
}

CString PathManager::GetFileNameFromPath(CString strPath, BOOL bNeedExtName)
{
	int nPos = strPath.ReverseFind(L'\\');
	if (nPos != -1)
		strPath = strPath.Mid(nPos + 1);

	if (!bNeedExtName)
	{
		nPos = strPath.ReverseFind(L'.');
		if (nPos != -1)
			strPath = strPath.Left(nPos);
	}

	return strPath;
}


BOOL PathManager::CreateDirRecursive( CString strDir, BOOL TopisDir )
{
	if(PathIsDirectory(strDir))
		return TRUE;


	CString strTmpDir;
	CString strLastDir;

	for(int i=0;i<strDir.GetLength();i++)
	{
		TCHAR ch = strDir.GetAt(i);
		strTmpDir += ch;
		if(ch == _T('\\') || ch == _T('/'))
		{
			::CreateDirectory(strTmpDir, NULL);
			strLastDir = strTmpDir;
		}
	}

	if (TopisDir)
	{
		::CreateDirectory(strDir, NULL);
		strLastDir = strDir;
	}

	return ::PathIsDirectory(strLastDir);
}
