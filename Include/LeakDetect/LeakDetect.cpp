#include "LeakDetect.h"


BOOL IsPatchInstalled(LPCTSTR lpKBNumber)
{
	//m_mapInstVul 这里面是最终的补丁数据
	if(	EnumAllInstallVul())
	{
		//判断形参中的补丁号是否存在
		return IsVulInstall(LPCTSTR2DWORD(lpKBNumber));
	}
	else
	{
		return FALSE;
	}
}


DWORD LPCTSTR2DWORD(LPCTSTR lpctsrt)
{
	CString str;
	str = lpctsrt;

	//判断是否是KB开头
	if((str[0] == 'K' || str[0] == 'k') && (str[1] == 'B' || str[1] == 'b'))
	{
		CString strTemp ;

		for(int i = 2 ; i < str.GetLength() ; i++)
		{
			if(str[i]>='0' && str[i]<='9')
				strTemp += str[i];
		}
		//提取去除KB后的补丁号
		return _ttoi(strTemp);
	}
	else
	{
				return -1;
	}
}


//判断是否是win7或者以后的系统
BOOL EntIsWin7AndLater()
{
	OSVERSIONINFO osver;

	osver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	GetVersionEx(&osver);

	if (osver.dwMajorVersion < 6)
		return FALSE;
	else if (osver.dwMajorVersion == 6)
		return osver.dwMinorVersion >= 1;
	else
		return TRUE;
}


//判断是否是win10或者以后的系统
BOOL EntIsWin10AndLater()
{
	OSVERSIONINFO osver;
	osver.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osver);
	if (osver.dwMajorVersion < 10)
		return FALSE;
	else if (osver.dwMajorVersion == 10)
		return osver.dwMinorVersion >= 0;
	else
		return TRUE;
}


//sid转换
BOOL ConvertSid(PSID pSid, LPWSTR TextualSid, LPDWORD lpdwBufferLen)
{
	PSID_IDENTIFIER_AUTHORITY psia;
	DWORD dwSubAuthorities;
	DWORD dwSidRev = SID_REVISION;
	DWORD dwCounter;
	DWORD dwSidSize;

	if(!IsValidSid(pSid))
	{
		return FALSE;
	}
	psia = GetSidIdentifierAuthority(pSid);
	dwSubAuthorities = *GetSidSubAuthorityCount(pSid);
	dwSidSize=(15 + 12 + (12 * dwSubAuthorities)+ 1)* sizeof(WCHAR);
	if (*lpdwBufferLen < dwSidSize)
	{
		*lpdwBufferLen = dwSidSize;
		SetLastError(ERROR_INSUFFICIENT_BUFFER);
		return FALSE;
	}

	WCHAR szTemp[MAX_PATH] = {0};
	StringCbPrintf(szTemp,sizeof(szTemp), L"S-%lu-", dwSidRev);
	StringCbCopy(TextualSid,*lpdwBufferLen,szTemp);

	if ((psia->Value[0] != 0)|| (psia->Value[1] != 0))
	{
		StringCbPrintf(szTemp,MAX_PATH,L"0x%02hx%02hx%02hx%02hx%02hx%02hx", 
			(USHORT)psia->Value[0], 
			(USHORT)psia->Value[1], 
			(USHORT)psia->Value[2], 
			(USHORT)psia->Value[3], 
			(USHORT)psia->Value[4], 
			(USHORT)psia->Value[5]);
		StringCbCat(TextualSid,*lpdwBufferLen,szTemp);
	}
	else 
	{
		StringCbPrintf(szTemp,sizeof(szTemp),L"%lu", 
			(ULONG)(psia->Value[5]) + 
			(ULONG)(psia->Value[4] <<  8) + 
			(ULONG)(psia->Value[3] << 16) + 
			(ULONG)(psia->Value[2] << 24));
		StringCbCat(TextualSid,*lpdwBufferLen,szTemp);
	}

	for (dwCounter=0;dwCounter < dwSubAuthorities;dwCounter++)
	{
		StringCbPrintf(szTemp, sizeof(szTemp), L"-%lu",*GetSidSubAuthority(pSid, dwCounter));
		StringCbCat(TextualSid,*lpdwBufferLen,szTemp);
	}

	return TRUE;
}


