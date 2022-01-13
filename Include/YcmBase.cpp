//QTBase�������ʵ��
#pragma once
#include <YcmBase.h>
#include <Pathmanager.cpp>
#include <RegeditManager.cpp>
#include <MyOperFile.cpp>
#include <DiretoryOperation.cpp>
#include <Thread.cpp>
#include <Mutex.cpp>
#include <FIFO.cpp>
#include <SharedMemory.cpp>
#include <IniManager.cpp>
#include <XmlManager.cpp>
#include <LogWriter.cpp>
#include <Socket.cpp>
#include <HttpPost.cpp>
#include <httpdown.cpp>
#include <WinFirewallControler.cpp>
#include <SvcHelper.cpp>
#include <SystemternlHelper.cpp>
#include <MakeCab.cpp>
#include <zip.cpp>
#include <unzip.cpp>


BOOL Base::IsAdmin() {
	#define ACCESS_READ		1  
	#define ACCESS_WRITE	2  
	HANDLE		hToken;
	DWORD		dwStatus;
	DWORD		dwAccessMask;
	DWORD		dwAccessDesired;
	DWORD		dwACLSize;
	DWORD		dwStructureSize = sizeof(PRIVILEGE_SET);
	PACL			pACL = NULL;
	PSID				psidAdmin = NULL;
	BOOL			bReturn = FALSE;

	PRIVILEGE_SET				ps;
	GENERIC_MAPPING		GenericMapping;

	PSECURITY_DESCRIPTOR			psdAdmin = NULL;
	SID_IDENTIFIER_AUTHORITY	SystemSidAuthority = SECURITY_NT_AUTHORITY;

	__try
	{
		BOOL Init = ImpersonateSelf(SecurityImpersonation);

		if (!OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, FALSE, &hToken))
		{
			if (GetLastError() != ERROR_NO_TOKEN)
				__leave;
			if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY,
				&hToken))
				__leave;
		}

		if (!AllocateAndInitializeSid(&SystemSidAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &psidAdmin))
			__leave;

		psdAdmin = LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);

		if (psdAdmin == NULL)
			__leave;

		if (!InitializeSecurityDescriptor(psdAdmin,
			SECURITY_DESCRIPTOR_REVISION))
			__leave;

		dwACLSize = sizeof(ACL) + sizeof(ACCESS_ALLOWED_ACE) +
			GetLengthSid(psidAdmin) - sizeof(DWORD);

		pACL = (PACL)LocalAlloc(LPTR, dwACLSize);
		if (pACL == NULL)
			__leave;

		if (!InitializeAcl(pACL, dwACLSize, ACL_REVISION2))
			__leave;

		dwAccessMask = ACCESS_READ | ACCESS_WRITE;

		if (!AddAccessAllowedAce(pACL, ACL_REVISION2,
			dwAccessMask, psidAdmin))
			__leave;

		if (!SetSecurityDescriptorDacl(psdAdmin, TRUE, pACL, FALSE))
			__leave;

		SetSecurityDescriptorGroup(psdAdmin, psidAdmin, FALSE);
		SetSecurityDescriptorOwner(psdAdmin, psidAdmin, FALSE);

		if (!IsValidSecurityDescriptor(psdAdmin))
			__leave;

		dwAccessDesired = ACCESS_READ;
		GenericMapping.GenericRead = ACCESS_READ;
		GenericMapping.GenericWrite = ACCESS_WRITE;
		GenericMapping.GenericExecute = 0;
		GenericMapping.GenericAll = ACCESS_READ | ACCESS_WRITE;

		if (!AccessCheck(psdAdmin, hToken, dwAccessDesired, &GenericMapping, &ps, &dwStructureSize, &dwStatus, &bReturn))
		{
			__leave;
		}

		RevertToSelf();

	}
	__finally {

		if (pACL)		LocalFree(pACL);
		if (psdAdmin)	LocalFree(psdAdmin);
		if (psidAdmin)	FreeSid(psidAdmin);
	}

	return   bReturn;
}


CString Base::GetLangaueSyncOS()
{
	CString strLanguage = L"zh_cn";

	LANGID id = GetSystemDefaultLangID();
	switch(id)
	{
	case 0x0804:	//��������
	case 0x0404:	//̨��
	case 0x0c04:	//���
		strLanguage = L"zh_cn";
		//WRITE_LOG(L"��ʾ���֣���������");
		break;
	case 0x0409:
		strLanguage = L"en_us";
		//WRITE_LOG(L"��ʾ���֣�Ӣ��");
		break;
	default:
		strLanguage = L"en_us";
		//WRITE_LOG(L"δ֪�����Ի�������ʾ���֣�Ӣ��");
	}

	if (!strLanguage.IsEmpty())
	{
		//REG_UTIL::setReg(kModuleBase, kLanguageServerName, strLanguage.GetString());
		return strLanguage;
	}

	return L"";

}


//��ǰ��ϵͳ�Ƿ���vista֮��İ汾
//bool is_vista_or_later()
//{
//	OSVERSIONINFOEXW osvi = { sizeof(osvi) };
//
//	if (!GetVersionExW((LPOSVERSIONINFOW)&osvi))
//	{
//		return true;
//	}
//
//	if (osvi.dwMajorVersion >= 6)
//	{
//		return true;
//	}
//
//	return false;
//}


