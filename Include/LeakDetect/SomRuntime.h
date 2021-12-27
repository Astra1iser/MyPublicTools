#pragma once

#define REGPATH_IE						L"SOFTWARE\\Microsoft\\Internet Explorer"
#define REGVAL_IE_VERSION				L"Version"
#define REGVAL_IE_SVCVERSION			L"svcVersion"
#define REGPATH_IE2						L"SOFTWARE\\Microsoft\\Internet Explorer\\Main"
#define REGVAL_IE2_VERSION				L"Wizard_Version"

#define REGPATH_IESP					L"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings"
#define REGVAL_MINOR_VERSION			L"MinorVersion"

#define REGPATH_DOTNET_FRAMEWORK		L"Software\\Microsoft\\NET Framework Setup\\NDP"

#define REGPATH_WJSCRIPT				L"SOFTWARE\\Microsoft\\Active Setup\\Installed Components\\{4f645220-306d-11d2-995d-00c04f98bbc9}"
#define REGVAL_JS_VERSION				L"Version"

#define REGPATH_MEDIAPLAYER				L"SOFTWARE\\Microsoft\\MediaPlayer\\PlayerUpgrade"
#define REGVAL_MEDIAPLAYER_VERSION		L"PlayerVersion"

#define REGPATH_OUTLOOKEXPRESS			L"SOFTWARE\\Microsoft\\Outlook Express\\Version Info"
#define REGVAL_OUTLOOKEXPRESS_VERSION	L"Current"

#define REGPATH_DATAACCESS				L"SOFTWARE\\Microsoft\\DataAccess"
#define REGVAL_DATAACCESS_VERSION		L"FullInstallVer"

#define REGPATH_XMLCORE					L"SOFTWARE\\Classes\\TypeLib\\{F5078F18-C551-11D3-89B9-0000F81FE221}"


extern BOOL g_bSomProfile;
extern BOOL g_bDetailLog;

__declspec(selectany) BOOL g_SomGlobal_bWin2k = (LOWORD(GetVersion()) == 0x5);
__declspec(selectany) BOOL g_SomGlobal_bVistaLater = ((LOBYTE(LOWORD(GetVersion()))) >= 0x6);
__declspec(selectany) BOOL g_SomGlobal_bWin64 = IsOS(OS_WOW6432);

class CSomRegistry
{
public:
	CSomRegistry(HKEY hRootKey, CString strSubKey, CString strValueName = NULL, BOOL b64KeyView = FALSE)
		: m_hRoot(hRootKey)
		, m_strSubKey(strSubKey)
		, m_strValue(strValueName)
	{
		m_samDes = g_SomGlobal_bWin2k ? 0 : (b64KeyView ? KEY_WOW64_64KEY : KEY_WOW64_32KEY);
	}
	CSomRegistry(LPCWSTR lpRootKey, CString strSubKey, CString strValueName = NULL, BOOL b64KeyView = FALSE)
		: m_strSubKey(strSubKey)
		, m_strValue(strValueName)
	{
		m_hRoot = ParserKeyName(lpRootKey);
		m_samDes = g_SomGlobal_bWin2k ? 0 : (b64KeyView ? KEY_WOW64_64KEY : KEY_WOW64_32KEY);
	}
	CSomRegistry(LPCWSTR lpKeyValue, BOOL b64KeyView = FALSE)
	{
		m_hRoot = ParserKeyValueName(lpKeyValue, m_strValue, m_strSubKey);
		m_samDes = g_SomGlobal_bWin2k ? 0 : (b64KeyView ? KEY_WOW64_64KEY : KEY_WOW64_32KEY);
	}

	HKEY m_hRoot;
	CString m_strSubKey;
	CString m_strValue;
	REGSAM m_samDes;

