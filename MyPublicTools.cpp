#include "stdafx.h"

HANDLE SimpleShellExec(CString lpFileName, LPCWSTR lpParam, CString lpDirectory, WORD wShowWindow)
{
#ifndef SEE_MASK_FORCENOIDLIST
#define SEE_MASK_FORCENOIDLIST 0x1000
#endif

	SHELLEXECUTEINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cbSize = sizeof(si);
	si.lpFile = lpFileName;
	si.lpParameters = lpParam;
	si.lpDirectory = lpDirectory;
	si.nShow = wShowWindow;
	si.fMask = SEE_MASK_FLAG_NO_UI | SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FORCENOIDLIST;

	ShellExecuteEx(&si);

	return si.hProcess;
}

BOOL CALLBACK EnumWindowsProc(_In_ HWND hwnd, _In_ LPARAM Lparam)
{
	HWND hDefView = FindWindowEx(hwnd, 0, L"SHELLDLL_DefView", 0);
	if (hDefView != NULL)
	{
		HWND hWorkerw = FindWindowEx(0, hwnd, L"WorkerW", 0);
		ShowWindow(hWorkerw, SW_HIDE);
		return FALSE;
	}
	return TRUE;
}

BOOL CALLBACK MonitorEnumProc(HMONITOR hMonitor,
	HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
{
	static BOOL first = TRUE;   //标志
	std::vector<CRect>* pRect = (std::vector<CRect>*)dwData;
	//保存显示器信息
	MONITORINFO monitorinfo;
	monitorinfo.cbSize = sizeof(MONITORINFO);

	//获得显示器信息，将信息保存到monitorinfo中
	GetMonitorInfo(hMonitor, &monitorinfo);
	//若检测到主屏
	if (monitorinfo.dwFlags == MONITORINFOF_PRIMARY)
	{
		if (first)  //第一次检测到主屏
		{
			first = FALSE;
			pRect->push_back(monitorinfo.rcMonitor);
		}
		else //第二次检测到主屏,说明所有的监视器都已经检测了一遍，故可以停止检测了
		{
			first = TRUE;    //标志复位
			return FALSE;    //结束检测
		}
	}
	else
	{
		pRect->push_back(monitorinfo.rcMonitor);
	}
	first = TRUE;     // 恢复主屏标记为初始状态
	return TRUE;
}

void dongtaibizhidemo()
{
	HANDLE adc;

	//https://github.com/BtbN/FFmpeg-Builds/releases
	CString mypath = MyExePath(L"ffmpeg\\bin\\ffplay.exe");
	
	
	
	CRect* pRect = new CRect();
	HWND hProgman = FindWindowW(L"Progman", 0);
	
	GetWindowRect(hProgman, pRect);
	std::vector<CRect> vRect;
	EnumDisplayMonitors(NULL, NULL, MonitorEnumProc, (LPARAM)&vRect);  // 枚举所有显示器的Rect
	
	auto DesktopInformation = vRect.begin();
	
	BOOL res = 1;
	
	for (; DesktopInformation != vRect.end(); DesktopInformation++)
	{
	
		int nowx = DesktopInformation->right - DesktopInformation->left;
		int nowy = (DesktopInformation->bottom - DesktopInformation->top);
	
		//cout << "左:" << DesktopInformation->right << " 上:" << DesktopInformation->top << endl;
		if (DesktopInformation->top > pRect->top)
			DesktopInformation->top = DesktopInformation->top - pRect->top;
	
		//LPCWSTR lpParameter = L"Comp1.mp4 -noborder -x 1920 -y 1080 -loop 0 -loglevel quiet -an";
		CString lpParameter;
		lpParameter.Format(L"Comp1.mp4 -noborder -x %d -y %d -loop 0 -loglevel quiet -an", nowx, nowy);
	
		if (1920 == nowy)
		{
			lpParameter.Format(L"Comp2.mp4 -noborder -x %d -y %d -loop 0 -loglevel quiet -an", nowx, nowy);
		}
	
		BOOL abc = StartPrograme(mypath, adc, lpParameter, true, false);
	
		Wcout(lpParameter);
	
	
	
		if (abc)
		{
			Sleep(1000);
			HWND hProgman = FindWindowW(L"Progman", 0);
			SendMessageTimeout(hProgman, 0x52C, 0, 0, 0, 100, 0);
			HWND hFfplay = FindWindowW(L"SDL_app", 0);
			SetParent(hFfplay, hProgman);
	
			EnumWindows(EnumWindowsProc, 0);
	
	
			SetWindowPos(hFfplay, HWND_TOP, DesktopInformation->left, DesktopInformation->top, nowx, nowy, SWP_SHOWWINDOW);
	
	
		}
	
	
		cout << DesktopInformation->left << " " << DesktopInformation->top << " " << nowx << " " << nowy << endl;
	
	
	}
}

typedef BOOL(*pFun)(LPCTSTR);

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

	system("pause");
}