//获取本地用户sid
BOOL GetLocalSid(LPWSTR szSid, DWORD dwSize)
{
	WCHAR szUserName[MAX_PATH] = {0};
	WCHAR szUserSid[MAX_PATH] = {0};
	WCHAR szUserDomain[MAX_PATH] = {0};
	SID_NAME_USE snu;

	DWORD dwNameLen = MAX_PATH;
	DWORD dwDomainLen = MAX_PATH;
	DWORD dwSidLen = sizeof(szUserSid);

	if (!GetUserName(szUserName, &dwNameLen))
	{
		return FALSE;
	}

	if (LookupAccountName(NULL, szUserName, (PSID)szUserSid, &dwSidLen, szUserDomain, &dwDomainLen, &snu))
	{
		if (IsValidSid((PSID)szUserSid))
		{
			if (ConvertSid((PSID)szUserSid, szSid, &dwSize))
			{
				return TRUE;
			}
		}
	}
	return  FALSE;
}

//获取系统sid
BOOL GetSystemSid(LPWSTR szSid,DWORD dwSize)
{
	WCHAR szUserName[MAX_PATH] = {0};
	WCHAR szUserSid[MAX_PATH] = {0};
	WCHAR szUserDomain[MAX_PATH] = {0};
	SID_NAME_USE snu;

	DWORD dwNameLen = MAX_PATH;
	DWORD dwSidLen = sizeof(szUserSid);
	DWORD dwDomainLen = MAX_PATH;

	StringCbCopy(szUserName, sizeof(szUserName), L"system");
	if (LookupAccountName(NULL, szUserName, (PSID)szUserSid, &dwSidLen, szUserDomain, &dwDomainLen, &snu))
	{
		if (IsValidSid((PSID)szUserSid))
		{
			if (ConvertSid((PSID)szUserSid,szSid,&dwSize))
			{
				return TRUE;
			}
		}
	}
	return  FALSE;
}

//没看懂干啥的
void ConvertGuidToProuctCode(LPWSTR szMsiKey, LPWSTR szProductCode, DWORD dwProductCodeSize)
{
	//{03DDA387-EBB9-49CD-9899-A7E8A6E78946}
	//783ADD30 9BBE DC94  8999 7A8E6A7E9864
	if (wcslen(szMsiKey) >= 32)
	{		
		wstring strMsi = szMsiKey;
		wstring str1, str2, str3, str4;

		str1 = strMsi.substr(0, 8);
		_wcsrev (const_cast<WCHAR*>(str1.c_str()));
		str2 = strMsi.substr(8, 4);
		_wcsrev (const_cast<WCHAR*>(str2.c_str()));
		str3 = strMsi.substr(12, 4);
		_wcsrev (const_cast<WCHAR*>(str3.c_str()));
		str4 = strMsi.substr(16, 16);

		for (int i = 0; i < 16; i++)
		{
			if (i % 2)
			{
				WCHAR szTemp = str4[i];
				str4[i] = str4[i - 1];
				str4[i - 1] = szTemp;
			}
		}

		StringCbPrintf(szProductCode, dwProductCodeSize, L"{%s-%s-%s-%s-%s}", str1.c_str(), str2.c_str(), str3.c_str(), str4.substr(0,4).c_str(), str4.substr(4, 12).c_str());
	}
}