BOOL Base::StartPrograme(CString Path, _Out_opt_ HANDLE& ProgrameHandle, CString Parameters, BOOL IsAdmin, BOOL IsWaitForSingle)
{

	//if (!PathIsDirectory(Path))
	//	return FALSE;


	SHELLEXECUTEINFO ShExecInfo = { 0 };

	//����ǽṹ���С��sizeof��SHELLEXECUTEINFO����
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);

	//һ����־���飬��������������Ա����Ч��
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;

	//�������ShellExecuteEx�Ĵ��ھ��
	ShExecInfo.hwnd = NULL;

	//���ó�runas�Ϳ����Թ���Ա��������ˣ������Ķ�����ͨ�û����
	//ָ��ִ�еĶ�����������edit ��explore ��find ��open��print�� properties
	if (IsAdmin == TRUE)
	{
		ShExecInfo.lpVerb = L"runas";
	}
	else
		ShExecInfo.lpVerb = L"open";


	//ָ�򸸴��ڵľ��
	ShExecInfo.hwnd = NULL;
	//Ҫ���е��ļ�·��
	//ShExecInfo.lpFile = L"cmd";
	ShExecInfo.lpFile = Path;

	//����/�򿪳���Ĳ���������򿪵���һ���ĵ����������Ч
	ShExecInfo.lpParameters = Parameters;

	//ָ������Ŀ¼�����֣���Աû��˵������Ĭ��Ϊ��ǰĿ¼
	ShExecInfo.lpDirectory = NULL;

	//���ô�����ʾ(SW_SHOW)�Ͳ���ʾ(SW_HIDE)����Ȼ����������,����Ĭ��Ϊ����ʾ(һ����cmd����)
	ShExecInfo.nShow = SW_SHOW;
	ShExecInfo.nShow = SW_HIDE;

	//����������гɹ��������ֵ������32������������ֵ�Լ���
	ShExecInfo.hInstApp = NULL;

	if (ShellExecuteEx(&ShExecInfo))
	{
		//�̹߳���,ֱ������Ľ�������Ӧ ��:�ر�
		if (IsWaitForSingle == TRUE)
			WaitForSingleObject(ShExecInfo.hProcess, INFINITE);

		//Ϊʲôע�͵����,�������lambda���ʽ���Գ����һ��ʹ��pid�ҽ��̾���ĺ���
		//Ȼ�����һ�����̿����ж�����,���ﻹ�Ƿ��ش�������ʱ�ľ���ȽϺ�
		//DWORD dwId = 0L;
		//dwId = GetProcessId(ShExecInfo.hProcess);

		//[=,&ProgrameHandle](DWORD pid) mutable
		//{
		//	ProgrameHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		//}(dwId);

		ProgrameHandle = ShExecInfo.hProcess;

		return TRUE;
	}	
	else
	{
		ProgrameHandle = NULL;
		return FALSE;
	}
}


void Base::GetIniValue(int& Source, LPCTSTR  Node, LPCTSTR Key, LPCTSTR IniPath)
{
	//int����
	auto temp = GetPrivateProfileInt(Node, Key, NULL, IniPath);
	Source = temp;
}


void Base::GetIniValue(CString& Source, LPCTSTR Node, LPCTSTR Key, LPCTSTR IniPath)
{
	//CString����
	ULONG len = MAX_PATH;
	WCHAR szValue[MAX_PATH] = { 0 };
	GetPrivateProfileString(Node, Key, NULL, szValue, len, IniPath);
	Source = szValue;
}


void Base::SetIniValue(LPCTSTR Node, LPCTSTR Key, LPCTSTR Value, LPCTSTR IniPath)
{
	WCHAR szValue[MAX_PATH] = { 0 };
	WritePrivateProfileString(Node, Key, Value, IniPath);
}


string Base::ReadFileCoding(CString FilePath)
{
	if (!PathFileExists(FilePath))
		return FALSE;

	ifstream fin(FilePath, ios::binary);
	unsigned char  s2;
	fin.read((char*)&s2, sizeof(s2));//��ȡ��һ���ֽڣ�Ȼ������8λ
	int p = s2 << 8;
	fin.read((char*)&s2, sizeof(s2));//��ȡ�ڶ����ֽ�
	p += s2;

	string code;

	switch (p)//�ж��ı�ǰ�����ֽ�,���,���������µı����ټ��������ȥ����
	{
	case 0xfffe:  //65534
		code = "UCS-2 LE BOM"; //Unicode
		break;
	case 0xfeff://65279
		code = "UCS-2 BE BOM"; //Unicode big endian
		break;
	case 0xe6a2://59042
		code = "UTF-8";
		break;
	case 0x5b61://23393
		code = "ANSI OR UTF-8";//��֪��Ϊɶ���ֵ��Ӧ����������
		break;
	case 0xefbb://61371
		code = "UTF-8-BOM";
		break;
	case 0x5b64://23396
		code = "ANSI";
		break;
	default:
		code = "ERROR";
	}
	fin.close();
	return code;
}


wstring Base::UTF8ToUnicode(const char* strSrc)
{
	std::wstring wstrRet;

	if (NULL != strSrc)
	{
		int len = MultiByteToWideChar(CP_UTF8, 0, strSrc, -1, NULL, 0) * sizeof(WCHAR);
		WCHAR* strDst = new(std::nothrow) WCHAR[len + 1];
		if (NULL != strDst)
		{
			MultiByteToWideChar(CP_UTF8, 0, strSrc, -1, strDst, len);
			wstrRet = strDst;;
			delete[]strDst;
		}
	}

	return wstrRet;
}


