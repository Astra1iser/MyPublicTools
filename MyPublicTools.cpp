#include "stdafx.h"

//HANDLE SimpleShellExec(CString lpFileName, LPCWSTR lpParam, CString lpDirectory, WORD wShowWindow)
//{
//#ifndef SEE_MASK_FORCENOIDLIST
//#define SEE_MASK_FORCENOIDLIST 0x1000
//#endif
//
//	SHELLEXECUTEINFO si;
//	ZeroMemory(&si, sizeof(si));
//	si.cbSize = sizeof(si);
//	si.lpFile = lpFileName;
//	si.lpParameters = lpParam;
//	si.lpDirectory = lpDirectory;
//	si.nShow = wShowWindow;
//	si.fMask = SEE_MASK_FLAG_NO_UI | SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FORCENOIDLIST;
//
//	ShellExecuteEx(&si);
//
//	return si.hProcess;
//}
//
//BOOL CALLBACK EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM Lparam)
//{
//	HWND hDefView = FindWindowEx(hwnd, 0, L"SHELLDLL_DefView", 0);
//	if (hDefView != NULL)
//	{
//		HWND hWorkerw = FindWindowEx(0, hwnd, L"WorkerW", 0);
//		ShowWindow(hWorkerw, SW_HIDE);
//		return FALSE;
//	}
//	return TRUE;
//}
//
//BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor,
//	HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
//{
//	static BOOL first = TRUE;   //标志
//	std::vector<CRect>* pRect = (std::vector<CRect>*)dwData;
//	//保存显示器信息
//	MONITORINFO monitorinfo;
//	monitorinfo.cbSize = sizeof(MONITORINFO);
//
//	//获得显示器信息，将信息保存到monitorinfo中
//	GetMonitorInfo(hMonitor, &monitorinfo);
//	//若检测到主屏
//	if (monitorinfo.dwFlags == MONITORINFOF_PRIMARY)
//	{
//		if (first)  //第一次检测到主屏
//		{
//			first = FALSE;
//			pRect->push_back(monitorinfo.rcMonitor);
//		}
//		else //第二次检测到主屏,说明所有的监视器都已经检测了一遍，故可以停止检测了
//		{
//			first = TRUE;    //标志复位
//			return FALSE;    //结束检测
//		}
//	}
//	else
//	{
//		pRect->push_back(monitorinfo.rcMonitor);
//	}
//	first = TRUE;     // 恢复主屏标记为初始状态
//	return TRUE;
//}
//
//void dongtaibizhidemo()
//{
//	HANDLE adc;
//
//	//https://github.com/BtbN/FFmpeg-Builds/releases
//	CString mypath = MyExePath(L"ffmpeg\\bin\\ffplay.exe");
//	
//	
//	
//	CRect* pRect = new CRect();
//	HWND hProgman = FindWindowW(L"Progman", 0);
//	
//	GetWindowRect(hProgman, pRect);
//	std::vector<CRect> vRect;
//	EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)&vRect);  // 枚举所有显示器的Rect
//	
//	auto DesktopInformation = vRect.begin();
//	
//	BOOL res = 1;
//	
//	for (; DesktopInformation != vRect.end(); DesktopInformation++)
//	{
//	
//		int nowx = DesktopInformation->right - DesktopInformation->left;
//		int nowy = (DesktopInformation->bottom - DesktopInformation->top);
//	
//		//cout << "左:" << DesktopInformation->right << " 上:" << DesktopInformation->top << endl;
//		if (DesktopInformation->top > pRect->top)
//			DesktopInformation->top = DesktopInformation->top - pRect->top;
//	
//		//LPCWSTR lpParameter = L"Comp1.mp4 -noborder -x 1920 -y 1080 -loop 0 -loglevel quiet -an";
//		CString lpParameter;
//		lpParameter.Format(L"Comp1.mp4 -noborder -x %d -y %d -loop 0 -loglevel quiet -an", nowx, nowy);
//	
//		if (1920 == nowy)
//		{
//			lpParameter.Format(L"Comp2.mp4 -noborder -x %d -y %d -loop 0 -loglevel quiet -an", nowx, nowy);
//		}
//	
//		BOOL abc = StartPrograme(mypath, adc, lpParameter, true, false);
//	
//		Wcout(lpParameter);
//	
//	
//	
//		if (abc)
//		{
//			Sleep(1000);
//			HWND hProgman = FindWindowW(L"Progman", 0);
//			SendMessageTimeout(hProgman, 0x52C, 0, 0, 0, 100, 0);
//			HWND hFfplay = FindWindowW(L"SDL_app", 0);
//			SetParent(hFfplay, hProgman);
//	
//			EnumWindows(EnumWindowsProc, 0);
//	
//	
//			SetWindowPos(hFfplay, HWND_TOP, DesktopInformation->left, DesktopInformation->top, nowx, nowy, SWP_SHOWWINDOW);
//	
//	
//		}
//	
//	
//		cout << DesktopInformation->left << " " << DesktopInformation->top << " " << nowx << " " << nowy << endl;
//	
//	
//	}
//}

