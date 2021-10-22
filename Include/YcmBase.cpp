//Base�������ʵ��
#pragma once
#include <YcmBase.h>
#include <Pathmanager.cpp>
#include <RegeditManager.cpp>
#include <MyOperFile.cpp>
#include <Thread.cpp>
#include <tinyxml2.cpp>
#include <httpdown.cpp>
#include <Mutex.cpp>
#include <FIFO.cpp>
#include <SharedMemory.cpp>
#include <MyIni.cpp>
#include <HttpPost.cpp>



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


LPCTSTR Base::SetLangaueSyncOS()
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

	return NULL;

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
	std::string pattern{ "^(http|https|ftp)\://([a-zA-Z0-9\.\-]+(\:[a-zA-Z0-9\.&amp;%\$\-]+)*@)*((25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9])\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9]|0)\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[1-9]|0)\.(25[0-5]|2[0-4][0-9]|[0-1]{1}[0-9]{2}|[1-9]{1}[0-9]{1}|[0-9])|localhost|([a-zA-Z0-9\-]+\.)*[a-zA-Z0-9\-]+\.(com|edu|gov|int|mil|net|org|biz|arpa|info|name|pro|aero|coop|museum|[a-zA-Z]{2}|[a-zA-Z]{1}))(\:[0-9]+)*(/($|[a-zA-Z0-9\.\,\?\'\\\+&amp;%\$#\=~_\-]+))*$"}; 
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


void Base::Wcout(CString cstring)
{
	wcout.imbue(std::locale("chs"));
	wcout << cstring.GetBuffer()<<endl;
}


XMLDocument* Base::CreateEmptyXMLFile(const char* xmlPath, const char* rootNodeName)
{
	const char* declaration = "<?xml version=\"1.0\" encoding=\"GBK\" standalone=\"yes\"?>";
	XMLDocument* doc = new XMLDocument;
	doc->Parse(declaration);//�Ḳ��xml��������

	//�����������ʹ����������
	//XMLDeclaration* declaration=doc.NewDeclaration();
	//doc.InsertFirstChild(declaration);

	//XMLElement* rootNode = doc->NewElement(G2U(rootNodeName));
	XMLElement* rootNode = doc->NewElement(rootNodeName);
	doc->InsertEndChild(rootNode);

	if (!(doc->SaveFile(xmlPath)))
	{
		return NULL;
	}

	return doc;
}


XMLDocument* Base::LoadXMLFile(const char* xmlPath)
{
	XMLDocument* doc = new XMLDocument;
	int res = doc->LoadFile(xmlPath);
	if (res != 0)
	{
		return NULL;
	}
	return doc;
}


BOOL Base::SaveXMLFile(XMLDocument* doc, const char* xmlSavePath)
{
	if ((doc->SaveFile(xmlSavePath)))
	{
		return FALSE;
	}

	//delete doc;
	return TRUE;
}


BOOL Base::GetXMLDeclaration(XMLDocument* doc, string& strDecl)
{
	if (NULL == doc)
		return FALSE;
	XMLNode* decl = doc->FirstChild();
	if (NULL != decl)
	{
		XMLDeclaration* declaration = decl->ToDeclaration();
		if (NULL != declaration)
		{
			strDecl = declaration->Value();
			return true;
		}
	}
	strDecl = "";
	return false;
}


BOOL Base::FindXMLNode(XMLElement* pRoot, const string nodeName, XMLElement*& pNode, map<const char*, const char*> Attribution)
{

	const char* value = pRoot->Value();
	if (strcmp(pRoot->Value(), nodeName.c_str()) == 0)
	{
		if (0 != Attribution.size())
		{
			int j = 0;
			for (auto it = Attribution.begin(); it != Attribution.end(); ++it)
			{
				string source = pRoot->Attribute(it->first);
				string target = it->second;
				if (source == target)
				{
					j++;
					if (j == Attribution.size())
					{
						pNode = pRoot;
						return TRUE;
					}
				}
			}
		}
		else
		{
			pNode = pRoot;
			return TRUE;
		}
	}

	XMLElement* p = pRoot;
	for (p = p->FirstChildElement(); p != NULL; p = p->NextSiblingElement())
	{
		if (FindXMLNode(p, nodeName, pNode, Attribution))
		{
			return TRUE;
		}
	}
	return FALSE;
}


BOOL Base::GetXMLNodeText(XMLElement* pRoot, const char*& text, const string nodeName, map<const char*, const char*> Attribution)
{
	if (!pRoot)
	{
		return FALSE;
	}

	if ("" == nodeName)
	{
		text = pRoot->GetText();
		return TRUE;
	}

	XMLElement* pNode = NULL;
	if (FindXMLNode(pRoot, nodeName, pNode, Attribution))
	{
		if (NULL != pNode)
		{
			text = pNode->GetText();
			return TRUE;
		}
		return FALSE;
	}
	else
	{
		return FALSE;
	}
}