// 把可能含有补丁号的字符串格式化到唯一KEY 的辅助函数，如果没有补丁号，返回 0
DWORD FormatKBID(LPCWSTR lpKBID, LPCWSTR* lppKBIDFirst, LPCWSTR* lppKBIDLast, BOOL bRequireKBPrefix)
{
	if (lpKBID == NULL || lpKBID[0] == 0)
		return 0;

	LPCWSTR lpKBHeader = NULL, lpKBTail = NULL;

	// 如果 bRequireKBPrefix 为 TRUE 或者不以数字开头，那么查找有没有 KB或者QH 开头的数字，如果有，认为这个数字就是 KB 号
	if (bRequireKBPrefix || (lpKBID[0] < L'0' || lpKBID[0] > L'9'))
	{
		for (LPCWSTR p = lpKBID; *p; p++)
		{
			if (((*p == L'k' || *p == L'K') && ((*(p + 1) == L'b' || *(p + 1) == L'B'))) || ((*p == L'q' || *p == L'Q') && ((*(p + 1) == L'h' || *(p + 1) == L'H'))))
			{
				lpKBHeader = p + 2;
				break;
			}
		}
	}
	else
		lpKBHeader = lpKBID;

	if (lpKBHeader == NULL)
		return 0;

	DWORD kbid = 0;
	LPCWSTR lpVersion = NULL;
	for (LPCWSTR p = lpKBHeader; *p; p++)
	{
		if (*p < L'0' || *p > L'9')
		{
			// 判断有没有小版本号(标准是以 -v _V 开始数字为标准)
			if ((*p == L'-' || *p == L'_') && ((*(p + 1) == L'v' || *(p + 1) == L'V')))
				lpVersion = p + 2;

			break;
		}
		else
		{
			kbid = (kbid * 10) + (*p - L'0');
			lpKBTail	= p + 1;
		}
	}

	// 合法的 KBID 至少 6 位十进制数字，最大值不能超过 0xFFFFFF
	if (kbid < 100000 || kbid > 0xFFFFFF)
		return 0;

	if (lpVersion && g_bSomKBIDVersion)
	{
		DWORD version = 0;
		for (LPCWSTR p = lpVersion; *p; p++)
		{
			if (*p == L'.')
				continue;

			if (*p < L'0' || *p > L'9')
				break;
			else
			{
				version = (version * 10) + (*p - L'0');
				lpKBTail	= p + 1;
			}
		}
		kbid |= ((version & 0x7F) << 24);
	}

	// 强制置最高位为0
	kbid &= 0x7FFFFFFF;

	if (lppKBIDFirst)
		*lppKBIDFirst	= lpKBHeader;
	if (lppKBIDLast)
		*lppKBIDLast	= lpKBTail;

	return kbid;
}

//是否是xp系统补丁
BOOL IsXPHotfix(LPCWSTR szUninstallString)
{
	WCHAR szFileName[MAX_PATH] = {0};
	StringCbCopy(szFileName, sizeof(szFileName), PathFindFileName(szUninstallString));

	WCHAR szParam1[MAX_PATH] = {0};
	StringCbCopy(szParam1, sizeof(szParam1), szUninstallString);

	_wcslwr_s(szParam1, MAX_PATH);
	_wcslwr_s(szFileName, MAX_PATH);

	CString strUninstFilePath = szParam1;
	strUninstFilePath.Trim(L"\"");
	CString strFileName = szFileName;
	strFileName.Trim(L"\"");
	if (lstrcmp(strFileName, L"spuninst.exe") == 0 && wcsstr(szParam1, L"$ntuninstall") != NULL)
	{
		// 确认卸载文件是否真的存在
		if (PathFileExists(strUninstFilePath))
		{
			return TRUE;
		}
		else
		{
			return FALSE;				
		}
	}

	if (strUninstFilePath.IsEmpty() || lstrcmpi(strUninstFilePath, L"(unsupported)") == 0)
	{
		return FALSE;
	}

	return TRUE;

}

//是否是xp系统补丁
BOOL IsXPHotfix(LPCWSTR szDisplayName, LPCWSTR szUninstallString)
{
	WCHAR szFileName[MAX_PATH] = {0};
	StringCbCopy(szFileName, sizeof(szFileName), PathFindFileName(szUninstallString));

	WCHAR szParam1[MAX_PATH] = {0};
	StringCbCopy(szParam1, sizeof(szParam1), szUninstallString);

	_wcslwr_s(szParam1, MAX_PATH);
	_wcslwr_s(szFileName, MAX_PATH);

	CString strUninstFilePath = szParam1;
	strUninstFilePath.Trim(L"\"");
	CString strFileName = szFileName;
	strFileName.Trim(L"\"");
	if (lstrcmp(strFileName, L"spuninst.exe") == 0 && wcsstr(szParam1, L"$ntuninstall") != NULL)
	{
		// 确认卸载文件是否真的存在
		if (PathFileExists(strUninstFilePath))
		{
			return TRUE;
		}
		else
		{
			return FALSE;				
		}
	}

	LPCWSTR p = StrStrI(szDisplayName, L"KB");
	if (p != NULL)
	{
		for (int i = 0; i < 6; i++)
		{
			if (p[i + 2] < L'0' &&  p[i + 3] > L'9')
				return FALSE;
		}

		// 没有卸载命令，返回失败
		if (strUninstFilePath.IsEmpty() || lstrcmpi(strUninstFilePath, L"(unsupported)") == 0)
		{
			return FALSE;
		}

		return TRUE;
	}

	return FALSE;
}