string Base::UnicodeToAnsi(const WCHAR* strSrc)
{
	std::string strRet;

	if (NULL != strSrc)
	{
		int len = WideCharToMultiByte(CP_ACP, 0, strSrc, -1, NULL, 0, NULL, NULL);
		char* strDst = new(std::nothrow) char[len + 1];
		if (NULL != strDst)
		{
			WideCharToMultiByte(CP_ACP, 0, strSrc, -1, strDst, len, NULL, NULL);
			strRet = strDst;
			delete[]strDst;
		}
	}

	return strRet;
}


string Base::UTF8ToAnsi(const char* strSrc)
{
	return UnicodeToAnsi(UTF8ToUnicode(strSrc).c_str());
}


char* Base::U2G(const char* utf8)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wstr, len);
	len = WideCharToMultiByte(CP_ACP, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}


char* Base::G2U(const char* gb2312)
{
	int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
	wchar_t* wstr = new wchar_t[len + 1];
	memset(wstr, 0, len + 1);
	MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
	len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
	char* str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
	if (wstr) delete[] wstr;
	return str;
}


void Base::CharToWchar(const char* constCharString, TCHAR* outWchar)
{
	int   nLen = strlen(constCharString) + 1;
	int   nwLen = MultiByteToWideChar(CP_ACP, 0, constCharString, nLen, NULL, 0);

	TCHAR* wString;
	wString = new TCHAR[nwLen];


	MultiByteToWideChar(CP_ACP, 0, constCharString, nLen, wString, nwLen);
	_tcscpy(outWchar, wString);//   wcscpy(outWchar,wString);


	delete[] wString;
}


void Base::WCharToChar(TCHAR* InWchar, char* OutStr)
{
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, InWchar, -1, NULL, 0, NULL, FALSE) + 1;
	WideCharToMultiByte(CP_OEMCP, NULL, InWchar, wcslen(InWchar), OutStr, dwNum, NULL, FALSE);
}


string Base::wstring2string(wstring wstr)
{
	string result;
	//��ȡ��������С��������ռ䣬��������С�°��ֽڼ����  
	int len = WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), NULL, 0, NULL, NULL);
	char* buffer = new char[len + 1];
	//���ֽڱ���ת���ɶ��ֽڱ���  
	WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), wstr.size(), buffer, len, NULL, NULL);
	buffer[len] = '\0';
	//ɾ��������������ֵ  
	result.append(buffer);
	delete[] buffer;
	return result;
}


wstring Base::string2wstring(string str)
{
	wstring result;
	//��ȡ��������С��������ռ䣬��������С���ַ�����  
	int len = MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), NULL, 0);
	TCHAR* buffer = new TCHAR[len + 1];
	//���ֽڱ���ת���ɿ��ֽڱ���  
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.size(), buffer, len);
	buffer[len] = '\0';             //����ַ�����β  
	//ɾ��������������ֵ  
	result.append(buffer);
	delete[] buffer;
	return result;
}


string Base::LPCTSTR2string(LPCTSTR lpctstr)
{
#ifdef UNICODE
	wstring strName(lpctstr);
	string str = wstring2string(strName);
	return str;
#else
	string strName(lpctstr);
	return strName;
#endif // UNICODE	
}


LPTSTR Base:: string2LPTSTR(string str)
{
/*
 Ϊʲô���������,��ο�
 https://docs.microsoft.com/zh-cn/cpp/preprocessor/predefined-macros?view=msvc-170
 _NATIVE_WCHAR_T_DEFINED��Ŀ��comutil.h���÷�
 */
#ifdef _NATIVE_WCHAR_T_DEFINED
	#pragma comment(lib,"comsuppw.lib")
#else
	#pragma comment(lib,"comsupp.lib")
#endif
	_bstr_t bstr(str.c_str());
	return (LPTSTR)bstr;
}


string Base::GetSysErrorMessage(_Out_opt_ int* pErrCode)
{
	string errMsg;
	LPVOID lpMsgBuf;

	*pErrCode = 0;
	*pErrCode = GetLastError();
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		*pErrCode,			//������
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Ĭ���������
		reinterpret_cast<LPTSTR>(&lpMsgBuf),
		0,
		NULL
	);

	//lstrcpy(*szMsg, (LPTSTR)lpMsgBuf);
	errMsg = LPCTSTR2string((LPCTSTR)lpMsgBuf);
	//MessageBox(NULL,(LPCTSTR)lpMsgBuf, L"ERROR", MB_OK);
	// �ͷŻ�����
	LocalFree(lpMsgBuf);

	return errMsg;
}