	static BOOL IsRegKeyExist(CSomRegistry& params)
	{
		if (params.m_hRoot == NULL)
			return FALSE;

		HKEY hReg;
		if (RegOpenKeyEx(params.m_hRoot, params.m_strSubKey, 0, KEY_READ | params.m_samDes, &hReg) == ERROR_SUCCESS)
		{
			RegCloseKey(hReg);
			return TRUE;
		}
		return FALSE;
	}
	static BOOL RegEnum(CSomRegistry& params, vector<CString>& valueVec)
	{
		if (params.m_hRoot == NULL)
			return FALSE;

		BOOL bRet = FALSE;

		HKEY hKey = NULL;
		WCHAR szValue[0x1000];
		if (params.m_strValue.GetLength() == 0)
		{
			ULONG nChars = 0x1000;
			if (RegOpenKeyExW(params.m_hRoot, params.m_strSubKey, 0, KEY_ENUMERATE_SUB_KEYS | params.m_samDes, &hKey) == ERROR_SUCCESS)
			{
				for (int i = 0; RegEnumKeyEx(hKey, i, szValue, &nChars, NULL, NULL, NULL, NULL) == ERROR_SUCCESS; i++, nChars = 0x1000)
				{
					valueVec.push_back(szValue);
				}

				RegCloseKey(hKey);
				bRet = TRUE;
			}
		}
		else
		{
			if (RegOpenKeyExW(params.m_hRoot, params.m_strSubKey.GetLength() ? (params.m_strSubKey + L"\\" + params.m_strValue) : params.m_strValue, 0, KEY_QUERY_VALUE | params.m_samDes, &hKey) == ERROR_SUCCESS)
			{
				ULONG nChars = 0x1000;
				for (int i = 0; ::RegEnumValueW(hKey, i, szValue, &nChars, NULL, NULL, NULL, NULL) == ERROR_SUCCESS; i++, nChars = 0x1000)
				{
					valueVec.push_back(szValue);
				}

				RegCloseKey(hKey);
				bRet = TRUE;
			}
		}
		return bRet;
	}
	static BOOL RegRead(CSomRegistry& params, LPDWORD pdwType, LPVOID pvData, LPDWORD pcbData)
	{
		HKEY hKey = NULL;

		if (RegOpenKeyEx(params.m_hRoot, params.m_strSubKey, 0, KEY_QUERY_VALUE | params.m_samDes, &hKey) == ERROR_SUCCESS)
		{
			BOOL bRet = (::RegQueryValueEx(hKey, params.m_strValue, NULL, pdwType, (LPBYTE)pvData, pcbData) == ERROR_SUCCESS);

			RegCloseKey(hKey);

			return bRet;
		}
		return FALSE;
	}
	static BOOL RegRead(CSomRegistry& params, CString& strValue)
	{
		BYTE szValue[0x1000];
		ULONG cbData = 0x1000;
		DWORD dwType;

		if (RegRead(params, &dwType, szValue, &cbData) && (dwType == REG_EXPAND_SZ || dwType == REG_SZ))
		{
			strValue = (LPCWSTR)szValue;
			return TRUE;
		}

		return FALSE;
	}
	static BOOL RegRead(CSomRegistry& params, DWORD* pdwValue)
	{
		BYTE szValue[0x1000];
		ULONG cbData = 0x1000;
		DWORD dwType;

		if (RegRead(params, &dwType, szValue, &cbData) && (dwType == REG_DWORD))
		{
			*pdwValue = *(DWORD*)szValue;
			return TRUE;
		}

		return FALSE;
	}
	static BOOL RegWrite(CSomRegistry& params, DWORD dwType, LPVOID pvData, DWORD cbData)
	{
		HKEY hKey = NULL;

		if (RegCreateKeyEx(params.m_hRoot, params.m_strSubKey, 0, NULL, 0, KEY_WRITE | params.m_samDes, NULL, &hKey, NULL) == ERROR_SUCCESS)
		{
			BOOL bRet = (RegSetValueEx(hKey, params.m_strValue, 0, dwType, (LPBYTE)pvData, cbData) == ERROR_SUCCESS);

			RegCloseKey(hKey);

			return bRet;
		}
		return FALSE;
	}
	static BOOL RegWrite(CSomRegistry& params, LPCWSTR strValue, BOOL bExpand = FALSE)
	{
		return RegWrite(params, bExpand ? REG_EXPAND_SZ : REG_SZ, (PBYTE)strValue, wcslen(strValue) * 2 + 2);
	}
	static BOOL RegWrite(CSomRegistry& params, DWORD dwValue)
	{
		return RegWrite(params, REG_DWORD, (PBYTE)&dwValue, sizeof(DWORD));
	}
	static BOOL RegDeleteValue(CSomRegistry& params)
	{
		HKEY hKey = NULL;

		if (RegOpenKeyEx(params.m_hRoot, params.m_strSubKey, 0, KEY_SET_VALUE | params.m_samDes, &hKey) == ERROR_SUCCESS)
		{
			BOOL bRet = ::RegDeleteValue(hKey, params.m_strValue);

			RegCloseKey(hKey);

			return bRet;
		}
		return FALSE;
	}
	static BOOL RegDeleteKey(CSomRegistry& params)
	{
		typedef LSTATUS(WINAPI* PFNRegDeleteKeyEx)(HKEY, LPCWSTR, REGSAM, DWORD);
		static bool bInitialized = false;
		static PFNRegDeleteKeyEx pfnRegDeleteKeyEx = NULL;

		if (!(g_SomGlobal_bWin2k || bInitialized))
		{
			HMODULE hAdvapi32 = GetModuleHandle(L"Advapi32.dll");
			if (hAdvapi32 != NULL)
				pfnRegDeleteKeyEx = (PFNRegDeleteKeyEx)GetProcAddress(hAdvapi32, "RegDeleteKeyExW");

			bInitialized = true;
		}

		if (pfnRegDeleteKeyEx != NULL)
			return pfnRegDeleteKeyEx(params.m_hRoot, params.m_strSubKey, params.m_samDes, 0) == ERROR_SUCCESS;
		else
			return ::RegDeleteKey(params.m_hRoot, params.m_strSubKey) == ERROR_SUCCESS;
	}
	inline static HKEY ParserKeyName(LPCWSTR strKeyName)
	{
		struct keymap
		{
			LPCWSTR lpsz;
			HKEY hkey;
		};
		static const keymap map[] = {
			{ L"HKCR", HKEY_CLASSES_ROOT },
			{ L"HKCU", HKEY_CURRENT_USER },
			{ L"HKLM", HKEY_LOCAL_MACHINE },
			{ L"HKU",  HKEY_USERS },
			{ L"HKPD", HKEY_PERFORMANCE_DATA },
			{ L"HKDD", HKEY_DYN_DATA },
			{ L"HKCC", HKEY_CURRENT_CONFIG },
			{ L"HKEY_CLASSES_ROOT", HKEY_CLASSES_ROOT },
			{ L"HKEY_CURRENT_USER", HKEY_CURRENT_USER },
			{ L"HKEY_LOCAL_MACHINE", HKEY_LOCAL_MACHINE },
			{ L"HKEY_USERS", HKEY_USERS },
			{ L"HKEY_PERFORMANCE_DATA", HKEY_PERFORMANCE_DATA },
			{ L"HKEY_DYN_DATA", HKEY_DYN_DATA },
			{ L"HKEY_CURRENT_CONFIG", HKEY_CURRENT_CONFIG }
		};

		for (int i = 0; i < sizeof(map) / sizeof(keymap); i++)
		{
			if (!_wcsicmp(strKeyName, map[i].lpsz))
				return map[i].hkey;
		}
		return NULL;
	}

protected:
	inline static HKEY ParserKeyValueName(LPCWSTR strName, CString& szValueName, CString& szSubKeyName)
	{
		CString szName = strName;
		int len = szName.GetLength();

		if (strName[len - 1] != L'\\')
		{
			for (int i = len - 1; i >= 0; i--)
			{
				if (strName[i] == L'\\')
				{
					szValueName = &strName[i + 1];
					szName = szName.Left(i);
					break;
				}
			}
		}

		for (LPWSTR p = szName.GetBuffer(); *p; p++)
		{
			if (*p == L'\\' || *(p + 1) == 0)
			{
				*p = 0;
				szSubKeyName = p + 1;
				return ParserKeyName(szName);
			}
		}

		return NULL;
	}
};


