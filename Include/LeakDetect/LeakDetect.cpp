#include "LeakDetect.h"


CString FormatInstallDate(LPCWSTR lpValue)
{
	int len = wcslen(lpValue);
	CString strDate;
	if (len < 8)
		return strDate;

	LPWSTR pDate = strDate.GetBuffer(10);
	LPCWSTR pDate2 = lpValue;
	if (pDate2[4] == L'-')
	{
		memcpy(pDate, pDate2, 8);
		pDate2 += 5;
		if (pDate2[1] == L'-')
		{
			pDate[5] = L'0';
			pDate[6] = *pDate2++;
		}
		else
		{
			pDate[5] = *pDate2++;
			pDate[6] = *pDate2++;
		}
		pDate2++;
		if (pDate2[1] == 0)
		{
			pDate[8] = L'0';
			pDate[9] = *pDate2++;
		}
		else
		{
			pDate[8] = *pDate2++;
			pDate[9] = *pDate2++;
		}
	}
	else if (pDate2[1] == L'/' || pDate2[2] == L'/')
	{
		if (pDate2[1] == L'/')
		{
			pDate[5] = L'0';
			pDate[6] = *pDate2++;
		}
		else
		{
			pDate[5] = *pDate2++;
			pDate[6] = *pDate2++;
		}
		pDate2++;
		if (pDate2[1] == L'/')
		{
			pDate[8] = L'0';
			pDate[9] = *pDate2++;
		}
		else
		{
			pDate[8] = *pDate2++;
			pDate[9] = *pDate2++;
		}
		memcpy(pDate, pDate2 + 1, 8);
	}
	else if (len == 8)
	{
		memcpy(pDate, pDate2, 8);
		memcpy(pDate + 5, pDate2 + 4, 4);
		memcpy(pDate + 8, pDate2 + 6, 4);
	}

	pDate[4] = L'-';
	pDate[7] = L'-';
	strDate.ReleaseBuffer(10);
	return strDate;
}


// �ѿ��ܺ��в����ŵ��ַ�����ʽ����ΨһKEY �ĸ������������û�в����ţ����� 0
DWORD FormatKBID(LPCWSTR lpKBID, LPCWSTR* lppKBIDFirst = NULL, LPCWSTR* lppKBIDLast = NULL, BOOL bRequireKBPrefix = FALSE)
{
	if (lpKBID == NULL || lpKBID[0] == 0)
		return 0;

	LPCWSTR lpKBHeader = NULL, lpKBTail = NULL;

	// ��� bRequireKBPrefix Ϊ TRUE ���߲������ֿ�ͷ����ô������û�� KB����QH ��ͷ�����֣�����У���Ϊ������־��� KB ��
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
			// �ж���û��С�汾��(��׼���� -v _V ��ʼ����Ϊ��׼)
			if ((*p == L'-' || *p == L'_') && ((*(p + 1) == L'v' || *(p + 1) == L'V')))
				lpVersion = p + 2;

			break;
		}
		else
		{
			kbid = (kbid * 10) + (*p - L'0');
			lpKBTail = p + 1;
		}
	}

	// �Ϸ��� KBID ���� 6 λʮ�������֣����ֵ���ܳ��� 0xFFFFFF
	if (kbid < 100000 || kbid > 0xFFFFFF)
		return 0;

	if (lpVersion)
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
				lpKBTail = p + 1;
			}
		}
		kbid |= ((version & 0x7F) << 24);
	}

	// ǿ�������λΪ0
	kbid &= 0x7FFFFFFF;

	if (lppKBIDFirst)
		*lppKBIDFirst = lpKBHeader;
	if (lppKBIDLast)
		*lppKBIDLast = lpKBTail;

	return kbid;
}

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