BOOL Base::EasyDownLoadFile(LPCTSTR lpcszURL, LPCTSTR localFilePath)
{
	BOOL iswhole = TRUE;
	LPCTSTR lpszName = lpcszURL;

#ifdef UNICODE
	wstring strName(lpszName);
	std::vector<std::string> str{ wstring2string(strName) };
#else
	string strName(lpszName);
	std::vector<std::string> str{ strName };
#endif // UNICODE

	// ��׼url��������ʽ
	//std::string pattern{ "^(http|https|ftp)\://([a-zA-Z0-9\.\-]+(\:[a-zA-Z0-9\.&amp;%\$\-]+)*@)*((25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9])\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9]|0)\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9]|0)\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[0-9])|localhost|([a-zA-Z0-9\-]+\.)*[a-zA-Z0-9\-]+\.(com|edu|gov|int|mil|net|org|biz|arpa|info|name|pro|aero|coop|museum|[a-zA-Z]{2}|[a-zA-Z]{1}))(\:[0-9]+)*(/($|[a-zA-Z0-9\.\,\?\'\\\+&amp;%\$#\=~_\-]+))*$"}; 
	std::string pattern{ "^(https?|http|ftp|file)://[-A-Za-z0-9+&@#/%?=~_|!:,.;]+[-A-Za-z0-9+&@#/%=~_|]"}; 
	std::regex re(pattern);

	//std::vector<std::string> str{"http://www.baidu.com", "www.baidu.com"};
	/* std::regex_search:
		������regex_match,������Ҫ�������ַ�������ȫƥ��
		������regex_search�����������е�һ�������У���������ƥ��������ʽre
	*/

	for (auto tmp : str) {
		bool ret = std::regex_search(tmp, re);
		/*if (ret) fprintf(stderr, "%s, can search\n", tmp.c_str());
		else fprintf(stderr, "%s, can not search\n", tmp.c_str());*/

		if (!ret)
		{
			iswhole = FALSE;
		}
	}

	#pragma	comment(lib,"Urlmon.lib")
	#pragma	comment(lib,"Wininet.lib")

	if (NULL == lpcszURL || _tcslen(lpcszURL) <= 0 || NULL == localFilePath || _tcslen(localFilePath) <= 0)
		return FALSE;

	::DeleteUrlCacheEntry(lpcszURL);

	for (int nTry = 0; nTry < 10; nTry++)
	{
		if (FALSE == iswhole)
		{
			LPCTSTR temp = lpcszURL;

			lpcszURL = CString(L"https://") + CString(lpcszURL);
			if (SUCCEEDED(::URLDownloadToFile(NULL, lpcszURL, localFilePath, 0, NULL)))
				return TRUE;
			else
				lpcszURL = temp;

			lpcszURL = CString(L"ftp://") + CString(lpcszURL);
			if (SUCCEEDED(::URLDownloadToFile(NULL, lpcszURL, localFilePath, 0, NULL)))
				return TRUE;
			else
				lpcszURL = temp;

			lpcszURL = CString(L"http://") + CString(lpcszURL);
			if (SUCCEEDED(::URLDownloadToFile(NULL, lpcszURL, localFilePath, 0, NULL)))
				return TRUE;
			else
				lpcszURL = temp;
		}
		else if (SUCCEEDED(::URLDownloadToFile(NULL, lpcszURL, localFilePath, 0, NULL)))
				return TRUE;

		Sleep(100);
	}
	return FALSE;
}


void Base::Wcout(CString cstring, ...)
{
	CString var_str;
	va_list	ap;
	va_start(ap, cstring);
	var_str.FormatV(cstring, ap);
	va_end(ap);

	wcout.imbue(std::locale("chs"));
	wcout << var_str.GetBuffer()<<endl;
}


CString Base::GetFileVersion(LPCSTR filename)
{
	//#pragma warning(disable:4996)
	#pragma comment(lib,"version.lib")
	string asVer = "";
	VS_FIXEDFILEINFO* pVsInfo;
	unsigned int iFileInfoSize = sizeof(VS_FIXEDFILEINFO);
	int iVerInfoSize = GetFileVersionInfoSizeA(filename, NULL);
	if (iVerInfoSize != 0)
	{
		char* pBuf = NULL;

		while (!pBuf)
		{
			pBuf = new char[iVerInfoSize];
		}
		if (GetFileVersionInfoA(filename, 0, iVerInfoSize, pBuf))
		{
			if (VerQueryValueA(pBuf, "\\", (void**)&pVsInfo, &iFileInfoSize))
			{
				sprintf(pBuf, "%d.%d.%d.%d", HIWORD(pVsInfo->dwFileVersionMS), LOWORD(pVsInfo->dwFileVersionMS), HIWORD(pVsInfo->dwFileVersionLS), LOWORD(pVsInfo->dwFileVersionLS));
				asVer = pBuf;
			}
		}
		delete pBuf;
	}
	return CString(asVer.c_str());
}


BOOL Base::CompareVersion(/*LARGE_INTEGER llLeft,*/ LPCTSTR lpLeftVersion, LPCTSTR lpRightVersion, CString& sOpt, BOOL& bResult)
{
	LARGE_INTEGER llLeft;
	DWORD wHMS_L = 0, wHLS_L = 0, wLMS_L = 0, wLLS_L = 0;
	swscanf_s(lpLeftVersion, L"%u.%u.%u.%u", &wHMS_L, &wHLS_L, &wLMS_L, &wLLS_L);
	llLeft.HighPart = MAKELONG(wHLS_L, wHMS_L);
	llLeft.LowPart = MAKELONG(wLLS_L, wLMS_L);



	LARGE_INTEGER llRight;
	DWORD wHMS_R = 0, wHLS_R = 0, wLMS_R = 0, wLLS_R = 0;
	swscanf_s(lpRightVersion, L"%u.%u.%u.%u", &wHMS_R, &wHLS_R, &wLMS_R, &wLLS_R);
	llRight.HighPart = MAKELONG(wHLS_R, wHMS_R);
	llRight.LowPart = MAKELONG(wLLS_R, wLMS_R);

	BOOL bRet = TRUE;
	if (sOpt == L"<")
	{
		bResult = (llLeft.QuadPart < llRight.QuadPart);
	}
	else if (sOpt == L"<=")
	{
		bResult = (llLeft.QuadPart <= llRight.QuadPart);
	}
	else if (sOpt == L">")
	{
		bResult = (llLeft.QuadPart > llRight.QuadPart);
	}
	else if (sOpt == L">=")
	{
		bResult = (llLeft.QuadPart >= llRight.QuadPart);
	}
	else if (sOpt == L"=")
	{
		bResult = (llLeft.QuadPart == llRight.QuadPart);
	}
	else if (sOpt == L"!=")
	{
		bResult = (llLeft.QuadPart != llRight.QuadPart);
	}
	else
	{
		bRet = FALSE;
		ATLASSERT(FALSE);
	}

	return bRet;
}