//typedef BOOL(*pFun)(LPCTSTR);
using pFun = BOOL(*)(LPCTSTR);

void chaxunbudinghao()
{
	string aa;
	CString a;
	
	HMODULE  hDll;
	hDll = LoadLibrary((L"LeakDetect.dll"));
	if (NULL == hDll)
	{
		cout << "error1" << endl;
	}
	else
	{
	
		pFun myfunc;
		myfunc = (pFun)GetProcAddress(hDll, "IsPatchInstalled");
	
	
		if (!myfunc)
		{
			FreeLibrary(hDll);
			cout << "error2" << endl;
		}
		else
		{
			while (TRUE)
			{
				cout << "请输入补丁号(带KB/kb/Kb/kB开头)按回车结束:";
				cin >> aa;
				a = string2LPTSTR(aa);
				if (myfunc(a))
				{
					cout << "查询结果为: 存在"<< endl;
				}
				else
				{
					cout << "查询结果为: 不存在" << endl;
				}
				cout << endl;
			}
		}
	}
}




#include <shlobj.h>

#include <strsafe.h>

#define ARRSIZEOF(x) (sizeof(x)/sizeof(x[0]))

typedef struct _SpecialClsid
{
	LPCTSTR lpFolderName;
	int nFolderClsid;
} SPECIALCLSID, * LPSPECIALCLSID;

static SPECIALCLSID _SpecialFolder_Clsid[] =
{
	{ _T("WINDOWS"), CSIDL_WINDOWS },						// x:\Windows
	{ _T("SYSTEM"), CSIDL_SYSTEM },							// x:\Windows\System32
	{ _T("PROGRAMFILES"), CSIDL_PROGRAM_FILES },			// x:\Program files
	{ _T("DESKTOP"), CSIDL_DESKTOP },						// win7:C:\Users\jiyanmin\Desktop   xp:C:\Documents and Settings\YM\桌面
	{ _T("APPDATA"), CSIDL_APPDATA},						// win7:C:\Users\jiyanmin\AppData\Roaming   xp:C:\Documents and Settings\YM\Application Data
	{ _T("DOCUMENTS"), CSIDL_MYDOCUMENTS },					// win7:C:\Users\jiyanmin\Documents   xp:C:\Documents and Settings\YM\My Documents
	{ _T("PICTURES"), CSIDL_MYPICTURES },					// win7:C:\Users\jiyanmin\Pictures   xp:C:\Documents and Settings\YM\My Documents\My Pictures
	{ _T("COMMON_APPDATA"), CSIDL_COMMON_APPDATA },			// win7:C:\ProgramData   xp:C:\Documents and Settings\All Users\Application Data
	{ _T("COMMON_DOCUMENTS"), CSIDL_COMMON_DOCUMENTS }		// win7:C:\Users\Public\Documents   xp:C:\Documents and Settings\All Users\Documents

	//{ _T("PROGRAMS"), CSIDL_PROGRAMS },					// Start Menu\Programs
	//{ _T("PERSONAL"), CSIDL_PERSONAL },					// My Documents
	//{ _T("MYDOCUMENTS"), CSIDL_MYDOCUMENTS },			
	//{ _T("FAVORITES"), CSIDL_FAVORITES },				// <user name>\Favorites
	//{ _T("STARTUP"), CSIDL_STARTUP },					// Start Menu\Programs\Startup
	//{ _T("RECENT"), CSIDL_RECENT },						// <user name>\Recent
	//{ _T("SENDTO"), CSIDL_SENDTO },						// <user name>\SendTo
	//{ _T("STARTMENU"), CSIDL_STARTMENU },				// <user name>\Start Menu
	//{ _T("DESKTOPDIRECTORY"), CSIDL_DESKTOPDIRECTORY },	// <user name>\Desktop
	//{ _T("FONTS"), CSIDL_FONTS },						// windows\fonts
	//{ _T("COOKIES"), CSIDL_COOKIES },					// Cookies
	//{ _T("HISTORY"), CSIDL_HISTORY },					// History
	//{ _T("COMMONFILES"), CSIDL_PROGRAM_FILES_COMMON },	//
	//{ _T("COMMON_DESKTOPDIRECTORY"), CSIDL_COMMON_DESKTOPDIRECTORY },	// All Users\Desktop
	//{ _T("COMMON_FAVORITES"), CSIDL_COMMON_FAVORITES },	// 
	//{ _T("COMMON_PROGRAMS"), CSIDL_COMMON_PROGRAMS },	// All Users\Start Menu\Programs
	//{ _T("COMMON_STARTMENU"), CSIDL_COMMON_STARTMENU },	// All Users\Start Menu
	//{ _T("COMMON_STARTUP"), CSIDL_COMMON_STARTUP },		// All Users\Startup
};