BOOL LoadInstalledXp(HKEY hKey, LPCWSTR lpValue, vector<INSTALLEDITEM>& InstalledVector, REGSAM sam)
{
	LPCWSTR pszStart, pszEnd;
	DWORD dwKey = FormatKBID(lpValue, &pszStart, &pszEnd, TRUE);

	if (dwKey == 0)
		return FALSE;

	HKEY hKey2 = NULL;
	if (RegOpenKeyEx(hKey, lpValue, 0, KEY_QUERY_VALUE | sam, &hKey2) != ERROR_SUCCESS)
		return FALSE;

	CString strKBID;
	WCHAR szTemp = *pszEnd;
	*(LPWSTR)pszEnd = 0;
	strKBID.Format(L"KB%s", pszStart);
	*(LPWSTR)pszEnd = szTemp;

	WCHAR szValue[0x1000];
	DWORD dwType, cbData = sizeof(szValue);
	CString strDescription, strInstalledDate;
	if (RegQueryValueEx(hKey2, L"Description", NULL, &dwType, (LPBYTE)szValue, &cbData) == ERROR_SUCCESS)
		strDescription = szValue;

	cbData = sizeof(szValue);
	if (RegQueryValueEx(hKey2, L"InstalledDate", NULL, &dwType, (LPBYTE)szValue, &cbData) == ERROR_SUCCESS)
		strInstalledDate = FormatInstallDate(szValue);

	INSTALLEDITEM item;
	item.dwKey = dwKey;
	item.strKBID = strKBID;
	item.strName = strDescription;
	item.strDate = strInstalledDate;

	InstalledVector.push_back(item);

	RegCloseKey(hKey2);

	return TRUE;
}