BOOL Base::IsDirectoryWritable(CString lpwszPath)
{
	BOOL bRet = FALSE;

	if (PathIsDirectoryW(lpwszPath))
	{
		WCHAR wszPath[MAX_PATH] = { 0 };
		PathCombineW(wszPath, lpwszPath, _T("Mypublic.test"));

		HANDLE hFile = CreateFileW(wszPath, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			bRet = TRUE;
			CloseHandle(hFile);
		}

		DeleteFileW(wszPath);
	}

	return bRet;
}


void Base:: SystemTimeToTimet(SYSTEMTIME st, time_t* pt)
{
	FILETIME ft;
	SystemTimeToFileTime(&st, &ft);

	LONGLONG ll;

	ULARGE_INTEGER ui;
	ui.LowPart = ft.dwLowDateTime;
	ui.HighPart = ft.dwHighDateTime;

	ll = ((LONGLONG)ft.dwHighDateTime << 32) + ft.dwLowDateTime;

	*pt = (DWORD)((LONGLONG)(ui.QuadPart - 116444736000000000) / 10000000);
}


void Base::TimetToSystemTime(time_t t, LPSYSTEMTIME pst)
{
	FILETIME ft;

	LONGLONG ll = Int32x32To64(t, 10000000) + 116444736000000000;
	ft.dwLowDateTime = (DWORD)ll;
	ft.dwHighDateTime = (DWORD)(ll >> 32);

	FileTimeToSystemTime(&ft, pst);
}


CString Base:: GetProcessUserNameAndIntegrity(DWORD dwPid, DWORD* pdwLevel)
{
	#define   INTEGRITY_LEVEL_LOW			1  
	#define   INTEGRITY_LEVEL_MEDIUM		2  
	#define   INTEGRITY_LEVEL_HIGH			3  
	#define   INTEGRITY_LEVEL_SYSTEM		4  

	HANDLE hProcess = NULL;
	CString strUserName = L"";
	HANDLE hToken = NULL;
	PTOKEN_USER pTokenUser = NULL;
	DWORD dwSize = 0;
	WCHAR szName[200] = {};
	WCHAR szDomain[200] = {};
	DWORD cchName = 200;
	SID_NAME_USE SNU;
	PTOKEN_MANDATORY_LABEL pTIL = NULL;
	DWORD dwIntegrityLevel;

	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwPid);
	if (!hProcess)
	{
		WRITE_LOG(L"GetProcessUserName OpenProcess Failed PID[%d]GLE[%d]", dwPid, GetLastError());
		goto _exit;
	}

	if (!OpenProcessToken(hProcess, TOKEN_QUERY, &hToken))
	{
		WRITE_LOG(L"GetProcessUserName OpenProcessToken Failed PID[%d]GLE[%d]", dwPid, GetLastError());
		goto _exit;
	}

	if (!GetTokenInformation(hToken, TokenUser, NULL, 0, &dwSize))
	{
		if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
		{
			WRITE_LOG(L"GetProcessUserName GetTokenInformation1 Failed PID[%d]GLE[%d]", dwPid, GetLastError());
			goto _exit;
		}

	}
	if (dwSize == 0)
		goto _exit;

	pTokenUser = (PTOKEN_USER)new BYTE[dwSize];
	if (!GetTokenInformation(hToken, TokenUser, pTokenUser, dwSize, &dwSize))
	{
		WRITE_LOG(L"GetProcessUserName GetTokenInformation2 Failed PID[%d]GLE[%d]", dwPid, GetLastError());
		goto _exit;
	}

	if (!LookupAccountSid(NULL, pTokenUser->User.Sid, szName, &cchName, szDomain, &cchName, &SNU))
	{
		WRITE_LOG(L"GetProcessUserName LookupAccountSid Failed PID[%d]GLE[%d]", dwPid, GetLastError());
		goto _exit;
	}

	strUserName = szName;

	if (0 == strUserName.GetLength())
	{
		WRITE_LOG(L"Waring:GetProcessUserName Name Is Empty...");
	}

	if (pdwLevel)
	{
		if (!GetTokenInformation(hToken, TokenIntegrityLevel, NULL, 0, &dwSize))
		{
			if (GetLastError() != ERROR_INSUFFICIENT_BUFFER)
				goto _exit;

		}
		if (dwSize == 0)
			goto _exit;

		pTIL = (PTOKEN_MANDATORY_LABEL)new BYTE[dwSize];
		if (!GetTokenInformation(hToken, TokenIntegrityLevel, pTIL, dwSize, &dwSize))
			goto _exit;

		dwIntegrityLevel = *GetSidSubAuthority(pTIL->Label.Sid, (DWORD)(UCHAR)(*GetSidSubAuthorityCount(pTIL->Label.Sid) - 1));

		if (dwIntegrityLevel == SECURITY_MANDATORY_LOW_RID)
		{
			// Low Integrity
			*pdwLevel = INTEGRITY_LEVEL_LOW;

		}
		else if (dwIntegrityLevel >= SECURITY_MANDATORY_MEDIUM_RID &&
			dwIntegrityLevel < SECURITY_MANDATORY_HIGH_RID)
		{
			// Medium Integrity
			*pdwLevel = INTEGRITY_LEVEL_MEDIUM;
		}
		else if (dwIntegrityLevel >= SECURITY_MANDATORY_HIGH_RID)
		{
			// High Integrity
			*pdwLevel = INTEGRITY_LEVEL_HIGH;
		}
		else if (dwIntegrityLevel >= SECURITY_MANDATORY_SYSTEM_RID)
		{
			// System Integrity
			*pdwLevel = INTEGRITY_LEVEL_SYSTEM;
		}

	}