//从KB*****中去掉KB并提取数字,LPCTSTR2DWORD()函数完全可以用这个替代
DWORD FindKBIDNoKB(LPCWSTR lpKBID)
{
	int nLenCount = lstrlenW(lpKBID);
	if (nLenCount <= 6)
	{
		return 0;
	}

	for (int i = 0; i < nLenCount - 5; i++)
	{
		BOOL bFinded = TRUE;
		for (int j = 0; j < 6; j++)
		{
			if (lpKBID[i + j] < L'0' || lpKBID[i + j] > L'9')
			{
				bFinded = FALSE;
				break;
			}
		}

		if (bFinded)
		{
			return FormatKBID(lpKBID + i);
		}
	}

	return 0;
}

//枚举用户已安装软件的补丁信息
BOOL EnumFromInstaller(map<CString, INSTALLVUL> &mapInstVul)
{
	for (int i = 0; i < 2; i++)
	{
		WCHAR szSid[MAX_PATH] = {0};
		if (i == 0)
		{
			GetLocalSid(szSid,sizeof(szSid));
		}
		else if (i == 1)
		{
			GetSystemSid(szSid,sizeof(szSid));
		}

		WCHAR szUninstallPath[MAX_PATH*2] = {0};	
		StringCbPrintf(szUninstallPath,sizeof(szUninstallPath), L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Installer\\UserData\\%s\\Products", szSid);

		HKEY hKey = NULL;
		REGSAM samDesired = KEY_READ;
		if (g_SomGlobal_bWin64)
			samDesired |= KEY_WOW64_64KEY;

		LONG lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, szUninstallPath, 0, samDesired, &hKey);
		if (lResult != ERROR_SUCCESS)
			continue;

		DWORD index = 0;
		int  nbufLen = MAX_PATH*sizeof(WCHAR);
		WCHAR szName[MAX_PATH] = {0};
		DWORD cbName = nbufLen;
		FILETIME ftLastWriteTime;
		ZeroMemory(&ftLastWriteTime,sizeof(ftLastWriteTime));

		while (RegEnumKeyEx(hKey, index, szName, &cbName, 0, NULL, NULL, &ftLastWriteTime)== ERROR_SUCCESS)
		{
			index++;
			cbName = nbufLen;

			HKEY hSubKey = NULL;
			WCHAR Patches[MAX_PATH];
			StringCbCopy (Patches,sizeof(Patches),szName);
			StringCbCat(Patches,sizeof(Patches),L"\\Patches");
			lResult = RegOpenKeyEx(hKey, Patches, 0, KEY_READ, &hSubKey);
			if (lResult != ERROR_SUCCESS)
				continue;

			DWORD indexPatches = 0;
			int  nbufLenPatches = MAX_PATH*sizeof(WCHAR);
			WCHAR szNamePatches[MAX_PATH] = {0};
			DWORD cbNamePatches = nbufLenPatches;
			FILETIME ftLastWriteTimePatches;
			ZeroMemory(&ftLastWriteTimePatches,sizeof(ftLastWriteTimePatches));

			while (RegEnumKeyEx(hSubKey, indexPatches, szNamePatches, &cbNamePatches, 0, NULL, NULL, &ftLastWriteTimePatches)== ERROR_SUCCESS)
			{
				indexPatches++;
				cbNamePatches = nbufLenPatches;

				HKEY hSubKey1 = NULL;
				WCHAR Patches1[MAX_PATH];

				StringCbCopy (Patches1,sizeof(Patches1),szUninstallPath);
				StringCbCat(Patches1,sizeof(Patches1),L"\\");
				StringCbCat (Patches1,sizeof(Patches1),Patches);
				StringCbCat(Patches1,sizeof(Patches1),L"\\");
				StringCbCat(Patches1,sizeof(Patches1),szNamePatches);

				WCHAR szProductCode[MAX_PATH] = {0};
				WCHAR szPatchesCode[MAX_PATH] = {0};
				ConvertGuidToProuctCode(szName, szProductCode, sizeof(szProductCode));
				ConvertGuidToProuctCode(szNamePatches, szPatchesCode, sizeof(szPatchesCode));

				lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, Patches1, 0, KEY_READ, &hSubKey1);
				if (lResult != ERROR_SUCCESS)
					continue;

				WCHAR szValue11[MAX_PATH] = {0};
				DWORD dwSize11 = MAX_PATH*sizeof(WCHAR);
				BOOL bUninstallable = FALSE;

				INSTALLVUL VulInfo;
				if (ERROR_SUCCESS == RegQueryValueEx(hSubKey1, L"Uninstallable", 0, NULL, (LPBYTE)szValue11, &dwSize11)&& dwSize11 > 0)
				{
					if (szValue11[0] == 1)
					{
						bUninstallable = TRUE;
					}
				}
				if (!bUninstallable)
				{
					RegCloseKey(hSubKey1);
					continue;
				}

				dwSize11 = MAX_PATH*sizeof(WCHAR);;
				szValue11[0] = _T('\0');
				if (ERROR_SUCCESS == RegQueryValueEx(hSubKey1, L"DisplayName", 0, NULL, (LPBYTE)szValue11, &dwSize11)&& dwSize11 > 0)
				{
					VulInfo.strDisplayName = szValue11;
				}

				if (szValue11[0] == _T('\0'))
				{
					RegCloseKey(hSubKey1);
					continue;
				}
				RegCloseKey(hSubKey1);

				VulInfo.strUninstallString.Format(L"MsiExec.exe /package %s /uninstall %s /qb", szProductCode, szPatchesCode);

				DWORD dwKey = FormatKBID(StrStrIW(VulInfo.strDisplayName, L"KB"));
				CString strIndexKey;
				strIndexKey.Format(L"%d+%s",dwKey,VulInfo.strUninstallString);
				mapInstVul[strIndexKey] = VulInfo;
			}
			RegCloseKey(hSubKey);
		}

		RegCloseKey(hKey);
		hKey = NULL;
	}

	return TRUE;
}