void LoadInstalled(vector<INSTALLEDITEM>& InstalledVector)
{
	HKEY hKey = NULL;
	WCHAR szValue[0x1000];
	vector<DWORD> vecInstPatches;
	REGSAM sam = (g_SomGlobal_bWin64 ? KEY_WOW64_64KEY : 0);
	CSomSystemEnvironment* pEnv = CSomSystemEnvironment::GetSystemEnvironment();
	if (pEnv->m_nWindowMajorVer == 0x5)
	{
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Updates", 0, KEY_ENUMERATE_SUB_KEYS | sam, &hKey) == ERROR_SUCCESS)
		{
			ULONG nChars = 0x1000;
			for (int i = 0; RegEnumKeyEx(hKey, i, szValue, &nChars, NULL, NULL, NULL, NULL) == ERROR_SUCCESS; i++, nChars = 0x1000)
			{
				//���ĳЩ���´����Ӹ���ʱ����ʾBUG������continue
				LoadInstalledXp(hKey, szValue, InstalledVector, sam);

				HKEY hKey2 = NULL;
				if (RegOpenKeyEx(hKey, szValue, 0, KEY_ENUMERATE_SUB_KEYS | sam, &hKey2) == ERROR_SUCCESS)
				{
					nChars = 0x1000;
					for (int j = 0; RegEnumKeyEx(hKey2, j, szValue, &nChars, NULL, NULL, NULL, NULL) == ERROR_SUCCESS; j++, nChars = 0x1000)
					{
						if (LoadInstalledXp(hKey2, szValue, InstalledVector, sam))
							continue;

						HKEY hKey3 = NULL;
						if (RegOpenKeyEx(hKey2, szValue, 0, KEY_ENUMERATE_SUB_KEYS | sam, &hKey3) == ERROR_SUCCESS)
						{
							nChars = 0x1000;
							for (int k = 0; RegEnumKeyEx(hKey3, k, szValue, &nChars, NULL, NULL, NULL, NULL) == ERROR_SUCCESS; k++, nChars = 0x1000)
							{
								if (LoadInstalledXp(hKey3, szValue, InstalledVector, sam))
									continue;
							}
							RegCloseKey(hKey3);
						}
					}
					RegCloseKey(hKey2);
				}
			}
			RegCloseKey(hKey);
		}
	}
	else
	{
		if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Component Based Servicing\\Packages", 0, KEY_ENUMERATE_SUB_KEYS | sam, &hKey) == ERROR_SUCCESS)
		{
			ULONG nChars = 0x1000;
			for (int i = 0; RegEnumKeyEx(hKey, i, szValue, &nChars, NULL, NULL, NULL, NULL) == ERROR_SUCCESS; i++, nChars = 0x1000)
			{
				LPCWSTR pszKBCode = StrStrI(szValue, L"Package_");
				if (NULL == pszKBCode)
					continue;

				pszKBCode += 8;
				pszKBCode = StrStrI(pszKBCode, L"for_KB");
				if (NULL == pszKBCode)
					continue;

				pszKBCode += 6;

				LPCWSTR pszStart, pszEnd;
				DWORD dwKey = FormatKBID(pszKBCode, &pszStart, &pszEnd);

				if (dwKey == 0)
					continue;

				// 					ATLASSERT(pszStart == pszKBCode && (*pszEnd == L'~'));

				CString strKBID;
				WCHAR szTemp = *pszEnd;
				*(LPWSTR)pszEnd = 0;
				strKBID.Format(L"KB%s", pszKBCode);
				*(LPWSTR)pszEnd = szTemp;

				HKEY hKey2 = NULL;

				CString strInstallDate;
				if (RegOpenKeyEx(hKey, szValue, 0, KEY_QUERY_VALUE, &hKey2) == ERROR_SUCCESS)
				{
					// �Ѱ�װ״̬���жϸ�Ϊ��CurrentState��ֵ���ж�
					// ��vista��CurrentState&0x0F=0x06��0x07��0x08��ʾ�Ѱ�װ
					// ��win7������ƽ̨CurrentState&0xF0=0x60��0x70��0x80��ʾ�Ѱ�װ
					FILETIME ft = { 0 };
					DWORD dwCurrentState = 0;
					DWORD dwType1, dwType2, dwType3, cbData1 = sizeof(DWORD), cbData2 = sizeof(DWORD), cbData3 = sizeof(DWORD);
					if (RegQueryValueEx(hKey2, L"InstallTimeLow", NULL, &dwType1, (LPBYTE)&ft.dwLowDateTime, &cbData1) == ERROR_SUCCESS &&
						RegQueryValueEx(hKey2, L"InstallTimeHigh", NULL, &dwType2, (LPBYTE)&ft.dwHighDateTime, &cbData2) == ERROR_SUCCESS &&
						RegQueryValueEx(hKey2, L"CurrentState", NULL, &dwType3, (LPBYTE)&dwCurrentState, &cbData3) == ERROR_SUCCESS &&
						dwType1 == REG_DWORD && dwType2 == REG_DWORD && dwType3 == REG_DWORD)
					{
						BOOL bRealInstalled = FALSE;
						if (CSomSystemEnvironment::WIN_VISTA == pEnv->m_nEnvWindowVer)
						{
							if ((dwCurrentState & 0x0f) == 0x06 || (dwCurrentState & 0x0f) == 0x07 || (dwCurrentState & 0x0f) == 0x08)
							{
								bRealInstalled = TRUE;
							}
						}
						else if ((dwCurrentState & 0xf0) == 0x60 || (dwCurrentState & 0xf0) == 0x70 || (dwCurrentState & 0xf0) == 0x80)
						{
							bRealInstalled = TRUE;
						}

						if (bRealInstalled)
						{
							SYSTEMTIME stUTC, stLocal;
							FileTimeToSystemTime(&ft, &stUTC);
							SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);
							strInstallDate.Format(L"%d-%02d-%02d", stLocal.wYear, stLocal.wMonth, stLocal.wDay);

							INSTALLEDITEM item;

							item.dwKey = dwKey;
							item.strKBID = strKBID;
							item.strDate = strInstallDate;

							InstalledVector.push_back(item);
							vecInstPatches.push_back(dwKey);
						}
					}

					RegCloseKey(hKey2);
				}
			}
			RegCloseKey(hKey);
		}
	}
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Installer\\UserData\\S-1-5-18\\Products", 0, KEY_ENUMERATE_SUB_KEYS | sam, &hKey) == ERROR_SUCCESS)
	{
		ULONG nChars = 0x1000;
		for (int i = 0; RegEnumKeyEx(hKey, i, szValue, &nChars, NULL, NULL, NULL, NULL) == ERROR_SUCCESS; i++, nChars = 0x1000)
		{
			HKEY hKey2 = NULL;
			StringCbCat(szValue, sizeof(szValue), L"\\Patches");
			if (RegOpenKeyEx(hKey, szValue, 0, KEY_ENUMERATE_SUB_KEYS | sam, &hKey2) == ERROR_SUCCESS)
			{
				nChars = 0x1000;
				for (int j = 0; RegEnumKeyEx(hKey2, j, szValue, &nChars, NULL, NULL, NULL, NULL) == ERROR_SUCCESS; j++, nChars = 0x1000)
				{
					HKEY hKey3 = NULL;
					if (RegOpenKeyEx(hKey2, szValue, 0, KEY_QUERY_VALUE | sam, &hKey3) == ERROR_SUCCESS)
					{
						DWORD dwType, cbData = sizeof(szValue);
						if (RegQueryValueEx(hKey3, L"DisplayName", NULL, &dwType, (LPBYTE)szValue, &cbData) == ERROR_SUCCESS)
						{
							CString strDisplayName = szValue;
							LPCWSTR pszStart, pszEnd;
							DWORD dwKey = FormatKBID(szValue, &pszStart, &pszEnd, TRUE);
							if (dwKey != 0)
							{
								//									ATLASSERT(*pszEnd == L')');

								CString strKBID;
								WCHAR szTemp = *pszEnd;
								*(LPWSTR)pszEnd = 0;
								strKBID.Format(L"KB%s", pszStart);
								*(LPWSTR)pszEnd = szTemp;

								INSTALLEDITEM item;
								item.dwKey = dwKey;
								item.strKBID = strKBID;
								item.strName = szValue;

								cbData = sizeof(szValue);
								if (RegQueryValueEx(hKey3, L"Installed", NULL, &dwType, (LPBYTE)szValue, &cbData) == ERROR_SUCCESS)
									item.strDate = FormatInstallDate(szValue);

								InstalledVector.push_back(item);
								vecInstPatches.push_back(dwKey);
							}
							else
							{
								DWORD dwType, cbData = sizeof(szValue);
								if (RegQueryValueEx(hKey3, L"MoreInfoURL", NULL, &dwType, (LPBYTE)szValue, &cbData) == ERROR_SUCCESS)
								{
									DWORD dwKey = FindKBIDNoKB(szValue);
									//SomOutputDebugString(L"MoreInfoURL (KB:%d): %s", dwKey, szValue);
									if (dwKey != 0)
									{
										CString strKBID;
										strKBID.Format(L"KB%d", dwKey);

										INSTALLEDITEM item;
										item.dwKey = dwKey;
										item.strKBID = strKBID;
										item.strName = strDisplayName;

										cbData = sizeof(szValue);
										if (RegQueryValueEx(hKey3, L"Installed", NULL, &dwType, (LPBYTE)szValue, &cbData) == ERROR_SUCCESS)
											item.strDate = FormatInstallDate(szValue);

										InstalledVector.push_back(item);
										vecInstPatches.push_back(dwKey);
									}
								}
							}
						}
						RegCloseKey(hKey3);
					}
				}
				RegCloseKey(hKey2);
			}
		}
		RegCloseKey(hKey);
	}

	CCorrectInstalledPatches _inst;
	std::map<DWORD, CString> wmiResult;
	if (_inst.GetInstalledPatches(wmiResult))
	{
		for (std::map<DWORD, CString>::iterator itor = wmiResult.begin(); itor != wmiResult.end(); itor++)
		{
			if (std::find(vecInstPatches.begin(), vecInstPatches.end(), itor->first) == vecInstPatches.end())
			{
				INSTALLEDITEM item;

				item.dwKey = itor->first;
				item.strKBID.Format(L"KB%d", itor->first);
				item.strDate = itor->second;

				InstalledVector.push_back(item);
				//WRITE_LOG(L"�����Ѱ�װ�б����Ӳ���KB%d", item.dwKey);
			}
		}
	}
	else
	{
		WRITE_LOG(L"����wmi�ӿ�ʧ��");
	}
}