namespace CSomPathParser
{

	// 判断文件是否存在,如果 pllFileSize 非空,并且值不为 -1,同时判断文件大小是否符合pllFileSize
	// 如果文件存在,但是大小不符,返回false,同时pllFileSize返回真正的大小
	inline bool IsFileExist(LPCWSTR pszFile, PLARGE_INTEGER pllFileSize = NULL)
	{
		//	WOW64FSREDIRECTIONCHECK(pszFile);

		bool bRet = false;
		if (pszFile == NULL || pszFile[0] == 0)
		{
			return bRet;
		}

		WIN32_FIND_DATA fd = { 0 };
		HANDLE hFile = FindFirstFile(pszFile, &fd);
		if (hFile != INVALID_HANDLE_VALUE)
		{
			FindClose(hFile);
			if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (pllFileSize == NULL || (pllFileSize->QuadPart == -1) || (pllFileSize->LowPart == fd.nFileSizeLow && pllFileSize->HighPart == fd.nFileSizeHigh)))
				bRet = true;

			if (pllFileSize)
			{
				pllFileSize->LowPart = fd.nFileSizeLow;
				pllFileSize->HighPart = fd.nFileSizeHigh;
			}
		}
		else if (pllFileSize)
			pllFileSize->QuadPart = 0;