_exit:
	if (hProcess)
		CloseHandle(hProcess);

	if (hToken)
		CloseHandle(hToken);

	if (pTokenUser)
		delete pTokenUser;

	if (pTIL)
		delete pTIL;

	return strUserName;
}


CString Base::WhoIsUser(DWORD dwPid)
{
	DWORD dwIntegrityLevel = 0;
	CString strUsername = GetProcessUserNameAndIntegrity(dwPid, &dwIntegrityLevel);
	//if (dwIntegrityLevel == INTEGRITY_LEVEL_HIGH && !strUsername.IsEmpty() && strUsername.CompareNoCase(L"SYSTEM") == 0)
	//{
	//	//��system�û��£��������Լ���Ӧ����High
	//	return ;
	//}

	return strUsername;
}


//BOOL Base::DosPathToNtPath(LPTSTR pszDosPath, LPTSTR pszNtPath)
//{
//	TCHAR			szDriveStr[500];
//	TCHAR			szDrive[3];
//	TCHAR			szDevName[100];
//	INT				cchDevName;
//	INT				i;
//
//	//������
//	if (!pszDosPath || !pszNtPath)
//		return FALSE;
//
//	//��ȡ���ش����ַ���
//	if (GetLogicalDriveStrings(sizeof(szDriveStr), szDriveStr))
//	{
//		for (i = 0; szDriveStr[i]; i += 4)
//		{
//			if (!lstrcmpi(&(szDriveStr[i]), _T("A:\\")) || !lstrcmpi(&(szDriveStr[i]), _T("B:\\")))
//				continue;
//
//			szDrive[0] = szDriveStr[i];
//			szDrive[1] = szDriveStr[i + 1];
//			szDrive[2] = '\0';
//			if (!QueryDosDevice(szDrive, szDevName, 100))//��ѯ Dos �豸��
//				return FALSE;
//
//			cchDevName = lstrlen(szDevName);
//			if (_tcsnicmp(pszDosPath, szDevName, cchDevName) == 0)//����
//			{
//				lstrcpy(pszNtPath, szDrive);//����������
//				lstrcat(pszNtPath, pszDosPath + cchDevName);//����·��
//
//				return TRUE;
//			}
//		}
//	}
//
//	lstrcpy(pszNtPath, pszDosPath);
//
//	return FALSE;
//}


BOOL Base::GetProcessFullPath(DWORD dwPID, TCHAR* pszFullPath)
{
	TCHAR       szImagePath[MAX_PATH];
	HANDLE      hProcess;

	if (!pszFullPath)
		return FALSE;

	pszFullPath[0] = '\0';
	hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, 0, dwPID);
	if (!hProcess)
		return FALSE;

	if (!GetProcessImageFileName(hProcess, szImagePath, MAX_PATH))
	{
		CloseHandle(hProcess);
		return FALSE;
	}

	if (!DosPathToNtPath(szImagePath, pszFullPath))
	{
		CloseHandle(hProcess);
		return FALSE;
	}

	CloseHandle(hProcess);

	return TRUE;
}


int Base::EnablePrivilege(LPCWSTR lpszPrivilegeName, BOOL bEnable)
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


BOOL Base::IsOfficeProcRuning()
{
	BOOL bRet = FALSE;

	LPCWSTR lpwszOfficeProc[] = { L"WINWORD.EXE", L"EXCEL.EXE", L"MSACCESS.EXE", L"POWERPNT.EXE", L"OUTLOOK.EXE", L"GROOVE.EXE", L"INFOPATH.EXE", L"MSPUB.EXE", L"ONENOTE.EXE", L"VISIO.EXE" };

	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapShot != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 pe = { 0 };
		memset(&pe, 0, sizeof(pe));
		pe.dwSize = sizeof(pe);
		if (Process32First(hSnapShot, &pe))
		{
			do
			{
				CString strName = pe.szExeFile;
				strName.Trim();
				strName.MakeLower();

				if (strName.GetLength() > 0)
				{
					int nCount = sizeof(lpwszOfficeProc) / sizeof(lpwszOfficeProc[0]);
					for (int i = 0; i < nCount; i++)
					{
						if (lstrcmpi(strName, lpwszOfficeProc[i]) == 0)
						{
							bRet = TRUE;
							break;
						}
					}
				}

				if (bRet)
				{
					break;
				}

				memset(&pe, 0, sizeof(pe));
				pe.dwSize = sizeof(pe);

			} while (Process32Next(hSnapShot, &pe));
		}

		CloseHandle(hSnapShot);
	}

	return bRet;
}


