// HttpDownload.cpp: implementation of the CHttpDownload class.
//
//////////////////////////////////////////////////////////////////////
#pragma once
#include "stdafx.h"
#include "httpdown.h"
#include <atlconv.h>
#include <strsafe.h>
#include <shlobj.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#pragma comment(lib, "wininet.lib")

#define HTTPVERSION			_T("HTTP/1.1")

#define BUFFER_READ_SIZE 1024
#define DEFAULT_AGENT	_T("Mozila/4.0 (compatible; MSIE 5.0; LEAKCHECK)")

BOOL g_bHaveConnect = TRUE;

CHttpDownload::CHttpDownload() :
	m_dwConnectTimeout(NULL),
	m_dwSendTimeout(NULL),
	m_dwReceiveTimeout(NULL),
	m_bStop(FALSE),
	m_pCallback(NULL),
	m_dwNetStatus(0),
	m_dwFileSizeUpperLimitation(0)
{
	m_szAgent[0] = _T('\0');
}

CHttpDownload::~CHttpDownload()
{
}

#define ISOMACRO(h, o, t) ::InternetSetOption(h, o, &t, sizeof(DWORD));

BOOL CHttpDownload::SetTimeouts(HINTERNET hInternet)
{
	if(hInternet)
	{
		if(m_dwConnectTimeout)
			ISOMACRO(hInternet, INTERNET_OPTION_CONNECT_TIMEOUT, m_dwConnectTimeout);

		if(m_dwSendTimeout)
			ISOMACRO(hInternet, INTERNET_OPTION_SEND_TIMEOUT, m_dwSendTimeout);

		if(m_dwReceiveTimeout)
			ISOMACRO(hInternet, INTERNET_OPTION_RECEIVE_TIMEOUT, m_dwReceiveTimeout);
		
		return TRUE;
	}

	return FALSE;
}

void CHttpDownload::SetTimeout(DWORD dwTimeout)
{
	m_dwConnectTimeout = m_dwSendTimeout = m_dwReceiveTimeout = dwTimeout;	
}

void CHttpDownload::SetTimeout(DWORD dwConnectTimeout, DWORD dwSendTimeout, DWORD dwReceiveTimeout)
{
	m_dwConnectTimeout = dwConnectTimeout;
	m_dwSendTimeout = dwSendTimeout;
	m_dwReceiveTimeout = dwReceiveTimeout;
}

void CHttpDownload::SetAgent(LPCTSTR lpAgent)
{
	if(lpAgent)
		StringCbCopy(m_szAgent, sizeof(m_szAgent), lpAgent);
	else
		m_szAgent[0] = _T('\0');
}

BOOL CHttpDownload::QueryStatusNumber(HINTERNET hInternet, DWORD dwFlag, DWORD &dwCode) const
{
	dwCode = 0;
	DWORD dwSize = sizeof(DWORD);
	return HttpQueryInfo( hInternet, dwFlag | HTTP_QUERY_FLAG_NUMBER, &dwCode, &dwSize, NULL );

}

BOOL CHttpDownload::QueryStatusCode(HINTERNET hInternet, DWORD &dwCode) const
{
	return QueryStatusNumber(hInternet, HTTP_QUERY_STATUS_CODE, dwCode);
}

BOOL CHttpDownload::QueryContentLength(HINTERNET hInternet, DWORD &dwCode) const
{
	return QueryStatusNumber(hInternet, HTTP_QUERY_CONTENT_LENGTH, dwCode);
}

BOOL CHttpDownload::MyCrackUrl(LPCTSTR lpszUrl, LPURL_COMPONENTS lpUrlComponents) const
{
	if(lpszUrl == NULL || lpUrlComponents == NULL)
		return FALSE;

	lpUrlComponents->dwStructSize = sizeof(URL_COMPONENTS);
	
	lpUrlComponents->dwSchemeLength    = 1;
	lpUrlComponents->dwHostNameLength  = 1;
	lpUrlComponents->dwUserNameLength  = 1;
	lpUrlComponents->dwPasswordLength  = 1;
	lpUrlComponents->dwUrlPathLength   = 1;
	lpUrlComponents->dwExtraInfoLength = 1;
	
	lpUrlComponents->lpszScheme     = NULL;
	lpUrlComponents->lpszHostName   = NULL;
	lpUrlComponents->lpszUserName   = NULL;
	lpUrlComponents->lpszPassword   = NULL;
	lpUrlComponents->lpszUrlPath    = NULL;
	lpUrlComponents->lpszExtraInfo  = NULL;
	
	return InternetCrackUrl(lpszUrl, 0, 0, lpUrlComponents);
}