//枚举注册表uninstall中的补丁信息
BOOL EnumFromRegUninstall(map<CString, INSTALLVUL> &mapInstVul)
{
	const WCHAR szUninstallPath[] = L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall";

	{
		HKEY hKey = NULL;
		LONG lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, szUninstallPath, 0, KEY_READ, &hKey);
		if (lResult != ERROR_SUCCESS)
			return FALSE;

		int  nbufLen = MAX_PATH * sizeof(WCHAR);
		DWORD index = 0;
		WCHAR szName[MAX_PATH] = {0};
		DWORD cbName = nbufLen;
		FILETIME ftLastWriteTime;

		ZeroMemory(&ftLastWriteTime, sizeof(ftLastWriteTime));

		while (RegEnumKeyEx(hKey, index, szName, &cbName, 0, NULL, NULL, &ftLastWriteTime) == ERROR_SUCCESS)
		{
			index++;
			cbName = nbufLen;

			HKEY hSubKey = NULL;
			lResult = RegOpenKeyEx(hKey, szName, 0, KEY_READ, &hSubKey);
			if (lResult != ERROR_SUCCESS)
				continue;

			WCHAR szValue[MAX_PATH] = {0};
			DWORD dwSize = sizeof(DWORD);

			INSTALLVUL VulInfo;
			dwSize = nbufLen;
			szValue[0] = _T('\0');

			dwSize = nbufLen;
			szValue[0] = L'\0';
			if (ERROR_SUCCESS == RegQueryValueEx(hSubKey, L"DisplayName", 0, NULL, (LPBYTE)szValue, &dwSize) && dwSize > 0)
				VulInfo.strDisplayName = szValue;

			dwSize = nbufLen;
			szValue[0] = L'\0';
			if (ERROR_SUCCESS == RegQueryValueEx(hSubKey, L"UninstallString", 0, NULL, (LPBYTE)szValue, &dwSize) && dwSize > 0)
				VulInfo.strUninstallString = szValue;

			dwSize = nbufLen;
			szValue[0] = L'\0';
			CString strMoreInfoUrl = L"";
			if (ERROR_SUCCESS == RegQueryValueEx(hSubKey, L"MoreInfoURL", 0, NULL, (LPBYTE)szValue, &dwSize) && dwSize > 0)
				strMoreInfoUrl = szValue;


			RegCloseKey(hSubKey);
			hSubKey = NULL;

			if (IsXPHotfix(VulInfo.strUninstallString))
			{
				DWORD dwKey = FormatKBID(StrStrIW(VulInfo.strDisplayName, L"KB"));
				if (dwKey == 0)
				{
					if (!strMoreInfoUrl.IsEmpty())
					{
						dwKey = FindKBIDNoKB(strMoreInfoUrl);
						//WRITE_LOG(L"Get KB:%d form MoreInfoURL : %s", dwKey, szValue);
					}
				}
				if (dwKey != 0)
				{
					CString strIndexKey;
					strIndexKey.Format(L"%d+%s",dwKey,VulInfo.strUninstallString);
					mapInstVul[strIndexKey] = VulInfo;
				}
			}
		}

		RegCloseKey(hKey);
		hKey = NULL;
	}

	if (g_SomGlobal_bWin64)
	{
		HKEY hKey = NULL;
		LONG lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, szUninstallPath, 0, KEY_READ | KEY_WOW64_64KEY, &hKey);
		if (lResult != ERROR_SUCCESS)
			return FALSE;

		int  nbufLen = MAX_PATH * sizeof(WCHAR);
		DWORD index = 0;
		WCHAR szName[MAX_PATH] = {0};
		DWORD cbName = nbufLen;
		FILETIME ftLastWriteTime;

		ZeroMemory(&ftLastWriteTime, sizeof(ftLastWriteTime));

		while (RegEnumKeyEx(hKey, index, szName, &cbName, 0, NULL, NULL, &ftLastWriteTime) == ERROR_SUCCESS)
		{
			index++;
			cbName = nbufLen;

			HKEY hSubKey = NULL;
			lResult = RegOpenKeyEx(hKey, szName, 0, KEY_READ, &hSubKey);
			if (lResult != ERROR_SUCCESS)
				continue;

			WCHAR szValue[MAX_PATH] = {0};
			DWORD dwSize = sizeof(DWORD);

			INSTALLVUL VulInfo;
			dwSize = nbufLen;
			szValue[0] = _T('\0');

			dwSize = nbufLen;
			szValue[0] = L'\0';
			if (ERROR_SUCCESS == RegQueryValueEx(hSubKey, L"DisplayName", 0, NULL, (LPBYTE)szValue, &dwSize) && dwSize > 0)
				VulInfo.strDisplayName = szValue;

			dwSize = nbufLen;
			szValue[0] = L'\0';
			if (ERROR_SUCCESS == RegQueryValueEx(hSubKey, L"UninstallString", 0, NULL, (LPBYTE)szValue, &dwSize) && dwSize > 0)
				VulInfo.strUninstallString = szValue;

			dwSize = nbufLen;
			szValue[0] = L'\0';
			CString strMoreInfoUrl = L"";
			if (ERROR_SUCCESS == RegQueryValueEx(hSubKey, L"MoreInfoURL", 0, NULL, (LPBYTE)szValue, &dwSize) && dwSize > 0)
				strMoreInfoUrl = szValue;

			RegCloseKey(hSubKey);
			hSubKey = NULL;

			if (IsXPHotfix(VulInfo.strUninstallString))
			{
				DWORD dwKey = FormatKBID(StrStrIW(VulInfo.strDisplayName, L"KB"));
				if (dwKey == 0)
				{
					dwKey = FindKBIDNoKB(strMoreInfoUrl);
					//WRITE_LOG(L"Get KB:%d form MoreInfoURL : %s", dwKey, szValue);
				}
				if (dwKey != 0)
				{
					CString strIndexKey;
					strIndexKey.Format(L"%d+%s",dwKey,VulInfo.strUninstallString);
					mapInstVul[strIndexKey] = VulInfo;
				}
			}
		}

		RegCloseKey(hKey);
		hKey = NULL;
	}

	return TRUE;
}