//othres:
/*
%SystemDrive%   // C:
%TEMP%          // win7:C:\Users\jiyanmin\AppData\Local\Temp  xp: C:\Documents and Settings\YM\Local Settings\TEMP
*/

class FolderObject
{
public:

	static BOOL ExpendFolder(LPCTSTR lpPathIn, LPTSTR lpPathOut)
	{
		if (!lpPathIn || !lpPathOut)
			return FALSE;

		lpPathOut[0] = 0;

		TCHAR szShortPath[MAX_PATH] = {};

		if (lpPathIn[0] == '%')
		{
			LPTSTR lpCharEnd = StrChr(lpPathIn + 1, '%');
			if (lpCharEnd)
			{
				TCHAR szSpecial[MAX_PATH];
				StrCpyN(szSpecial, lpPathIn + 1, (static_cast<int>(lpCharEnd - lpPathIn)));

				if (GetSpecialFolder(szSpecial, szShortPath))
				{
					if (PathAppend(szShortPath, lpCharEnd + 1))
					{
						if (0 == GetLongPathName(szShortPath, lpPathOut, MAX_PATH))
						{
							StringCchCopy(lpPathOut, MAX_PATH, szShortPath);
						}

						return TRUE;
					}
				}
			}

			// path必须以一个%开始，而且只能有一个变量
			return FALSE;
		}

		StringCchCopy(szShortPath, MAX_PATH, lpPathIn);

		if (!GetLongPathName(szShortPath, lpPathOut, MAX_PATH))
			StringCchCopy(lpPathOut, MAX_PATH, szShortPath);

		return TRUE;
	}

	static BOOL GetSpecialFolder(LPCTSTR lpSpecialFolder, LPTSTR pszPath)
	{
		if (lpSpecialFolder == NULL || pszPath == NULL)
			return FALSE;

		pszPath[0] = '\0';

		// SHGetSpecialFolderPath
		for (int i = 0; i < ARRSIZEOF(_SpecialFolder_Clsid); ++i)
		{
			if (StrCmpI(_SpecialFolder_Clsid[i].lpFolderName, lpSpecialFolder) == 0)
			{
				if (SHGetSpecialFolderPath(NULL, pszPath, _SpecialFolder_Clsid[i].nFolderClsid, FALSE))
					return TRUE;
			}
		}

		if (StrCmpI(lpSpecialFolder, _T("SystemDrive")) == 0)
		{
			if (GetSystemWindowsDirectory(pszPath, MAX_PATH))
			{
				return PathStripToRoot(pszPath);
			}
			return FALSE;
		}

		if (StrCmpI(lpSpecialFolder, _T("TEMP")) == 0)
		{

			if (!GetTempPath(MAX_PATH - 1, pszPath))
				return FALSE;

			PathRemoveBackslash(pszPath);

			return TRUE;
		}


		return FALSE;
	}

	static CString GetSystemPath(LPCTSTR szAppendPath)
	{
		TCHAR szPath[MAX_PATH] = {};
		if (::GetSystemDirectory(szPath, MAX_PATH))
			PathAppend(szPath, szAppendPath);

		return szPath;
	}
private:

	static BOOL _RegGetString(HKEY hkey, LPCTSTR pszSubKey, LPCTSTR pszValue, LPTSTR szData, DWORD dwDataSize)
	{
		DWORD dwType = REG_SZ;
		return (ERROR_SUCCESS == SHGetValue(hkey, pszSubKey, pszValue, &dwType, szData, &dwDataSize));
	}

};