		return bRet;
	}
} // namespace CSomPathParser


inline BOOL GetFileVersionFromRes(LPCTSTR lpFileFullPath, VS_FIXEDFILEINFO* pInfo)
{
	LPVOID pResVersionData = NULL;

	HMODULE hDll = LoadLibraryEx(lpFileFullPath, NULL,/*LOAD_LIBRARY_AS_IMAGE_RESOURCE|*/LOAD_LIBRARY_AS_DATAFILE); //不需要做RVA转换
	if (hDll)
	{
		HRSRC hRes = FindResource(hDll, (LPCTSTR)VS_VERSION_INFO, (LPCTSTR)RT_VERSION);
		if (hRes)
		{
			DWORD dwResLen = SizeofResource(hDll, hRes);

			HGLOBAL hGlobal = LoadResource(hDll, hRes);
			if (hGlobal)
			{
				LPVOID lpResLock = LockResource(hGlobal);
				if (lpResLock)
				{
					//读取资源中的版本信息
					pResVersionData = malloc(dwResLen);
					if (pResVersionData)
						memcpy(pResVersionData, lpResLock, dwResLen);

					UnlockResource(lpResLock);
				}

				FreeResource(hGlobal);
			}
		}

		FreeLibrary(hDll);
	}

	BOOL bRet = FALSE;

	if (pResVersionData)
	{
		//查询版本
		VS_FIXEDFILEINFO* pTmpInfo = NULL;
		unsigned int nInfoLen = 0;

		if (VerQueryValue(pResVersionData, _T("\\"), (void**)&pTmpInfo, &nInfoLen))
		{
			*pInfo = *pTmpInfo;
			bRet = TRUE;
		}

		free(pResVersionData);
	}

	return bRet;
}


inline BOOL GetFileVersion(LPCWSTR strFile, LARGE_INTEGER& lgVersion)
{
	//SomOutputDebugString(_T("开始判断文件版本号，文件:%s"), strFile);

	//BYTE  VersionBuffer[8192];
	DWORD dwVerSize;
	DWORD dwHandle;
	dwVerSize = GetFileVersionInfoSize(strFile, &dwHandle);
	if (dwVerSize == 0 /*|| dwVerSize > (sizeof(VersionBuffer) - 1)*/)
	{
		//SomOutputDebugString(_T("	从资源里读取文件版本号失败，无法判断文件版本号！"));
		return FALSE;
	}

	VS_FIXEDFILEINFO fileInfo;
	if (GetFileVersionFromRes(strFile, &fileInfo))
	{
		lgVersion.LowPart = fileInfo.dwFileVersionLS;
		lgVersion.HighPart = fileInfo.dwFileVersionMS;

		//SomOutputDebugString(_T("	从资源里读取文件版本号成功，版本号为：%d.%d.%d.%d"), HIWORD(lgVersion.HighPart), LOWORD(lgVersion.HighPart), HIWORD(lgVersion.LowPart), LOWORD(lgVersion.LowPart));

		return TRUE;
	}

	//SomOutputDebugString(_T("	从资源里读取文件版本号失败，无法判断文件版本号！"));

	return FALSE;
}


class CSomSystemEnvironment
{
public:
	int m_nEnvWindowVer;
	int m_nWindowMajorVer;
	int m_nWindowMinorVer;
	int m_nEnvWindowSPVer;
	int m_nEnvIEVer;
	int m_nEnvIESPVer;

	CString m_strEnvDirectX;
	CString m_strEnvJScript;
	CString m_strEnvMediaplayer;
	CString m_strEnvOutlookExpress;
	CString m_strEnvDataAccess;

	CString m_strEnvXMLCore;
	CString m_strEnvFramework;