//枚举删除包中的补丁信息
BOOL EnumPackageInternal(HKEY hPackagesKey,HKEY hPackageKey,CString& strUninstallString)
{
	HKEY hOwnKey = NULL;
	if(RegOpenKeyEx(hPackageKey, L"Owners", 0, KEY_READ, &hOwnKey)== ERROR_SUCCESS)
	{
		TCHAR szName[MAX_PATH] = {0};
		DWORD dwSize = MAX_PATH;
		ULONG nChars = 0x1000;
		for (int i = 0;RegEnumValue(hOwnKey, i, szName, &dwSize, NULL, NULL, NULL, NULL)== ERROR_SUCCESS;i++, nChars = 0x1000)
		{
			CString strPackageKeyName = szName;
			szName[0] = _T('\0');
			if (!strPackageKeyName.IsEmpty())
			{
				//获取父节点注册表信息
				HKEY hParentKey;
				if (RegOpenKeyEx(hPackagesKey, strPackageKeyName, 0, KEY_READ, &hParentKey) == ERROR_SUCCESS)
				{
					HKEY hParentOwnKey = NULL;
					if(RegOpenKeyEx(hParentKey, L"Owners", 0, KEY_QUERY_VALUE, &hParentOwnKey)== ERROR_SUCCESS)
					{
						DWORD dwOwnType,cbOwnData = sizeof(DWORD),TmpState;
						if (RegQueryValueEx(hParentOwnKey,strPackageKeyName,NULL,&dwOwnType,(LPBYTE)&TmpState,&cbOwnData) == ERROR_SUCCESS)
						{
							if (dwOwnType == REG_DWORD)
							{
								RegCloseKey(hParentOwnKey);
								RegCloseKey(hParentKey);
								RegCloseKey(hOwnKey);

								strUninstallString.Format(L"Dism.exe /Online /Remove-Package /PackageName:%s",strPackageKeyName);
								return TRUE;
							}
						}
						RegCloseKey(hParentOwnKey);
					}
					RegCloseKey(hParentKey);
				}					
			}
		}
		RegCloseKey(hOwnKey);
	}
	return FALSE;
}

