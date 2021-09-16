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
	//ֹͣ����(δʵ��)
	void StopDownload();
	//���ô���
	void SetAgent(LPCTSTR lpAgent);
	//���ó�ʱʱ��(ȫ��)
	void SetTimeout(DWORD dwTimeout);
	//������ϸ�ĳ�ʱʱ��
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
	DWORD	m_dwFileSizeUpperLimitation;//�����ļ��������(0-�����ƣ���0-�ļ���С����(��λ��Byte))

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