	CSomSystemEnvironment()
		: m_nEnvWindowVer(0)
	{

	}
	static CSomSystemEnvironment* GetSystemEnvironment()
	{
		static CSomSystemEnvironment env;

		if (env.m_nEnvWindowVer == 0)
		{
			DWORD dwTick = GetTickCount();
			env.m_nEnvWindowVer = GetWindowFamily(env.m_nWindowMajorVer, env.m_nWindowMinorVer, env.m_nEnvWindowSPVer);
			env.m_nEnvIEVer = GetIEVer();
			env.m_nEnvIESPVer = GetIESPVer();
			env.m_strEnvDirectX = GetDirectXVer();
			env.m_strEnvJScript = GetWindowJScriptVer();
			env.m_strEnvMediaplayer = GetMediaplayerVer();
			env.m_strEnvOutlookExpress = GetOutlookExpressVer();
			env.m_strEnvDataAccess = GetDataAccessVer();
			env.m_strEnvXMLCore = GetXMLCoreServicesVer();
			env.m_strEnvFramework = GetDotNetFrameworkVer();

			//SomOutputDebugString(L"------- 获取系统信息 (%d ms) Version: %d (%d.%d sp%d), IE: %d.sp%d, DirectX: %s, JScript: %s, Mediaplayer: %s, OutlookExpress: %s, DataAccess: %s, XMLCore: %s, Framework: %s\n",
			//	GetTickCount() - dwTick,
			//	env.m_nEnvWindowVer,
			//	env.m_nWindowMajorVer,
			//	env.m_nWindowMinorVer,
			//	env.m_nEnvWindowSPVer,
			//	env.m_nEnvIEVer,
			//	env.m_nEnvIESPVer,
			//	env.m_strEnvDirectX,
			//	env.m_strEnvJScript,
			//	env.m_strEnvMediaplayer,
			//	env.m_strEnvOutlookExpress,
			//	env.m_strEnvDataAccess,
			//	env.m_strEnvXMLCore,
			//	env.m_strEnvFramework);
		}

		return &env;
	}

	enum WINVERSION
	{
		WIN_UNKNOWN = 0,
		WIN_2K,
		WIN_XP,
		WIN_VISTA,
		WIN_2K3,
		WIN_7,
		WIN_2008,
		WIN_2008R2,
		WIN_8,
		WIN_BLUE, //win8.1
		WIN_2012,
		WIN_2012R2,
		WIN_10,
	};
	static LPCWSTR FormatSystemName(DWORD dwSystem)
	{
		if (dwSystem == CSomSystemEnvironment::WIN_2K)
			return L"Win2k";
		else if (dwSystem == CSomSystemEnvironment::WIN_XP)
			return L"WinXP";
		else if (dwSystem == CSomSystemEnvironment::WIN_VISTA)
			return L"Vista";
		else if (dwSystem == CSomSystemEnvironment::WIN_2K3)
			return L"Win2003";
		else if (dwSystem == CSomSystemEnvironment::WIN_7)
			return L"Win7";
		else if (dwSystem == CSomSystemEnvironment::WIN_2008)
			return L"Win2008";
		else if (dwSystem == CSomSystemEnvironment::WIN_2008R2)
			return L"Win2008R2";
		else if (dwSystem == CSomSystemEnvironment::WIN_8)
			return L"Win8";
		else if (dwSystem == CSomSystemEnvironment::WIN_BLUE)
			return L"Blue";
		else if (dwSystem == CSomSystemEnvironment::WIN_2012)
			return L"Win2012";
		else if (dwSystem == CSomSystemEnvironment::WIN_2012R2)
			return L"Win2012R2";
		else if (dwSystem == CSomSystemEnvironment::WIN_10)
			return L"w10";
		else
			return L"Windows";
	}
	static DWORD GetSysCode(LPCWSTR strSys)
	{
		if (_wcsicmp(strSys, L"win2k") == 0)
			return WIN_2K;

		if (_wcsicmp(strSys, L"winxp") == 0)
			return WIN_XP;

		if (_wcsicmp(strSys, L"vista") == 0)
			return WIN_VISTA;

		if (_wcsicmp(strSys, L"win2k3") == 0)
			return WIN_2K3;

		if (_wcsicmp(strSys, L"win7") == 0)
			return WIN_7;

		if (_wcsicmp(strSys, L"2k8") == 0)
			return WIN_2008;

		if (_wcsicmp(strSys, L"2k8r2") == 0)
			return WIN_2008R2;

		if (_wcsicmp(strSys, L"win8") == 0)
			return WIN_8;

		if (_wcsicmp(strSys, L"blue") == 0)
			return WIN_BLUE;

		if (_wcsicmp(strSys, L"212") == 0)
			return WIN_2012;

		if (_wcsicmp(strSys, L"212r2") == 0)
			return WIN_2012R2;

		if (_wcsicmp(strSys, L"w10") == 0)
			return WIN_10;

		if (_wcsicmp(strSys, L"-1") == 0)
			return -1;

		return 0;
	}
	static WINVERSION GetWindowFamily(int& WindowMajorVer, int& WindowMinorVer, int& WindowSPVer)
	{
		WINVERSION wVersion = WIN_UNKNOWN;

		OSVERSIONINFOEX	osif = { 0 };
		osif.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

		typedef LONG(NTAPI* PFUN_RTLGETVERSION)(PRTL_OSVERSIONINFOW);
		RTL_OSVERSIONINFOEXW rtlVer = { 0 };
		rtlVer.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOEXW);
		PFUN_RTLGETVERSION pFuncRtlGetVersion = (PFUN_RTLGETVERSION)GetProcAddress(GetModuleHandle(TEXT("ntdll.dll")), "RtlGetVersion");
		if (pFuncRtlGetVersion)
		{
			pFuncRtlGetVersion((PRTL_OSVERSIONINFOW)&rtlVer);
			osif.dwMajorVersion = rtlVer.dwMajorVersion;
			osif.dwMinorVersion = rtlVer.dwMinorVersion;
			osif.wProductType = rtlVer.wProductType;
			osif.wServicePackMajor = rtlVer.wServicePackMajor;
			osif.wServicePackMinor = rtlVer.wServicePackMinor;
			_tcscpy(osif.szCSDVersion, rtlVer.szCSDVersion);
			osif.dwPlatformId = rtlVer.dwPlatformId;
			osif.dwBuildNumber = rtlVer.dwBuildNumber;
			osif.wSuiteMask = rtlVer.wSuiteMask;
		}
		else
		{
			GetVersionEx((LPOSVERSIONINFO)&osif);
		}

