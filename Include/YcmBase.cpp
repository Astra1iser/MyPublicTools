//Base�������ʵ��
#pragma once
#include <YcmBase.h>
#include <Pathmanager.cpp>
#include <RegeditManager.cpp>
#include <MyOperFile.cpp>
#include <Thread.cpp>
#include <tinyxml2.cpp>

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

//extern "C" __declspec(dllexport) int __stdcall SetLangaueSyncOS()	 //����������д��ע�����
//{
//	CString strLanguage = L"zh_cn";
//
//	LANGID id = GetSystemDefaultLangID();
//	switch(id)
//	{
//	case 0x0804:	//��������
//	case 0x0404:	//̨��
//	case 0x0c04:	//���
//		strLanguage = L"zh_cn";
//		WRITE_LOG(L"��ʾ���֣���������");
//		break;
//	case 0x0409:
//		strLanguage = L"en_us";
//		WRITE_LOG(L"��ʾ���֣�Ӣ��");
//		break;
//	default:
//		strLanguage = L"en_us";
//		WRITE_LOG(L"δ֪�����Ի�������ʾ���֣�Ӣ��");
//	}
//
//	if (!strLanguage.IsEmpty())
//	{
//		REG_UTIL::setReg(kModuleBase, kLanguageServerName, strLanguage.GetString());
//	}
//
//	return TRUE;
//
//}

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

BOOL Base::StartPrograme(LPCTSTR Path, LPCTSTR Parameters, BOOL IsAdmin, BOOL IsWaitForSingle)
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


	//Ҫ���е��ļ�·��
	//ShExecInfo.lpFile = L"cmd";
	ShExecInfo.lpFile = Path;

	//����/�򿪳���Ĳ���������򿪵���һ���ĵ����������Ч
	ShExecInfo.lpParameters = Parameters;

	//ָ������Ŀ¼�����֣���Աû��˵������Ĭ��Ϊ��ǰĿ¼
	ShExecInfo.lpDirectory = NULL;

	//���ô�����ʾ(SW_SHOW)�Ͳ���ʾ(SW_HIDE)����Ȼ����������
	ShExecInfo.nShow = SW_SHOW;

	//����������гɹ��������ֵ������32������������ֵ�Լ���
	ShExecInfo.hInstApp = NULL;
	if (ShellExecuteEx(&ShExecInfo))
	{
		//�̹߳���,ֱ������Ľ�������Ӧ ��:�ر�
		if (IsWaitForSingle == TRUE)
			WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
		return TRUE;
	}	
	else
		return FALSE;	
}

void Base::GetIniValue(int& Source, LPCTSTR  Node, LPCTSTR Key, LPCTSTR IniPath)
{
	//int����
	auto temp = GetPrivateProfileInt(Node, Key, NULL, IniPath);
	Source = temp;
}

void Base::GetIniValue(LPCTSTR& Source, LPCTSTR Node, LPCTSTR Key, LPCTSTR IniPath)
{
	//CString����
	ULONG len = MAX_PATH;
	WCHAR szValue[MAX_PATH] = { 0 };
	GetPrivateProfileString(Node, Key, NULL, szValue, len, IniPath);
	Source = szValue;
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
	string strName(lpszName);
	return strName;
#endif // UNICODE	
}

int Base::EasyDownLoadFile(LPCTSTR lpcszURL, LPCTSTR localFilePath)
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
	std::string pattern{ """^(http|https|ftp)\://([a-zA-Z0-9\.\-]+(\:[a-zA-Z0-9\.&amp;%\$\-]+)*@)*((25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9])\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9]|0)\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9]|0)\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[0-9])|localhost|([a-zA-Z0-9\-]+\.)*[a-zA-Z0-9\-]+\.(com|edu|gov|int|mil|net|org|biz|arpa|info|name|pro|aero|coop|museum|[a-zA-Z]{2}|[a-zA-Z]{1}))(\:[0-9]+)*(/($|[a-zA-Z0-9\.\,\?\'\\\+&amp;%\$#\=~_\-]+))*$"""}; 
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

int Base::CreateEmptyXML(const char* xmlPath, const char* rootNodeName)
{
	const char* declaration = "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>";
	XMLDocument doc;
	doc.Parse(declaration);//�Ḳ��xml��������

	//�����������ʹ����������
	//XMLDeclaration* declaration=doc.NewDeclaration();
	//doc.InsertFirstChild(declaration);

	XMLElement* rootNode = doc.NewElement(rootNodeName);
	doc.InsertEndChild(rootNode);

	return doc.SaveFile(xmlPath);
}