class CSingletonTest {
public:
	CSingletonTest() :a(1)
	{

	}

	void Init();
	int a;
};

typedef SimpleLoki::SingletonHolder<CSingletonTest> SingletonTest;
typedef SimpleLoki::SingletonHolder<CSingletonTest> SingletonTest2;





LRESULT CALLBACK HookCallWndProcForCrashWindow(int nCode, WPARAM wParam, LPARAM lParam)
{
	LPCWPRETSTRUCT lpMsg = (LPCWPRETSTRUCT)lParam;
	if (NULL != lpMsg)
	{
		if ((WM_INITDIALOG == lpMsg->message))
		{

			WCHAR szCaption[200] = { 0 };
			GetWindowText(lpMsg->hwnd, szCaption, 200);

			Wcout(L"窗口名是:%s", szCaption);
			MessageBox(NULL, szCaption, L"我是钩子", 0);
			goto _EXIT;
		}

	_EXIT:
		return CallNextHookEx(NULL, nCode, wParam, lParam);

	}
}




int main(int argc, _TCHAR* argv[])
{


	//char str[80] = "This is - www.runoob.com - website";
	//const char s[2] = "-";
	//char* token;

	///* 获取第一个子字符串 */
	//token = strtok(str, s);

	///* 继续获取其他的子字符串 */
	//while (token != NULL) {
	//	printf("%s\n", token);

	//	token = strtok(NULL, s);
	//}




	/*
	* 获取相对路径
	CString strFilePath = L"%desktop%";

	strFilePath.Trim(L' ');

	WCHAR szPath[MAX_PATH] = {};
	FolderObject::ExpendFolder(strFilePath.GetString(), szPath);
	CString m_strFile_path = szPath;

	Wcout(m_strFile_path);

	*/



	//vector<int> vecHandles = {1,2,3,4,5};
	//for(int handle : vecHandles)
	//{
	//	cout << handle;
	//}
	//HMODULE g_hModule = NULL;

	//HHOOK g_hHook = SetWindowsHookEx(WH_CALLWNDPROCRET, (HOOKPROC)HookCallWndProcForCrashWindow, g_hModule, 0);

	//system("pause");

	//if (g_hHook)
	//	UnhookWindowsHookEx(g_hHook);


	//CString srcIni = MyExePath(L"360EntExt.ini");
	//CString tgtIni = MyExePath(L"data\\inst\\360EntExt.ini");
	//if ((_waccess(srcIni.GetBuffer(), 0)) != -1)
	//{
	//	//取出模块
	//	WCHAR wch_srcmodules_buf[MAX_PATH * 10] = { 0 };
	//	WCHAR wch_tgtmodules_buf[MAX_PATH * 10] = { 0 };
	//	GetPrivateProfileString(_T("ext"), _T("modules"), _T(""), wch_srcmodules_buf, MAX_PATH * 10, srcIni);
	//	GetPrivateProfileString(_T("ext"), _T("modules"), _T(""), wch_tgtmodules_buf, MAX_PATH * 10, tgtIni);
	//	CString s_srcmodules_buf = wch_srcmodules_buf;
	//	CString s_tgtmodules_buf = wch_tgtmodules_buf;
	//	vector<CString> strSrcModuleArray;	//用户的自定义ini文件的模块
	//	vector<CString> strTgtModuleArray;	//默认ini文件中的模块
	//	vector<CString> strNeedPushArray;	//需要添加的模块
	//	vector<CString> strNeedEraseArray;	//需要删除的模块

	//	SplitString(s_srcmodules_buf, strSrcModuleArray, ',');
	//	SplitString(s_tgtmodules_buf, strTgtModuleArray, ',');

	//	vector<CString> strSrcModuleArray_Refactor = strSrcModuleArray; //删减过用户添加的新模块后的用户自定义模块(此时的元素为:默认ini中的模块-用户添加的模块)
	//	vector<CString> strFinalArray = strTgtModuleArray;	//最终的模块(结果:默认ini文件中的模块-用户删除的模块+用户添加的模块)

	//	//取出需要添加的元素
	//	for (vector<CString>::const_iterator iter = strSrcModuleArray.begin(); iter != strSrcModuleArray.end(); iter++)
	//	{
	//		CString buffer = *iter;
	//		vector<CString>::iterator result = find(strTgtModuleArray.begin(), strTgtModuleArray.end(), buffer); //查找

	//		if (result == strTgtModuleArray.end()) //没找到,证明用户添加了一个新的模块
	//		{
	//			strNeedPushArray.push_back(buffer);
	//		}
	//	}

	//	//接下来先去掉用户的ini中用户定义的新增模块,得到的数据与自带的ini中的模块比较,再从自带的ini中去除用户删除的模块
	//	for (vector<CString>::const_iterator iter = strNeedPushArray.begin(); iter != strNeedPushArray.end(); iter++)
	//	{
	//		CString buffer = *iter;
	//		vector<CString>::iterator result = find(strSrcModuleArray_Refactor.begin(), strSrcModuleArray_Refactor.end(), buffer); //查找
	//		if (result != strSrcModuleArray_Refactor.end())
	//		{
	//			strSrcModuleArray_Refactor.erase(result);
	//		}
	//		
	//	}

	//	//获取需要删除的元素
	//	for (vector<CString>::const_iterator iter = strTgtModuleArray.begin(); iter != strTgtModuleArray.end(); iter++)
	//	{
	//		CString buffer = *iter;
	//		vector<CString>::iterator result = find(strSrcModuleArray_Refactor.begin(), strSrcModuleArray_Refactor.end(), buffer); //查找

	//		if (result == strSrcModuleArray_Refactor.end()) //没找到,证明用户删除了一个旧的模块
	//		{
	//			strNeedEraseArray.push_back(buffer);
	//		}
	//	}

	//	//合成最终的结果
	//	//删除元素
	//	for (vector<CString>::const_iterator iter = strNeedEraseArray.begin(); iter != strNeedEraseArray.end(); iter++)
	//	{
	//		CString buffer = *iter;
	//		vector<CString>::iterator result = find(strFinalArray.begin(), strFinalArray.end(), buffer); //查找
	//		if (result != strFinalArray.end())
	//		{
	//			strFinalArray.erase(result);
	//		}

	//	}
	//	//添加元素
	//	for (vector<CString>::const_iterator iter = strNeedPushArray.begin(); iter != strNeedPushArray.end(); iter++)
	//	{
	//		CString buffer = *iter;
	//		strFinalArray.push_back(buffer);
	//	}

	//	CString finalvalue;

	//	for (vector<CString>::const_iterator iter = strFinalArray.begin(); iter != strFinalArray.end(); iter++)
	//	{
	//		SetIniValue(*iter, L"ver", L"1.0.0.1", srcIni);
	//		SetIniValue(*iter, L"lib", L"0", srcIni);

	//		finalvalue += *iter;
	//		if (iter != strFinalArray.end() - 1)
	//			finalvalue += ',';
	//	}

	//	for (vector<CString>::const_iterator iter = strNeedEraseArray.begin(); iter != strNeedEraseArray.end(); iter++)
	//	{
	//		SetIniValue(*iter, NULL, NULL, srcIni);
	//	}

	//	SetIniValue(L"ext", L"modules", finalvalue, srcIni);


	//	LPWSTR pSecNames = new WCHAR[2048];

	//	int nSectionNum = 0;
	//	wstring strSecName;

	//	DWORD retVal = GetPrivateProfileSectionNames(pSecNames, 2048, srcIni);

	//	if (retVal)
	//	{
	//		strSecName.assign(pSecNames, retVal);
	//	}
	//	delete pSecNames;

	//	CString allsession;
	//	for (int i = 0; i < strSecName.size(); i++)
	//	{
	//		if (strSecName[i] == '\0')
	//			allsession += ',';
	//		else
	//			allsession += strSecName[i];
	//	}

	//	vector<CString> strNeedCheckArray;
	//	SplitString(allsession, strNeedCheckArray, ',');

	//	for (vector<CString>::const_iterator iter = strNeedCheckArray.begin(); iter != strNeedCheckArray.end(); iter++)
	//	{
	//		CString buffer = *iter;
	//		if (L"ext" == buffer)
	//		{
	//			continue;
	//		}
	//		vector<CString>::iterator result = find(strFinalArray.begin(), strFinalArray.end(), buffer); //查找

	//		if (result == strFinalArray.end()) //没找到,证明用户已经删除了他之前自定义的模块,但是节点还没删掉
	//		{
	//			SetIniValue(*iter, NULL, NULL, srcIni);
	//		}
	//		
	//	}
	//}



}

