#pragma once

class CLogWriter
{
public:
	CLogWriter(void);
	~CLogWriter(void);

public:
	BOOL Init(LPCWSTR lpszModulePathName);


	BOOL Write(LPCWSTR format, ...); 

private:
	WCHAR m_szLogPathName[MAX_PATH];
	HANDLE m_hLogFile;

	BOOL   m_bInit;

};

extern CLogWriter  g_logger;