BOOL IsPatchInstalled(LPCTSTR lpKBNumber)
{
	//vecInstalled �����������յĲ�������
	CString buffer = lpKBNumber;

	if (!buffer.IsEmpty())
	{
		LoadInstalled(vecInstalled);

		//�ж��β��еĲ������Ƿ����
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

	//�ж��Ƿ���KB��ͷ
	if ((str[0] == 'K' || str[0] == 'k') && (str[1] == 'B' || str[1] == 'b'))
	{
		CString strTemp;

		for (int i = 2; i < str.GetLength(); i++)
		{
			if (str[i] >= '0' && str[i] <= '9')
				strTemp += str[i];
			else
				return -1;
		}
		//��ȡȥ��KB��Ĳ�����
		return _ttoi(strTemp);
	}
	else
	{
		CString strTemp;
		for (int i = 0; i < str.GetLength(); i++)
		{
			if (str[i] >= '0' && str[i] <= '9')
				strTemp += str[i];
			else
				return -1;
		}
		//��ȡȥ��KB��Ĳ�����
		return _ttoi(strTemp);
	}
}

//�����Ƿ�װ��
BOOL IsVulInstall(DWORD dwKey)
{
	DWORD dwbuffer = dwKey;
	CString strKBID;
	strKBID.Format(L"KB%d",dwbuffer);
	for (vector<INSTALLEDITEM>::iterator itTemp = vecInstalled.begin(); itTemp != vecInstalled.end(); itTemp++)
	{
		CString strIndexKey = itTemp->strKBID;

		if (strIndexKey == strKBID)
			return TRUE;
	}
	return FALSE;
}