		SYSTEM_INFO info = { 0 };

		GetNativeSystemInfo(&info);

		WindowSPVer = osif.wServicePackMajor;

		int nSysVer = osif.dwMajorVersion * 10 + osif.dwMinorVersion;

		WCHAR pszSystemPath[MAX_PATH] = { 0 };
		LARGE_INTEGER liVersion;
		if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_SYSTEM, NULL, NULL, pszSystemPath)) &&
			PathAppend(pszSystemPath, _T("\\ntoskrnl.exe")) &&
			GetFileVersion(pszSystemPath, liVersion) &&
			(nSysVer < (HIWORD(liVersion.HighPart) * 10 + LOWORD(liVersion.HighPart))))
		{
			// 说明可能在兼容模式下运行，这个模式下通过GetVersionEx获得的版本都不正确
			osif.dwMajorVersion = HIWORD(liVersion.HighPart);
			osif.dwMinorVersion = LOWORD(liVersion.HighPart);

			WindowSPVer = 0;

			CString strCSDVersion, strPrefix(L"Service Pack ");
			CSomRegistry CSomRegistry_buffer(HKEY_LOCAL_MACHINE, L"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", L"CSDVersion");
			if (CSomRegistry::RegRead(CSomRegistry_buffer, strCSDVersion) && !strCSDVersion.IsEmpty())
			{
				if (_wcsnicmp(strCSDVersion, strPrefix, strPrefix.GetLength()) == 0)
					WindowSPVer = _wtoi((LPCWSTR)strCSDVersion + strPrefix.GetLength());
			}
		}

		WindowMajorVer = osif.dwMajorVersion;
		WindowMinorVer = osif.dwMinorVersion;

		if (osif.dwMajorVersion == 5 && osif.dwMinorVersion == 0)
		{
			wVersion = WIN_2K;
		}
		else if (osif.dwMajorVersion == 5 && osif.dwMinorVersion == 1)
		{
			wVersion = WIN_XP;
		}
		else if (osif.dwMajorVersion == 5 && osif.dwMinorVersion == 2)
		{
			if (osif.wProductType == VER_NT_WORKSTATION && info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
				wVersion = WIN_XP;
			else
				wVersion = WIN_2K3;
		}
		else if (osif.dwMajorVersion == 6 && osif.dwMinorVersion == 0)
		{
			if (osif.wProductType == VER_NT_SERVER || osif.wProductType == VER_NT_DOMAIN_CONTROLLER)
				wVersion = WIN_2008;
			else
				wVersion = WIN_VISTA;
		}
		else if (osif.dwMajorVersion == 6 && osif.dwMinorVersion == 1)
		{
			if (osif.wProductType == VER_NT_SERVER || osif.wProductType == VER_NT_DOMAIN_CONTROLLER)
				wVersion = WIN_2008R2;
			else
				wVersion = WIN_7;
		}
		else if (osif.dwMajorVersion == 6 && osif.dwMinorVersion == 2)
		{
			if (osif.wProductType == VER_NT_WORKSTATION)
				wVersion = WIN_8;
			else
				wVersion = WIN_2012;
		}
		else if (osif.dwMajorVersion == 6 && osif.dwMinorVersion == 3)
		{
			if (osif.wProductType == VER_NT_WORKSTATION)
				wVersion = WIN_BLUE;
			else
				wVersion = WIN_2012R2;
		}
		else if ((osif.dwMajorVersion == 6 && osif.dwMinorVersion == 4) ||
			(osif.dwMajorVersion == 10 && osif.dwMinorVersion == 0))
		{
			if (osif.wProductType == VER_NT_WORKSTATION)
				wVersion = WIN_10;
		}

		if (osif.wSuiteMask & VER_SUITE_EMBEDDEDNT)
		{
			wVersion = WIN_UNKNOWN;
		}

		return wVersion;
	}
	static DWORD GetIEVer()
	{
		CString strVersion;

		// 先读取svcVersion的值，若失败再读取Version
		CSomRegistry CSomRegistry_buffer1(HKEY_LOCAL_MACHINE, REGPATH_IE, REGVAL_IE_SVCVERSION, g_SomGlobal_bWin64 ? TRUE : FALSE);
		CSomRegistry CSomRegistry_buffer2(HKEY_LOCAL_MACHINE, REGPATH_IE, REGVAL_IE_VERSION, g_SomGlobal_bWin64 ? TRUE : FALSE);
		if ((CSomRegistry::RegRead(CSomRegistry_buffer1, strVersion) && !strVersion.IsEmpty())
			|| (CSomRegistry::RegRead(CSomRegistry_buffer2, strVersion) && !strVersion.IsEmpty()))
		{
			strVersion.Replace(L".", L"");
			strVersion = strVersion.Left(2);

			DWORD nVer = _wtoi(strVersion);
			if (nVer == 50)
			{
				CSomRegistry CSomRegistry_buffer(HKEY_LOCAL_MACHINE, REGPATH_IE2, REGVAL_IE2_VERSION, g_SomGlobal_bWin64 ? TRUE : FALSE);
				if (CSomRegistry::RegRead(CSomRegistry_buffer, strVersion))
				{
					strVersion.Replace(L".", L"");
					strVersion = strVersion.Left(2);

					return _wtoi(strVersion);
				}
			}
			return nVer;
		}

		return 0;
	}
	static DWORD GetIESPVer()
	{
		CString strVersion;
		CSomRegistry CSomRegistry_buffer(HKEY_LOCAL_MACHINE, REGPATH_IESP, REGVAL_MINOR_VERSION, TRUE);
		if (CSomRegistry::RegRead(CSomRegistry_buffer, strVersion))
		{
			strVersion.TrimLeft(L";");
			strVersion.TrimRight(L";");
			if (strVersion.GetLength() > 2)
			{
				strVersion = strVersion.Mid(2, 1);

				return _wtoi(strVersion);
			}
		}

		return 0;
	}
	static CString GetDotNetFrameworkVer()
	{
		CString strVer;
		vector<CString> ver;
		CSomRegistry CSomRegistry_buff(HKEY_LOCAL_MACHINE, REGPATH_DOTNET_FRAMEWORK);

		if (CSomRegistry::RegEnum(CSomRegistry_buff, ver))
		{
			for (vector<CString>::iterator iter = ver.begin(); iter != ver.end(); iter++)
			{
				CString strVerTemp = *iter;
				strVerTemp.Replace(L".", L"");
				strVerTemp = strVerTemp.Mid(1, 2);
				strVer += strVerTemp;
				strVer += "|";
			}
		}

		return strVer;
	}
	static CString GetWindowJScriptVer()
	{
		CString strVersion;
		CSomRegistry CSomRegistry_buffer(HKEY_LOCAL_MACHINE, REGPATH_WJSCRIPT, REGVAL_JS_VERSION, TRUE);
		if (CSomRegistry::RegRead(CSomRegistry_buffer, strVersion))
		{
			strVersion.Replace(L",", L"");
			strVersion = strVersion.Left(2);
			return strVersion;
		}

		return NULL;
	}
	static CString GetMediaplayerVer()
	{
		CString strVersion;
		CSomRegistry CSomRegistry_buffer(HKEY_LOCAL_MACHINE, REGPATH_MEDIAPLAYER, REGVAL_MEDIAPLAYER_VERSION, FALSE);
		if (CSomRegistry::RegRead(CSomRegistry_buffer, strVersion))
		{
			strVersion.Replace(L",", L"");
			strVersion = strVersion.Left(2);
			return strVersion;
		}

		return NULL;
	}

	static CString GetDirectXVer()
	{
		CString strVersion;
		WCHAR szSystemDirectory[MAX_PATH];
		GetSystemDirectory(szSystemDirectory, MAX_PATH);
		for (int i = 11; i >= 8; i--)
		{
			CString strD3D;
			strD3D.Format(L"%s\\d3d%d.dll", szSystemDirectory, i);
			if (CSomPathParser::IsFileExist(strD3D))
			{
				strVersion.Format(L"%d", i);
				break;
			}
		}
		return strVersion;
	}
	static CString GetOutlookExpressVer()
	{
		CString strVersion;
		CSomRegistry CSomRegistry_buffer(HKEY_LOCAL_MACHINE, REGPATH_OUTLOOKEXPRESS, REGVAL_OUTLOOKEXPRESS_VERSION, TRUE);
		if (CSomRegistry::RegRead(CSomRegistry_buffer, strVersion))
		{
			strVersion.Replace(L",", L"");
			strVersion = strVersion.Left(2);
			return strVersion;
		}

		return NULL;
	}
	static CString GetDataAccessVer()
	{
		CString strVersion;
		CSomRegistry CSomRegistry_buffer(HKEY_LOCAL_MACHINE, REGPATH_DATAACCESS, REGVAL_DATAACCESS_VERSION, TRUE);
		if (CSomRegistry::RegRead(CSomRegistry_buffer, strVersion))
		{
			strVersion.Replace(L".", L"");
			strVersion = strVersion.Left(3);
			return strVersion;
		}

		return NULL;
	}
	static CString GetXMLCoreServicesVer()
	{
		CString strVer;
		vector<CString> ver;
		CSomRegistry CSomRegistry_buffer(HKEY_LOCAL_MACHINE, REGPATH_XMLCORE, NULL);
		if (CSomRegistry::RegEnum(CSomRegistry_buffer, ver))
		{
			for (vector<CString>::iterator iter = ver.begin(); iter != ver.end(); iter++)
			{
				strVer += *iter;
				strVer += L"|";
			}
		}

		strVer.Replace(L".", L"");

		return strVer;
	}
	static BOOL IsWin7OrLater()
	{
		int WindowMajorVer = 0, WindowMinorVer = 0, nWindowVer = 0, nWindowSPVer = 0;
		nWindowVer = (CSomSystemEnvironment::WINVERSION)CSomSystemEnvironment::GetWindowFamily(WindowMajorVer, WindowMinorVer, nWindowSPVer);

		if (WindowMajorVer < 6)
			return FALSE;
		else if (WindowMajorVer == 6)
			return WindowMinorVer >= 1;
		else
			return TRUE;
	}
	static BOOL IsWin10OrLater()
	{
		int WindowMajorVer = 0, WindowMinorVer = 0, nWindowVer = 0, nWindowSPVer = 0;
		nWindowVer = (CSomSystemEnvironment::WINVERSION)CSomSystemEnvironment::GetWindowFamily(WindowMajorVer, WindowMinorVer, nWindowSPVer);

		if (WIN_10 == nWindowVer)
		{
			return TRUE;
		}

		return FALSE;
	}
	static BOOL IsVistaAndLater()
	{
		int WindowMajorVer = 0, WindowMinorVer = 0, nWindowVer = 0, nWindowSPVer = 0;
		nWindowVer = (CSomSystemEnvironment::WINVERSION)CSomSystemEnvironment::GetWindowFamily(WindowMajorVer, WindowMinorVer, nWindowSPVer);

		return (WindowMajorVer >= 6);
	}
	static BOOL IsCurPlatformMatched(__int64 iFieldValue)
	{
		int WindowMajorVer = 0, WindowMinorVer = 0, nWindowVer = 0, nWindowSPVer = 0;
		nWindowVer = (CSomSystemEnvironment::WINVERSION)CSomSystemEnvironment::GetWindowFamily(WindowMajorVer, WindowMinorVer, nWindowSPVer);
		__int64 iCurSysVer = (__int64)nWindowVer;

		if (iFieldValue & (1ull << (iCurSysVer - 1)))
		{
			return TRUE;
		}

		return FALSE;
	}
};