BOOL Base::KillOfficeProc()
{
	BOOL bRet = FALSE;

	LPCWSTR lpwszOfficeProc[] = { L"WINWORD.EXE", L"EXCEL.EXE", L"MSACCESS.EXE", L"POWERPNT.EXE", L"OUTLOOK.EXE", L"GROOVE.EXE", L"INFOPATH.EXE", L"MSPUB.EXE", L"ONENOTE.EXE", L"VISIO.EXE" };

	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapShot != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 pe = { 0 };
		memset(&pe, 0, sizeof(pe));
		pe.dwSize = sizeof(pe);
		if (Process32First(hSnapShot, &pe))
		{
			do
			{
				CString strName = pe.szExeFile;
				strName.Trim();
				strName.MakeLower();

				if (strName.GetLength() > 0)
				{
					int nCount = sizeof(lpwszOfficeProc) / sizeof(lpwszOfficeProc[0]);
					for (int i = 0; i < nCount; i++)
					{
						if (lstrcmpi(strName, lpwszOfficeProc[i]) == 0)
						{
							TerminateProcessByPid(pe.th32ProcessID);
							break;
						}
					}
				}

				memset(&pe, 0, sizeof(pe));
				pe.dwSize = sizeof(pe);

			} while (Process32Next(hSnapShot, &pe));
		}

		CloseHandle(hSnapShot);
	}

	return bRet;
}


vector<DWORD> Base::GetProceeIDfromParentID(DWORD& dwParentProcessId)
{
	vector<DWORD> parent_id_vec;
	DWORD dwProcessID = 0;

	//����һ�����̿���
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		cout << ("���̿���ʧ��");
		return parent_id_vec;
	}

	PROCESSENTRY32 pe;
	TCHAR procID[100] = { 0 };
	pe.dwSize = sizeof(pe);
	BOOL bProcess = Process32First(hProcessSnap, &pe);
	//�������н���
	while (bProcess)
	{
		if (pe.th32ParentProcessID == dwParentProcessId)//�ж������id����pid��ȣ�
		{

			dwProcessID = pe.th32ProcessID;
			parent_id_vec.push_back(dwProcessID);
		}
		bProcess = Process32Next(hProcessSnap, &pe);
	}
	CloseHandle(hProcessSnap);
	return parent_id_vec;
}


void Base::TerminateProcessByPid(DWORD dwPid, BOOL andChildProcess)
{
	HANDLE hProc = OpenProcess(PROCESS_TERMINATE, FALSE, dwPid);
	if (hProc)
	{
		if (andChildProcess)
		{
			vector<DWORD> dwChildrenPid = GetProceeIDfromParentID(dwPid);
			if (!dwChildrenPid.empty())
			{
				auto atChildrenPid = dwChildrenPid.begin();
				for (int i = 0; atChildrenPid != dwChildrenPid.end(); atChildrenPid++, i++)
				{
					TerminateProcessByPid(atChildrenPid[i]);
				}
			}
		}
		TerminateProcess(hProc, 0);
		CloseHandle(hProc);
	}
}


BOOL Base::TerminateProcessByName(CString ProcName, BOOL andChildProcess)
{
	BOOL bRet = FALSE;

	LPCWSTR lpwszProc[] = { ProcName };

	HANDLE hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hSnapShot != INVALID_HANDLE_VALUE)
	{
		PROCESSENTRY32 pe = { 0 };
		memset(&pe, 0, sizeof(pe));
		pe.dwSize = sizeof(pe);
		if (Process32First(hSnapShot, &pe))
		{
			do
			{
				CString strName = pe.szExeFile;
				strName.Trim();
				strName.MakeLower();

				if (strName.GetLength() > 0)
				{
					int nCount = sizeof(lpwszProc) / sizeof(lpwszProc[0]);
					for (int i = 0; i < nCount; i++)
					{
						if (lstrcmpi(strName, lpwszProc[i]) == 0)
						{
							if (TRUE == andChildProcess)
							{
								vector<DWORD> dwChildrenPid = GetProceeIDfromParentID(pe.th32ProcessID);
								if (!dwChildrenPid.empty())
								{
									auto atChildrenPid = dwChildrenPid.begin();
									for (int i =0; atChildrenPid != dwChildrenPid.end(); atChildrenPid++,i++)
									{
										TerminateProcessByPid(atChildrenPid[i]);
									}
								}
							}
							TerminateProcessByPid(pe.th32ProcessID);
							break;
						}
					}
				}

				memset(&pe, 0, sizeof(pe));
				pe.dwSize = sizeof(pe);

			} while (Process32Next(hSnapShot, &pe));
		}
		CloseHandle(hSnapShot);
	}

	return bRet;
}