BOOL Base::GetXMLNodeAttribute(XMLElement* pRoot, map<string, string>& mapAttribute, const string nodeName, map<const char*, const char*> Attribution)
{
	if (!pRoot)
	{
		return FALSE;
	}

	if ("" == nodeName)
	{
		const XMLAttribute* pAttr = NULL;
		for (pAttr = pRoot->FirstAttribute(); pAttr != NULL; pAttr = pAttr->Next())
		{
			string name = pAttr->Name();
			string value = pAttr->Value();
			mapAttribute.insert(make_pair(name, value));
		}
		return TRUE;
	}

	XMLElement* pNode = NULL;
	if (FindXMLNode(pRoot, nodeName, pNode, Attribution))
	{
		if (NULL != pNode)
		{
			const XMLAttribute* pAttr = NULL;
			for (pAttr = pNode->FirstAttribute(); pAttr != NULL; pAttr = pAttr->Next())
			{
				string name = pAttr->Name();
				string value = pAttr->Value();
				mapAttribute.insert(make_pair(name, value));
			}
		}
		return true;
	}
	return false;
}


BOOL Base::SetXMLNodeText(XMLDocument* doc, const char* xmlSavePath, XMLElement* pRoot, const string text, const string nodeName, map<const char*, const char*> Attribution)
{
	if (NULL == pRoot)
	{
		return FALSE;
	}

	if ("" == nodeName)
	{
		try
		{

			XMLNode* pText = pRoot->LastChild();
			if (NULL != pText)
			{
				pText->SetValue(text.c_str());
			}
			else
			{
				//XMLText* pText = doc->NewText(text.c_str());
				//pRoot->InsertFirstChild(pText);
				pRoot->InsertNewText(text.c_str());
			}
		}
		catch (...)
		{
			//XMLText* pText = doc->NewText(text.c_str());
			//pRoot->InsertFirstChild(pText);
			pRoot->InsertNewText(text.c_str());
		}
		if (SaveXMLFile(doc, xmlSavePath))
		{
			return TRUE;
		}
		return FALSE;
	}

	XMLElement* pNode = NULL;
	if (FindXMLNode(pRoot, nodeName, pNode, Attribution))
	{
		try
		{
			XMLNode* pText = pNode->LastChild();
			if (NULL != pText)
			{
				pText->SetValue(text.c_str());
				//pNode->InsertAfterChild(pText, pText);
			}
			else
			{
				//XMLText* pText = doc->NewText(text.c_str());
				//pNode->InsertEndChild(pText);
				pNode->InsertNewText(text.c_str());
			}
		}
		catch (...)
		{
			/*XMLText* pText = doc->NewText(text.c_str());
			pNode->InsertFirstChild(pText);*/
			pNode->InsertNewText(text.c_str());
		}
		if (SaveXMLFile(doc, xmlSavePath))
		{
			return TRUE;
		}
	}
	return FALSE;
}


