// HttpDownload.h: interface for the CHttpDownload class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTTPDOWNLOAD_H__87EB0DB1_D21E_4A19_9321_C94E34FA5DD5__INCLUDED_)
#define AFX_HTTPDOWNLOAD_H__87EB0DB1_D21E_4A19_9321_C94E34FA5DD5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <WinInet.h>

interface IHttpDownloadCallback
{
public:
	virtual HRESULT OnDownloadProgress(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode) = 0;
};

class CHttpDownload  
{
public:
	//停止下载(未实现)
	void StopDownload();
	//设置代理
	void SetAgent(LPCTSTR lpAgent);
	//设置超时时间(全局)
	void SetTimeout(DWORD dwTimeout);
	//设置详细的超时时间
	void SetTimeout(DWORD dwConnectTimeout, DWORD dwSendTimeout, DWORD dwReceiveTimeout);
	void SetCallback(IHttpDownloadCallback* pCallback);
	BOOL DownloadToFile(LPCTSTR lpURL, LPCTSTR lpFileName);
	DWORD DownloadToBuff(LPCTSTR lpURL, LPBYTE lpBuffer, DWORD dwBufSize);
	BOOL DownloadToTempFile(LPCTSTR lpURL, LPTSTR strFileName);
	DWORD GetNetStatus();
	BOOL Authorize(HWND hParentWnd, HINTERNET hRequest);
	void SetFileSizeUpperLimitation(DWORD dwSize);

	CHttpDownload();
	virtual ~CHttpDownload();

private:
	DWORD	m_dwConnectTimeout;
	DWORD	m_dwSendTimeout;
	DWORD	m_dwReceiveTimeout;
	IHttpDownloadCallback* m_pCallback;
	TCHAR	m_szAgent[MAX_PATH];
	BOOL	m_bStop;
	DWORD	m_dwNetStatus;
	DWORD	m_dwFileSizeUpperLimitation;//下载文件最大限制(0-无限制，非0-文件大小上限(单位：Byte))

protected:
	void NotifyCallback(ULONG ulProgress, ULONG ulProgressMax, ULONG ulStatusCode);
	void NotifyCallback(ULONG ulStatusCode);
	BOOL HttpReadFile(HINTERNET hRequest, HANDLE hFile);
	inline BOOL QueryContentLength(HINTERNET hInternet, DWORD& dwCode) const;
	inline BOOL QueryStatusCode(HINTERNET hInternet, DWORD& dwCode) const;
	BOOL QueryStatusNumber(HINTERNET hInternet, DWORD dwFlag, DWORD& dwCode) const;
	BOOL SetTimeouts(HINTERNET hInternet);
	BOOL MyCrackUrl(LPCTSTR lpszUrl, LPURL_COMPONENTS lpUrlComponents) const;
};

#endif // !defined(AFX_HTTPDOWNLOAD_H__87EB0DB1_D21E_4A19_9321_C94E34FA5DD5__INCLUDED_)