void CHttpDownload::SetCallback(IHttpDownloadCallback* pCallback)
{
	m_pCallback = pCallback;
}

HANDLE	g_hEventProxy = NULL;

BOOL CHttpDownload::DownloadToFile(LPCTSTR lpURL, LPCTSTR lpFileName)
{
	BOOL bRet = FALSE;

	static BOOL bFirstCall = TRUE;
	if (bFirstCall)
	{
		bFirstCall = FALSE;
		g_hEventProxy = CreateEvent(NULL, TRUE, TRUE, NULL);
		SetEvent(g_hEventProxy);
	}

	if (g_hEventProxy)
	{
		WaitForSingleObject(g_hEventProxy, INFINITE);
		ResetEvent(g_hEventProxy);
	}

	m_bStop = FALSE;
	m_dwNetStatus = 0;

	if (lpURL && !m_bStop)
	{
		HINTERNET hSession = NULL;
		HINTERNET hConnect = NULL;
		HINTERNET hRequest = NULL;

		URL_COMPONENTS URLparts;

		// 仅处理HTTP
		if (MyCrackUrl(lpURL, &URLparts) && URLparts.nScheme == INTERNET_SERVICE_HTTP)\
		{
			HANDLE hFile = INVALID_HANDLE_VALUE;
			if (lpFileName == NULL || INVALID_HANDLE_VALUE != (hFile = CreateFile(lpFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL)))
			{
				// 如果没有使用Proxy不能设置INTERNET_OPEN_TYPE_PROXY，否则在IE7下无法连接
				LPCTSTR lpAgent = m_szAgent[0] ? m_szAgent : DEFAULT_AGENT;
				hSession = ::InternetOpen(lpAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
				if(hSession && m_bStop == FALSE)
				{
					TCHAR szBuffer[INTERNET_MAX_HOST_NAME_LENGTH];

					SetTimeouts(hSession);
					NotifyCallback(BINDSTATUS_CONNECTING);

					StringCchCopyN(szBuffer, INTERNET_MAX_HOST_NAME_LENGTH, URLparts.lpszHostName, URLparts.dwHostNameLength);
					hConnect = InternetConnect(hSession, szBuffer, URLparts.nPort, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
					if(hConnect && m_bStop == FALSE)
					{
						DWORD dwFlags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_PRAGMA_NOCACHE;
						static LPCTSTR ppszAcceptTypes[2] =	{ _T("*/*"), NULL };

						hRequest = HttpOpenRequest(hConnect, NULL, URLparts.lpszUrlPath, HTTPVERSION, NULL, ppszAcceptTypes, dwFlags, 0);
						if(hRequest && m_bStop == FALSE)
						{
							BOOL bSend = HttpSendRequest(hRequest, NULL, 0, NULL, 0);

							QueryStatusCode(hRequest, m_dwNetStatus);

							BOOL IsProxyOnline = FALSE;
							if (m_dwNetStatus == HTTP_STATUS_DENIED || m_dwNetStatus == HTTP_STATUS_PROXY_AUTH_REQ)
							{
								IsProxyOnline = TRUE;
								Authorize(NULL, hRequest);
								bSend = HttpSendRequest(hRequest, NULL, 0, NULL, 0);
								QueryStatusCode(hRequest, m_dwNetStatus);
							}

							if (bSend && m_dwNetStatus == HTTP_STATUS_OK && m_bStop == FALSE )
							{
								bRet = HttpReadFile(hRequest, hFile);
							}
						}
					}
				}

				CloseHandle(hFile);
			}
			
			if (hRequest)
				::InternetCloseHandle(hRequest);
			
			if (hConnect)
				::InternetCloseHandle(hConnect);
			
			if (hSession)
				::InternetCloseHandle(hSession);
		}
	}

	if (g_hEventProxy)
	{
		SetEvent(g_hEventProxy);
		CloseHandle(g_hEventProxy);
		g_hEventProxy = NULL;
	}

	return bRet;
}

DWORD CHttpDownload::DownloadToBuff(LPCTSTR lpURL, LPBYTE lpBuffer, DWORD dwBufSize)
{
	DWORD dwAllBytes = 0;			// 下载总字节

	m_bStop = FALSE;
	m_dwNetStatus = 0;

	if (lpURL && !m_bStop)
	{
		HINTERNET hSession = NULL;
		HINTERNET hConnect = NULL;
		HINTERNET hRequest = NULL;

		URL_COMPONENTS URLparts;

		// 仅处理HTTP
		if (MyCrackUrl(lpURL, &URLparts) && URLparts.nScheme == INTERNET_SERVICE_HTTP)
		{
			// 如果没有使用Proxy不能设置INTERNET_OPEN_TYPE_PROXY，否则在IE7下无法连接
			LPCTSTR lpAgent = m_szAgent[0] ? m_szAgent : DEFAULT_AGENT;
			hSession = ::InternetOpen(lpAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
			if (hSession && m_bStop == FALSE)
			{
				TCHAR szBuffer[INTERNET_MAX_HOST_NAME_LENGTH] = {0};

				SetTimeouts(hSession);

				StringCchCopyN(szBuffer, INTERNET_MAX_HOST_NAME_LENGTH, URLparts.lpszHostName, URLparts.dwHostNameLength);
				hConnect = InternetConnect(hSession, szBuffer, URLparts.nPort, NULL, NULL, INTERNET_SERVICE_HTTP, 0, 0);
				if(hConnect && m_bStop == FALSE)
				{
					DWORD dwFlags = INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_PRAGMA_NOCACHE;

					static LPCTSTR ppszAcceptTypes[2] =	{_T("*/*"), NULL};

					hRequest = HttpOpenRequest(hConnect, NULL, URLparts.lpszUrlPath, HTTPVERSION, NULL, ppszAcceptTypes, dwFlags, 0);
					if (hRequest && m_bStop == FALSE)
					{
						BOOL bSend = HttpSendRequest(hRequest, NULL, 0, NULL, 0);

						QueryStatusCode(hRequest, m_dwNetStatus);

						BOOL IsProxyOnline = FALSE;
						if (m_dwNetStatus == HTTP_STATUS_DENIED || m_dwNetStatus == HTTP_STATUS_PROXY_AUTH_REQ)
						{
							IsProxyOnline = TRUE;
							Authorize(NULL, hRequest);
							bSend = HttpSendRequest(hRequest, NULL, 0, NULL, 0);
							QueryStatusCode(hRequest, m_dwNetStatus);
						}

						if (bSend && m_dwNetStatus == HTTP_STATUS_OK && m_bStop == FALSE)
						{
							// 试图取得文件长度
							DWORD dwLength = 0;
							BOOL bGetFilesize = FALSE;

							if (QueryContentLength(hRequest, dwLength))
							{
								bGetFilesize = TRUE;
							}

							DWORD dwReadSize = 0;
							BYTE lpReadBuff[BUFFER_READ_SIZE] = {0};
							BOOL bReadRet = FALSE;
							DWORD dwResidual = dwBufSize - 2;	// buff剩余字节

							while (m_bStop == FALSE)
							{
								bReadRet = ::InternetReadFile(hRequest, lpReadBuff, BUFFER_READ_SIZE, &dwReadSize);
								if (bReadRet && dwReadSize > 0)
								{
									if (dwResidual >= dwReadSize)
									{
										memcpy(lpBuffer + dwAllBytes, lpReadBuff, dwReadSize);
										dwResidual -= dwReadSize;
										
										lpBuffer[dwAllBytes + dwReadSize] = 0;
										lpBuffer[dwAllBytes + dwReadSize + 1] = 0;
									}
									else if (dwResidual > 0)
									{
										memcpy(lpBuffer + dwAllBytes, lpReadBuff, dwResidual);
										dwResidual = 0;
										
										lpBuffer[dwAllBytes + dwResidual] = 0;
										lpBuffer[dwAllBytes + dwResidual + 1] = 0;
									}

									dwAllBytes += dwReadSize;
								}
								else
								{
									break;
								}
							}
						}
					}
				}
			}
		}
		
		if (hRequest)
			::InternetCloseHandle(hRequest);
		if (hConnect)
			::InternetCloseHandle(hConnect);
		if (hSession)
			::InternetCloseHandle(hSession);
	}

	return dwAllBytes;
}

BOOL CHttpDownload::HttpReadFile(HINTERNET hRequest, HANDLE hFile)
{
	// 试图取得文件长度
	DWORD dwLength = 0;
	BOOL bGetFilesize = FALSE;
	
	if( QueryContentLength(hRequest, dwLength) )
	{
		bGetFilesize = TRUE;
		//文件长度异常（大于最大上限），则停止下载
		if (0 != m_dwFileSizeUpperLimitation && dwLength > m_dwFileSizeUpperLimitation)
		{
			return FALSE;
		}
	}
	
	DWORD dwBytesRead, dwBytesWrite;
	BYTE byBuffer[BUFFER_READ_SIZE];
	BOOL bReadRet;
	DWORD dwAllBytes = 0;

	NotifyCallback(BINDSTATUS_BEGINDOWNLOADDATA);
	int item = 0;
	while(m_bStop == FALSE)
	{
		if (!bGetFilesize)
		{
			if (QueryContentLength(hRequest, dwLength))
			{
				bGetFilesize = TRUE;
				if (0 != m_dwFileSizeUpperLimitation && dwLength > m_dwFileSizeUpperLimitation)
				{
					return FALSE;
				}
			}
		}
		bReadRet = ::InternetReadFile(hRequest, byBuffer, BUFFER_READ_SIZE, &dwBytesRead);
		if(bReadRet && dwBytesRead)
		{
			if(INVALID_HANDLE_VALUE != hFile)
				WriteFile(hFile, byBuffer, dwBytesRead, &dwBytesWrite, NULL);
			
			dwAllBytes += dwBytesRead;
			if( item % 10 == 0 )
				NotifyCallback( dwAllBytes, dwLength, BINDSTATUS_DOWNLOADINGDATA );

			item ++;
			if(bGetFilesize && dwAllBytes >= dwLength)
				break;
		}
		else
			break;
	}
	
	NotifyCallback(BINDSTATUS_ENDDOWNLOADDATA);

	return (m_bStop == FALSE);
}

void CHttpDownload::NotifyCallback(ULONG ulStatusCode)
{
	if (m_pCallback && m_bStop == FALSE)
	{
		m_pCallback->OnDownloadProgress(0, 0, ulStatusCode);
	}
}

void CHttpDownload::NotifyCallback(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode)
{
	if (m_pCallback && m_bStop == FALSE)
	{
		m_pCallback->OnDownloadProgress(ulProgress, ulProgressMax, ulStatusCode);
	}
}

BOOL MyGetTempFileName(LPTSTR lpFileName)
{
	TCHAR szTempPath[MAX_PATH] = {0};
	if (GetTempPath(MAX_PATH, szTempPath))
	{
		SHCreateDirectory(NULL, T2CW(szTempPath));
		if (GetTempFileName(szTempPath, _T("~DF"), 0, lpFileName))	// ~DF = Temp data file
		{
			return TRUE;
		}
	}
	
	return FALSE;
}

// 下载到临时文件
// strFileName : 为保存文件名准备的内存，大小为MAX_PATH
// strFileName不能为空
BOOL CHttpDownload::DownloadToTempFile(LPCTSTR lpURL, LPTSTR strFileName)
{
	m_dwNetStatus = 0;

	if(lpURL && strFileName)
	{
		if (MyGetTempFileName(strFileName))
		{
			if (DownloadToFile(lpURL, strFileName))
				return TRUE;

			DeleteFile(strFileName);
		}
	}

	return FALSE;
}

DWORD CHttpDownload::GetNetStatus()
{
	return m_dwNetStatus;
}

// 收到应答报文后，首先检查返回码，如果返回码为： HTTP_STATUS_DENIED或HTTP_STATUS_PROXY_AUTH_REQ，
// 则表示传输路径中有认证需求（不一定是代理服务器），此时需要执行下面函数：
// 然后重新调用HttpSendRequest()。由于WinINet中已经缓冲了接收到的认证信息，为了保险起见，我都是重新建立连接。

BOOL CHttpDownload::Authorize(HWND hParentWnd, HINTERNET hRequest)
{
	DWORD dwError = InternetErrorDlg(hParentWnd, hRequest, 
		ERROR_INTERNET_INCORRECT_PASSWORD, 
		FLAGS_ERROR_UI_FILTER_FOR_ERRORS | FLAGS_ERROR_UI_FLAGS_GENERATE_DATA | FLAGS_ERROR_UI_FLAGS_CHANGE_OPTIONS, 
		NULL);

	return dwError == ERROR_SUCCESS;
}

void CHttpDownload::SetFileSizeUpperLimitation(DWORD dwSize)
{
	m_dwFileSizeUpperLimitation = dwSize;
}