BOOL Base::SetXMLNodeAttribution(XMLDocument* doc, const char* xmlSavePath, XMLElement* pRoot, map<string, string>& mapAttribute, const string nodeName, map<const char*, const char*> Attribution)
{
	if (NULL == pRoot)
	{
		return FALSE;
	}

	if ("" == nodeName)
	{
		const XMLAttribute* pAttr = pRoot->FirstAttribute();

		if (NULL == pAttr)
		{
			for (auto it = mapAttribute.begin(); it != mapAttribute.end(); ++it)
			{
				if ("" != it->first.c_str())
				{
					if ("" != it->second.c_str())
					{
						pRoot->SetAttribute(it->first.c_str(), it->second.c_str());
					}
					else
					{
						return FALSE;
					}
				}
				else
				{
					return FALSE;
				}
			}
		}

		char* strName = NULL;
		for (; pAttr != NULL; pAttr = pAttr->Next())
		{
			strName = const_cast<char*>(pAttr->Name());
			map<string, string> isnofind = {};
			for (auto it = mapAttribute.begin(); it != mapAttribute.end(); ++it)
			{
				if (strName == it->first)
				{
					if ("" != it->second.c_str())
					{
						pRoot->SetAttribute(strName, it->second.c_str());
					}
					else
					{
						pRoot->DeleteAttribute(strName);
					}
				}
				else
				{
					auto pr = std::make_pair(it->first, it->second);
					isnofind.insert(pr);
				}

			}

			if (0 != isnofind.size())
			{
				for (auto it = isnofind.begin(); it != isnofind.end(); ++it)
				{

					if ("" != it->first.c_str())
					{
						if ("" != it->second.c_str())
						{
							pRoot->SetAttribute(it->first.c_str(), it->second.c_str());
						}
						else
						{
							return FALSE;
						}
					}
					else
					{
						return FALSE;
					}
				}
			}
		}

		if (SaveXMLFile(doc, xmlSavePath))
		{
			return TRUE;
		}
		return FALSE;
	}

	XMLElement* pNode = NULL;
	if (FindXMLNode(pRoot, nodeName, pNode, Attribution))
	{
		const XMLAttribute* pAttr = pNode->FirstAttribute();

		if (NULL == pAttr)
		{
			for (auto it = mapAttribute.begin(); it != mapAttribute.end(); ++it)
			{
				if ("" != it->first.c_str())
				{
					if ("" != it->second.c_str())
					{
						pNode->SetAttribute(it->first.c_str(), it->second.c_str());
					}
					else
					{
						return FALSE;
					}
				}
				else
				{
					return FALSE;
				}
			}
		}

		char* strName = NULL;
		for (; pAttr != NULL; pAttr = pAttr->Next())
		{
			strName = const_cast<char*>(pAttr->Name());
			map<string, string> isnofind = {};
			for (auto it = mapAttribute.begin(); it != mapAttribute.end(); ++it)
			{
				if (strName == it->first)
				{
					if ("" != it->second.c_str())
					{
						pNode->SetAttribute(strName, it->second.c_str());
					}
					else
					{
						pNode->DeleteAttribute(strName);
					}
				}
				else
				{
					auto pr = std::make_pair(it->first, it->second);
					isnofind.insert(pr);
				}

			}

			if (0 != isnofind.size())
			{
				for (auto it = isnofind.begin(); it != isnofind.end(); ++it)
				{

					if ("" != it->first.c_str())
					{
						if ("" != it->second.c_str())
						{
							pNode->SetAttribute(it->first.c_str(), it->second.c_str());
						}
						else
						{
							return FALSE;
						}
					}
					else
					{
						return FALSE;
					}
				}
			}
		}

		if (SaveXMLFile(doc, xmlSavePath))
		{
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}


BOOL Base::SetXMLNewNode(XMLDocument* doc, const char* xmlSavePath, XMLElement* pRoot, const char* newNodeName, map<string, string>newMapAttribute, const char* newText, const string nodeName, map<const char*, const char*> Attribution)
{
	if (NULL == pRoot)
	{
		return FALSE;
	}
	if ("" == nodeName)
	{
		if ("" != newNodeName)
		{
			XMLElement* pNewNode = pRoot->InsertNewChildElement(newNodeName);
			if (0 != newMapAttribute.size())
			{
				SetXMLNodeAttribution(doc, xmlSavePath, pNewNode, newMapAttribute);
			}
			if ("" != newText)
			{
				SetXMLNodeText(doc, xmlSavePath, pNewNode, newText);
			}

			if (SaveXMLFile(doc, xmlSavePath))
			{
				return TRUE;
			}
		}
		return FALSE;
	}

	XMLElement* pNode = NULL;
	if (FindXMLNode(pRoot, nodeName, pNode, Attribution))
	{
		if ("" != newNodeName)
		{
			XMLElement* pNewNode = pNode->InsertNewChildElement(newNodeName);
			if (0 != newMapAttribute.size())
			{
				SetXMLNodeAttribution(doc, xmlSavePath, pNewNode, newMapAttribute);
			}
			if ("" != newText)
			{
				SetXMLNodeText(doc, xmlSavePath, pNewNode, newText);
			}

			if (SaveXMLFile(doc, xmlSavePath))
			{
				return TRUE;
			}
		}
		else
		{
			return FALSE;
		}
	}
	return FALSE;
}


BOOL Base::DeleteXMLNode(XMLDocument* doc, const char* xmlSavePath, XMLElement* pRoot, const char* delNodeName, const string nodeName, map<const char*, const char*> Attribution)
{
	if (NULL == pRoot)
	{
		return FALSE;
	}
	if ("" == nodeName)
	{
		//pRoot->DeleteChild(NodeName); //ɾ��ָ���ڵ�
		pRoot->DeleteChildren();//ɾ���ڵ�����к��ӽڵ�


		if (SaveXMLFile(doc, xmlSavePath))
		{
			return TRUE;
		}
		return FALSE;
	}
	XMLElement* pNode = NULL;
	if (FindXMLNode(pRoot, nodeName, pNode, Attribution))
	{

		//pRoot->DeleteChild(NodeName); //ɾ��ָ���ڵ�
		pNode->DeleteChildren();//ɾ���ڵ�����к��ӽڵ�


		if (SaveXMLFile(doc, xmlSavePath))
		{
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}


BOOL Base::DeleteXMLNode(XMLElement* fatherNode, XMLElement* childrenNode)
{
	fatherNode->DeleteChild(childrenNode); //ɾ��ָ���ڵ�
	return TRUE;
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
