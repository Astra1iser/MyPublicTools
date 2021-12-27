#include "stdafx.h"
#include "LogWriter.h"
#include <strsafe.h>

CLogWriter  g_logger;

CLogWriter::CLogWriter(void)
{
	m_bInit = FALSE;
	ZeroMemory(m_szLogPathName, sizeof(m_szLogPathName));
	m_hLogFile = INVALID_HANDLE_VALUE;
}

CLogWriter::~CLogWriter(void)
{
	if (m_hLogFile != INVALID_HANDLE_VALUE)
		CloseHandle(m_hLogFile);
}

BOOL CLogWriter::Init(LPCWSTR lpszLogName)
{
	if (!lpszLogName)
		return FALSE;

	m_bInit = TRUE;

	wcscpy(m_szLogPathName, lpszLogName);
	//wcscat(m_szLogPathName, L".log");

	m_hLogFile = CreateFile(m_szLogPathName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hLogFile == INVALID_HANDLE_VALUE)
	{
		OutputDebugString(L"创建日志文件失败!");
		return FALSE;
	}

	DWORD dwFileSizeHigh = 0;
	DWORD dwFileSizeLow = 0;

	dwFileSizeLow = GetFileSize(m_hLogFile, &dwFileSizeHigh);
	if (dwFileSizeHigh != 0  || dwFileSizeLow > 20 *1024 *1024)
	{
		SetFilePointer(m_hLogFile, 0, NULL, FILE_BEGIN);
		SetEndOfFile(m_hLogFile);
		dwFileSizeLow = 0;
	}
	if (dwFileSizeLow == 0)
	{
		DWORD tag = 0xFEFF;
		DWORD nHave = 0;
		WriteFile(m_hLogFile, &tag, 2, &nHave, NULL);
	}

	SetFilePointer(m_hLogFile, 0, 0, FILE_END);
	
	return TRUE;
}

BOOL CLogWriter::Write(LPCWSTR format, ...)
{
	if (!m_bInit)
	{
		WCHAR szLogPath[MAX_PATH] = {};
		WCHAR szModuleName[MAX_PATH] = {};
		WCHAR szAppName[MAX_PATH] = {};

		GetModuleFileName((HMODULE)&__ImageBase, szModuleName, MAX_PATH);
		PathStripPath(szModuleName);//dll名字

		GetModuleFileName(NULL, szAppName, MAX_PATH);
		PathStripPath(szAppName);//dll名字

		wcscat(szModuleName, L"-");
		wcscat(szModuleName, szAppName);
		

		SHGetSpecialFolderPath(NULL, szLogPath, CSIDL_COMMON_APPDATA,0);
		PathCombine(szLogPath, szLogPath, L"360Skylar6");
		PathCombine(szLogPath, szLogPath, szModuleName);
		wcscat(szLogPath, L".log");
		g_logger.Init(szLogPath);	
	}

	if (m_hLogFile == INVALID_HANDLE_VALUE)
		return FALSE;

	DWORD  nNumberOfBytesToWrite, nNumberOfBytesWritten;
	WCHAR szFlag[80] = {};
	SYSTEMTIME st = {};

	GetLocalTime(&st);

	StringCchPrintf(szFlag, 80, L"%04d-%02d-%02d %02d:%02d:%02d.%03d PID:%d TID:%d   ", st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond,st.wMilliseconds, GetCurrentProcessId(), GetCurrentThreadId());

	nNumberOfBytesToWrite = wcslen(szFlag) * sizeof(WCHAR);
	WriteFile(m_hLogFile, szFlag, nNumberOfBytesToWrite, &nNumberOfBytesWritten, NULL);

	
	CString var_str;

	va_list	ap;
	va_start(ap, format);
	    var_str.FormatV(format, ap);
	va_end(ap);

	DWORD dwFileSizeHigh = 0;
	DWORD dwFileSizeLow = 0;

	dwFileSizeLow = GetFileSize(m_hLogFile, &dwFileSizeHigh);
	if (dwFileSizeHigh != 0  || dwFileSizeLow > 10 *1024 *1024)
	{
		SetFilePointer(m_hLogFile, 2, NULL, FILE_BEGIN);
		SetEndOfFile(m_hLogFile);
	}
	
	nNumberOfBytesToWrite = var_str.GetLength() * sizeof(WCHAR);
	WriteFile(m_hLogFile, var_str, nNumberOfBytesToWrite, &nNumberOfBytesWritten, NULL);

	WriteFile(m_hLogFile, L"\r\n", 2*sizeof(WCHAR), &nNumberOfBytesWritten, NULL);

	return TRUE;
}