BOOL Base::Str2Int(_In_ string str, _Out_ int& resNum)
{
	//stringstream sin(str);
	//double d;
	//char c;
	//int e;
	//if (!(sin >> d))
	//{
	//	/*���ͣ�
	//		sin>>t��ʾ��sinת����double�ı�������ʵ����int��float�͵Ķ�����գ���
	//		���ת���ɹ�����ֵΪ��0�����ת�����ɹ��ͷ���Ϊ0
	//	*/
	//	cout << "��������ַ�!" << endl;
	//	return false;
	//}
	//if (sin >> c)
	//{
	//	/*���ͣ�
	//	�˲������ڼ����������У����ּ��ַ�����������ʽ�����磺34.f����������ĵĲ��֣�sin>>t��
	//	�Ѿ����ղ�ת������������ֲ��֣���stringstream����ӦҲ�����һ���ָ������
	//	��ʱ���յ���.f�ⲿ�֣�������������������false
	//	  */
	//	cout << "��������ַ�!" << endl;
	//	return false;
	//}
	//sin >> e;
	//nLoop = e;
	//return true;

	int buffer = 0;
	try
	{
		buffer = stoi(str);
	}
	catch (...)
	{
		return FALSE;
	}

	resNum = buffer;
	return TRUE;
}


int Base:: GetCurrScreenNmber()
{
	int screenNum = 0;
	screenNum = GetSystemMetrics(SM_CMONITORS);
	return screenNum;
}


int Base::ProcessModule(DWORD pid, _Out_ vector<tstring>& vtTstring)
{
	vector<tstring> patch;

	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
	if (hProcessSnap)
	{
		MODULEENTRY32 me32;
		me32.dwSize = sizeof(MODULEENTRY32);
		Module32First(hProcessSnap, &me32);//��ȡ���̵�һ��ģ����Ϣ
		do
		{
			//printf("ģ��·��:%s\n",me32.szExePath);
			// patch.push_back(me32.szExePath);
			//printf("ģ����:%s\n",me32.szModule);
			patch.push_back(me32.szModule);
			//printf("ģ���ַ:0x%08X\n",(DWORD)me32.modBaseAddr);

		} while (Module32Next(hProcessSnap, &me32));
		CloseHandle(hProcessSnap);
		vtTstring = patch;
		return TRUE;
	}
	else
	{
		cout << "��ȡ���̿���ʧ��" << endl;;
		return FALSE;
	}
	return FALSE;
}


BOOL Base::RunAppInAuthorityByProessName(LPCWSTR lpFile, LPCWSTR lpParam, LPCWSTR srcProcess)
{
	bool bRet = false;
	HANDLE srcProcessToken = NULL;
	do
	{
		if (GetTokenByName(srcProcessToken, (LPTSTR)srcProcess))
		{
			if (NULL != srcProcessToken)
			{

			}
			else
				return FALSE;
		}
		else
			return FALSE;


		DWORD dwReturnLen = 0;
		DWORD dwTokenSessionId = 0;
		if (::GetTokenInformation(srcProcessToken, TokenSessionId, &dwTokenSessionId, sizeof(DWORD), &dwReturnLen))
		{
			bRet = CreateProcessAsSystemBySession(lpFile, lpParam, dwTokenSessionId, srcProcessToken);
		}
		else
		{
			WRITE_LOG(L"��ȡToken %u �� TokenSessionIdʧ�ܣ������� %d,��Token������������", srcProcessToken, GetLastError());
		}

		CloseHandle(srcProcessToken);

	} while (false);

	return bRet;
}


BOOL Base::RunAppInAuthorityByProessID(LPCWSTR lpFile, LPCWSTR lpParam, DWORD srcPid)
{
	bool bRet = false;
	HANDLE srcProcessToken = NULL;
	do
	{
		if (GetTokenByPid(srcProcessToken, srcPid))
		{
			if (NULL != srcProcessToken)
			{

			}
			else
				return FALSE;
		}
		else
			return FALSE;


		DWORD dwReturnLen = 0;
		DWORD dwTokenSessionId = 0;
		if (::GetTokenInformation(srcProcessToken, TokenSessionId, &dwTokenSessionId, sizeof(DWORD), &dwReturnLen))
		{
			bRet = CreateProcessAsSystemBySession(lpFile, lpParam, dwTokenSessionId, srcProcessToken);
		}
		else
		{
			WRITE_LOG(L"��ȡToken %u �� TokenSessionIdʧ�ܣ������� %d,��Token������������", srcProcessToken, GetLastError());
		}

		CloseHandle(srcProcessToken);

	} while (false);

	return bRet;
}


void Base::SplitString(CString strLine, std::vector<CString>& vec_rlt, const CString& strSep /* = T("|")*/)
{
	vec_rlt.clear();
	strLine.Trim();
	while (!strLine.IsEmpty())
	{
		int nPos = strLine.Find(strSep);
		if (-1 != nPos)
		{
			vec_rlt.push_back(strLine.Left(nPos));
			strLine = strLine.Mid(nPos + strSep.GetLength());
		}
		else
		{
			vec_rlt.push_back(strLine);
			strLine = _T("");
		}
	}
}


void Base::SplitString(CString strLine, std::list<CString>& vec_rlt, const CString& strSep /* = T("|")*/)
{
	vec_rlt.clear();
	strLine.Trim();
	while (!strLine.IsEmpty())
	{
		int nPos = strLine.Find(strSep);
		if (-1 != nPos)
		{
			vec_rlt.push_back(strLine.Left(nPos));
			strLine = strLine.Mid(nPos + strSep.GetLength());
		}
		else
		{
			vec_rlt.push_back(strLine);
			strLine = _T("");
		}
	}
}


DWORD Base::SomGetSystemRebootTime()
{
	return _time32(NULL) - (GetTickCount() / 1000);
}


DWORD Base::SomGetCurrentTime()
{
	return _time32(NULL);
}