//枚举包内补丁信息
BOOL EnumFromPackages(map<CString, INSTALLVUL> &mapInstVul)
{
	HKEY hKey = NULL;
	WCHAR szValue[0x1000] = {0};
	DWORD dwSize = sizeof(DWORD);

	REGSAM samDesired = KEY_ENUMERATE_SUB_KEYS;
	if (g_SomGlobal_bWin64)
		samDesired |= KEY_WOW64_64KEY;

	BOOL bIsWin10AndLater = EntIsWin10AndLater();
	vector<CString> arrPackagesKeyPath;
	arrPackagesKeyPath.push_back(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Component Based Servicing\\Packages");
	if(bIsWin10AndLater)
	{
		arrPackagesKeyPath.push_back(L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Component Based Servicing\\PackagesPending");
	}
	for (vector<CString>::iterator it = arrPackagesKeyPath.begin(); it != arrPackagesKeyPath.end(); it++)
	{
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, *it, 0, samDesired, &hKey) == ERROR_SUCCESS)
		{
			ULONG nChars = 0x1000;
			for (int i = 0;RegEnumKeyEx(hKey, i, szValue, &nChars, NULL, NULL, NULL, NULL)== ERROR_SUCCESS;i++, nChars = 0x1000)
			{
				CString strKeyName = szValue;
				DWORD dwKey = 0;
				CString strKBID = L"";
				LPCWSTR pszKBCode = StrStrI(szValue, L"Package_for_KB");
				if (NULL == pszKBCode)
				{
					if (bIsWin10AndLater)
					{
						pszKBCode = StrStrI(szValue, L"_for_KB");
						if (pszKBCode == NULL)
							continue;
						pszKBCode += 7;
						LPCWSTR pszEnd = StrStrI(pszKBCode, L"~");
						if (pszEnd - pszKBCode <= 6)
							continue;

						dwKey = FormatKBID(pszKBCode);
						strKBID.Format(L"KB%d", dwKey);
					}
				}
				else
				{
					pszKBCode += 14;
					LPCWSTR pszEnd = StrStrI(pszKBCode, L"~");
					if (pszEnd - pszKBCode <= 6)
						continue;

					WCHAR szTemp = *pszEnd;
					*(LPWSTR)pszEnd = 0;
					dwKey = FormatKBID(pszKBCode);
					strKBID.Format(L"KB%d", dwKey);
					*(LPWSTR)pszEnd = szTemp;
				}

				if (bIsWin10AndLater)
				{
					HKEY hKey2 = NULL;
					if (RegOpenKeyEx(hKey, szValue, 0, KEY_ENUMERATE_SUB_KEYS, &hKey2) == ERROR_SUCCESS)
					{
						CString strUninstallString;
						if (EnumPackageInternal(hKey,hKey2,strUninstallString))
						{
							if (!strUninstallString.IsEmpty())
							{
								INSTALLVUL VulInfo;
								VulInfo.dwKey = dwKey;
								VulInfo.strDisplayName = strKBID;
								VulInfo.strUninstallString = strUninstallString;
								CString strIndexKey;
								strIndexKey.Format(L"%d+%s",dwKey,VulInfo.strUninstallString);
								mapInstVul[strIndexKey] = VulInfo;
							}
						}
						RegCloseKey(hKey2);
					}
				}
				else
				{
					HKEY hKey2 = NULL;
					CString strInstallDate;
					if (RegOpenKeyEx(hKey, szValue, 0, KEY_QUERY_VALUE, &hKey2) == ERROR_SUCCESS)
					{
						// 已安装状态的判断改为对CurrentState键值的判断
						// 在vista，CurrentState&0x0F=0x06、0x07、0x08表示已安装
						// 在win7及以上平台CurrentState&0xF0=0x60、0x70、0x80表示已安装
						FILETIME ft = {0 };
						DWORD dwCurrentState = 0;
						DWORD dwType1, dwType2, dwType3, cbData1 = sizeof(DWORD), cbData2 = sizeof(DWORD), cbData3 = sizeof(DWORD);
						if (RegQueryValueEx(hKey2, L"InstallTimeLow", NULL, &dwType1, (LPBYTE)&ft.dwLowDateTime, &cbData1)== ERROR_SUCCESS &&
							RegQueryValueEx(hKey2, L"InstallTimeHigh", NULL, &dwType2, (LPBYTE)&ft.dwHighDateTime, &cbData2)== ERROR_SUCCESS &&
							RegQueryValueEx(hKey2, L"CurrentState", NULL, &dwType3, (LPBYTE)&dwCurrentState, &cbData3) == ERROR_SUCCESS &&
							dwType1 == REG_DWORD && dwType2 == REG_DWORD && dwType3 == REG_DWORD)
						{
							if ((dwCurrentState & 0xf0) == 0x60 || (dwCurrentState & 0xf0) == 0x70)
							{
								SYSTEMTIME stUTC, stLocal;
								FileTimeToSystemTime(&ft, &stUTC);
								SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
								strInstallDate.Format(L"%d-%02d-%02d", stLocal.wYear, stLocal.wMonth, stLocal.wDay);

								INSTALLVUL VulInfo;
								WCHAR szInstallName1[MAX_PATH] = {0};
								DWORD dwSize11 = MAX_PATH*sizeof(WCHAR);

								if (RegQueryValueEx(hKey2, L"InstallName", 0,0, (LPBYTE)szInstallName1, &dwSize11)== ERROR_SUCCESS)
								{
									VulInfo.dwKey = dwKey;
									VulInfo.strDisplayName = strKBID;
									VulInfo.strUninstallString.Format(L"wusa.exe /uninstall /kb:%d", dwKey);

									CString strIndexKey;
									strIndexKey.Format(L"%d+%s",dwKey,VulInfo.strUninstallString);
									mapInstVul[strIndexKey] = VulInfo;
								}
							}
						}

						RegCloseKey(hKey2);
					}
				}
			}

			RegCloseKey(hKey);
		}
	}

	return TRUE;
}

//枚举所有补丁信息
BOOL EnumAllInstallVul()
{
	m_mapInstVul.clear();

	EnumFromInstaller(m_mapInstVul);

	EnumFromRegUninstall(m_mapInstVul);

	if (EntIsWin7AndLater()) //Vista不支持wusa.exe卸载补丁
	{
		EnumFromPackages(m_mapInstVul);
	}

	return TRUE;
}

//补丁是否安装了
BOOL IsVulInstall(DWORD dwKey)
{
	CString strKBID;
	strKBID.Format(L"%d",dwKey);
	for (map<CString, INSTALLVUL>::iterator itTemp = m_mapInstVul.begin(); itTemp != m_mapInstVul.end(); itTemp++)
	{
		CString strIndexKey = itTemp->first;
		if (strIndexKey.Find(strKBID) == 0)
			return TRUE;
	}
	return FALSE;
}