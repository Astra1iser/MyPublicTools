#pragma once
#include <list>

class DirectoryOperation
{
public:
	// �ж�Ŀ¼�Ƿ����
	static BOOL FolderExists(CString strDir);
	// ����Ŀ¼
	static BOOL CreateDirRecursive(CString strDir);
	// TopisDir����˼�Ǳ�ʾ strDir������һ��Ŀ¼������һ���ļ�����������ļ�������ֻ����������һ��
	static BOOL CreateDirRecursive(CString strDir, BOOL TopisDir);
	//get size of folder
	static DWORD64 GetFolderSize(CString path, int &n_File);
	//Ŀ¼�ԡ�\����β
	static CString& EnsurePathEndWithSuffix(CString& strPath, WCHAR cSuffix = L'\\')
	{
		if (!strPath.IsEmpty() && strPath[strPath.GetLength() - 1] != cSuffix)
			strPath += cSuffix;
		return strPath;
	}
	//Ŀ¼���ԡ�\����β
	static CString& EnsurePathEndWithoutSuffix(CString& strPath, WCHAR cSuffix = L'\\')
	{
		if (!strPath.IsEmpty() && strPath[strPath.GetLength() - 1] == cSuffix)
			strPath = strPath.Left(strPath.GetLength() - 1);
		return strPath;
	}
	static BOOL DeleteDirRecursive(CString strDir);
	// ��ȫ�����ļ���ֱ��CopyFile�����fileid�仯�������޷����ұ���
	static BOOL EntSafeCopyFile(LPCTSTR src, LPCTSTR dst);
	//���Ŀ���ļ���ռ�ã����ǹ���ɾ����ʽ�򿪣���һ���㶨(ĿǰMyFile�ǹ���ɾ���򿪵�)
	//���Ŀ���ļ���ռ�ã������ǹ���ɾ������ֻ�ܵȴ��ˡ�����
	static BOOL SafeMoveFile(CString src, CString dst, DWORD max_wait_second);
	static BOOL GetFileVer(const CString& rFilePath, CString& rVer);
	static BOOL GetModuleFilePath(CString& strSubPath, CString& strOutFile);
	static void DeleteAllFileInDir(const CString &strFolder);
	static BOOL CopyAllFileInDir(const CString strScrFolder,const CString strDestFolder);
	static BOOL CopyOneFile(const CString strScrFolder,const CString strDestFolder);
	static BOOL MakeCab(CString strCabFile, CString strFolder, std::list<CString> fileList);//strFolder��'\'��β
	static BOOL MakeCab(CString strCabFile, CString strFolder);//strFolder��'\'��β,���һ���ļ����µ������ļ�
	static BOOL ExtractCab(CString strCabFile, CString strFolder);
	static CString GetTempFilePath();
	static BOOL RenameFile(const CString strSrc, const CString StrDes);
	static BOOL VersionStringToLargeInteger(LPCWSTR lpVersion, LARGE_INTEGER& liVersion)
	{
		if (lpVersion == NULL || lpVersion[0] == 0)
		{
			liVersion.QuadPart = 0;
			return FALSE;
		}

		DWORD wHMS = 0, wHLS = 0, wLMS = 0, wLLS = 0;

		CString strVer = lpVersion;
		if(-1 != strVer.Find(','))
			swscanf_s(lpVersion , L"%u,%u,%u,%u", &wHMS, &wHLS, &wLMS, &wLLS);
		else
			swscanf_s(lpVersion , L"%u.%u.%u.%u", &wHMS, &wHLS, &wLMS, &wLLS);

		liVersion.HighPart = MAKELONG(wHLS, wHMS);
		liVersion.LowPart = MAKELONG(wLLS, wLMS);

		return TRUE;
	}
	static CString GetPrivateCloudBDVersion(CString strPath, time_t& time)
	{
		CString strFolder = strPath;
		DWORD dwCount = 0;
		CString strFind = strFolder + _T("\\versions.id.*");

		CString strVDBVer = _T("");

		WIN32_FIND_DATAW FindFileData;
		HANDLE hFind=::FindFirstFileW(strFind, &FindFileData);

		if(INVALID_HANDLE_VALUE == hFind)    
			return strVDBVer;	

		while(TRUE)
		{
			if (!(FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
			{
				CString strFilePath = strFolder + FindFileData.cFileName;
				HANDLE hFile = CreateFile(strFilePath, 
					GENERIC_READ, 
					FILE_SHARE_READ|FILE_SHARE_WRITE|FILE_SHARE_DELETE, 
					NULL, 
					OPEN_EXISTING,
					FILE_ATTRIBUTE_NORMAL, 
					NULL);
				if (hFile == INVALID_HANDLE_VALUE)
					return _T("");

				DWORD dwSize = GetFileSize(hFile, NULL);
				CHAR* pData = new CHAR[dwSize + 1];
				if (pData != NULL)
					ZeroMemory(pData, dwSize + 1);

				DWORD	dwReaded = 0;
				ReadFile(hFile, pData, dwSize, &dwReaded, NULL);		
				CloseHandle(hFile);

				//CString str = CA2W(pData);
				CString str = pData;
				int nBegin = str.Find(_T("<update>"));
				int nEnd = str.Find(_T("</update>"));
				int nLen = wcslen(_T("<update>"));
				nBegin = nBegin + nLen;

				strVDBVer = str.Mid(nBegin, nEnd - nBegin);
				break;
			}

			if(!FindNextFileW(hFind, &FindFileData))    
				break;
		}

		FindClose(hFind);
		strVDBVer.TrimLeft();
		strVDBVer.TrimRight();

		UINT nPrivateCloudVer = _ttoi(strVDBVer);

		TIME_ZONE_INFORMATION tzi = {0};
		GetTimeZoneInformation(&tzi);		

		time = nPrivateCloudVer; 

		SYSTEMTIME stTime;
		TimetToSystemTime(nPrivateCloudVer, &stTime);

		strVDBVer.Format(_T("%4d-%02d-%02d"), stTime.wYear,stTime.wMonth,stTime.wDay);
		return strVDBVer;
	}
	static void TimetToSystemTime(time_t t, LPSYSTEMTIME pst)
	{
		FILETIME ft;
		LONGLONG ll = Int32x32To64(t, 10000000) + 116444736000000000;
		ft.dwLowDateTime = (DWORD) ll;
		ft.dwHighDateTime = (DWORD)(ll >> 32);

		FileTimeToSystemTime( &ft, pst );
	}
};