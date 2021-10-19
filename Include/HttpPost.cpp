#include "StdAfx.h"
#include <httppost.h>
 
//#include <atlhttp.h>
#include <WinInet.h>
#pragma comment(lib, "Wininet.lib")

#define HTTP_ACCEPT_TYPE	_T("*/*")
#define HTTP_ACCEPT			_T("Accept: */*\r\n")
#define HTTP_CONTENT_TYPE	_T("Content-Type: application/x-www-form-urlencoded\r\n")		// additional HTTP header

//对字符串做FORM编码
/*
函数参数：
strInput为输入串，不要包括url的非form部分
nInputLen为输入串的长度，-1表示串以 '\0' 结尾
strOutBuf为输出串缓冲区
nOutBufLen为输出缓冲区大小，包括结尾的 '\0'

  返回值：
输出串的长度，不包括结尾的0

  如果nOutBufLen的长度为0，则返回需要的输出缓冲区的大小，包括结尾的0
*/
int FormURLEncode(LPCSTR strInput, int nInputLen, char* strOutBuf, int nOutBufLen)
{
	int i;
	unsigned char cCur;
	char* pDst;
	int nDstLen;
	char strBuf[4];
	int nBufLen;

	if (nInputLen==-1)
		nInputLen=strlen(strInput);

	if (nOutBufLen==0)
	{
		nDstLen=0;
		for(i=0;i<nInputLen;i++)
		{
			cCur=strInput[i];
			if (cCur==' ')
				nDstLen++;
			else if (!isalnum(cCur))
				nDstLen+=3;
			else
				nDstLen++;
		}
		return nDstLen+1;
	}

	pDst=strOutBuf;
	nDstLen=0;
	for(i=0;i<nInputLen;i++)
	{
		cCur=strInput[i];
		if (cCur==' ')
		{
			*strBuf='+';
			*(strBuf+1)=0;
		}
		else if (!isalnum(cCur))
			sprintf(strBuf,"%%%2.2X",cCur);
		else
		{
			*strBuf=cCur;
			*(strBuf+1)=0;
		}
		nBufLen=strlen(strBuf);
		if (nBufLen+nDstLen>=nOutBufLen)
			break;
		strcpy(pDst,strBuf);
		nDstLen+=nBufLen;
		pDst+=nBufLen;
	}
	return strlen(strOutBuf);
}


VOID ParseURL( LPCTSTR szURL, LPTSTR szProtocol, LPTSTR szAddress, WORD &wPort, LPTSTR szURI )
{
#define MAX_URL_SIZE 4096
	DWORD dwPosition=0;
	BOOL bFlag=FALSE;

	while( _tcslen(szURL)>0 && dwPosition<_tcslen(szURL) && _tcsncmp((szURL+dwPosition), _T(":"), 1) )
		++dwPosition;

	if(!_tcsncmp((szURL+dwPosition+1), _T("/"), 1))	// is PROTOCOL
	{
		if(szProtocol)
		{
			_tcsncpy(szProtocol, szURL, dwPosition);
			szProtocol[dwPosition]=0;
		}
		bFlag=TRUE;
	}
	else
	{	// is HOST 
		if(szProtocol)
		{
			_tcsncpy(szProtocol, _T("http"), 4);
			szProtocol[5]=0;
		}
	}

	DWORD dwStartPosition=0;
	
	if(bFlag)
		dwStartPosition=dwPosition+=3;				
	else
		dwStartPosition=dwPosition=0;

	bFlag=FALSE;
	while(_tcslen(szURL)>0 && dwPosition<_tcslen(szURL) && _tcsncmp((szURL+dwPosition), _T("/"), 1))
			++dwPosition;

	DWORD dwFind=dwStartPosition;

	for( ; dwFind <= dwPosition; dwFind++ )
	{
		if(!_tcsncmp((szURL+dwFind), _T(":"), 1))// find PORT
		{ 
			bFlag=TRUE;
			break;
		}
	}

	if(bFlag)
	{
		TCHAR sztmp[MAX_URL_SIZE] = _T("");
		_tcsncpy( sztmp, (szURL+dwFind+1), dwPosition-dwFind) ;
		wPort = (WORD)_ttol( sztmp );
		_tcsncpy( szAddress, (szURL+dwStartPosition), dwFind-dwStartPosition );
	}
	else if(!_tcscmp(szProtocol, _T("https")))
	{
		wPort=INTERNET_DEFAULT_HTTPS_PORT;
		_tcsncpy(szAddress, (szURL+dwStartPosition), dwPosition-dwStartPosition);
	}
	else
	{
		wPort=INTERNET_DEFAULT_HTTP_PORT;
		_tcsncpy(szAddress, (szURL+dwStartPosition), dwPosition-dwStartPosition);
	}

	if(dwPosition<_tcslen(szURL))// find URI
		_tcsncpy(szURI, (szURL+dwPosition), _tcslen(szURL)-dwPosition);
	else
		szURI[0]=0;
	
	return;
}
#define ATL_HTTP_METHOD_POST _T("POST")

BOOL HttpPostForm(LPCTSTR szURL, LPVOID lpPostData, DWORD dwDataSize, CString& strResponse)
{
	if(szURL == NULL || lpPostData == NULL)
		return FALSE;

//	CUrl url;
//	if( url.CrackUrl(szURL) == FALSE )
//		return FALSE;

	HINTERNET hSession		= NULL;
	HINTERNET hConnect		= NULL;
	HINTERNET hRequest		= NULL;

	BOOL bRet = FALSE;
	BYTE buffer[4097];

	TCHAR szUri[4096] = _T("");
	TCHAR szHost[MAX_PATH] = _T("");
	TCHAR szPortocol[MAX_PATH] = _T("");
	WORD wPort;

	ParseURL( szURL, szPortocol, szHost, wPort, szUri );


	try
	{
		// 第一步：初始化 internet DLL 会话句柄，这是第一个被调用的函数
		hSession = InternetOpen( NULL, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );
		if ( hSession )
		{
			// 第二步：初始化 HTTP session 连接句柄
			hConnect = ::InternetConnect (
				hSession,				// 当前internet会话句柄
//				url.GetHostName() ,		// Server Name
				szHost,
//				url.GetPortNumber(),	// Server Port
				wPort,
				NULL,			// User Name
				NULL,			// Password
				INTERNET_SERVICE_HTTP,	// Type of service to access
				0, 0 );

			if ( hConnect )
			{
				////////////////////////////////////////////////////////////
				// 第三步：打开一个HTTP请求句柄
				DWORD dwFlags = INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_NO_CACHE_WRITE | INTERNET_FLAG_FORMS_SUBMIT;
				hRequest =:: HttpOpenRequest (
					hConnect,
					ATL_HTTP_METHOD_POST,	// HTTP Verb
//					url.GetUrlPath(),			// Object Name
					szUri,
					HTTP_VERSION,	// Version
					NULL,			// Reference
					NULL,			// Accept Type
					dwFlags,		// flag
					NULL			// context call-back point
					);

				if ( hRequest ) 
				{
					// 第四步：添加 HTTP 协议头
					bRet = ::HttpAddRequestHeaders(hRequest, HTTP_ACCEPT, sizeof(HTTP_ACCEPT)-1, HTTP_ADDREQ_FLAG_REPLACE|HTTP_ADDREQ_FLAG_ADD);
					if ( bRet )
					{
						// 第五步：发出请求 SEND REQUEST WITH POST ARGUEMENTS
						bRet = ::HttpSendRequest(
							hRequest,							// handle by returned HttpOpenRequest
							HTTP_CONTENT_TYPE,					// additional HTTP header
							sizeof(HTTP_CONTENT_TYPE)-1,	// additional HTTP header length
							lpPostData,							// additional data in HTTP Post or HTTP Put
							dwDataSize							// additional data length
							);


						// ???
						if ( FALSE == bRet && ERROR_HTTP_REDIRECT_NEEDS_CONFIRMATION == ::GetLastError() )
							bRet = TRUE;

						// 第六步：得到返回内容
						DWORD dwBytesRead;
						while(bRet)
						{
							bRet = InternetReadFile( hRequest, buffer, sizeof(buffer)-1, &dwBytesRead );
							if(dwBytesRead)
							{
								buffer[dwBytesRead] = 0;
								strResponse += (LPCTSTR)buffer;
							}
							else
								break;
						}
					}

					::InternetCloseHandle(hRequest);
					hRequest = NULL;
				}

				::InternetCloseHandle(hConnect);
				hConnect = NULL;
			}

			::InternetCloseHandle(hSession);
			hSession = NULL;
		}
	}
	catch (...)
	{
		if ( hRequest )
			::InternetCloseHandle(hRequest);

		if ( hConnect )
			::InternetCloseHandle(hConnect);

		if ( hSession )
			::InternetCloseHandle(hSession);

		bRet = FALSE;
	}

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
BOOL CHttpPost::AddParam(LPCTSTR lpKey, LPCTSTR lpValue, BOOL bEscape)
{
	USES_CONVERSION;

	if(lpKey == NULL)
		return FALSE;

	BOOL bRet = FALSE;
	LPCSTR lpStringIn = T2CA(lpValue);
	LPSTR lpStringOut = NULL;

	// escape
	if(lpValue)
	{
		DWORD dwStrLen = (DWORD)strlen(lpStringIn);
		if(dwStrLen)
		{
			if(bEscape)
			{
				// 三倍的BUFFER
				const int dwBufferSize = dwStrLen*4;
				lpStringOut = new CHAR[dwBufferSize];
				if(lpStringOut)
				{
					// bRet = AtlEscapeUrl(lpStringIn, lpStringOut, &dwStrLen, dwStrLen*2);
					int nRetLen = FormURLEncode(lpStringIn, dwStrLen, lpStringOut, dwBufferSize);
					if(nRetLen == 0)
					{
						delete []lpStringOut;
						lpStringOut = NULL;
						lpValue = NULL;
					}
					else
					{
						lpStringOut[nRetLen] = 0;
					}
				}
			}
			else
			{
				lpStringOut = NULL;
			}
		}
		else
		{
			lpValue = NULL;
		}
	}

	if(! m_szParam.IsEmpty())
		m_szParam += '&';

	m_szParam += lpKey;
	m_szParam += '=';

	if(lpStringOut)
	{
		m_szParam += A2CT(lpStringOut);
		delete []lpStringOut;
	}
	else if(lpValue)
	{
		m_szParam += lpValue;
	}

	return FALSE;
}

BOOL CHttpPost::Post(LPCTSTR lpURL)
{
	ATLASSERT(lpURL);
	USES_CONVERSION;

	m_szResponse.Empty();
	LPCTSTR lpPostData = m_szParam;
	DWORD dwLen = m_szParam.GetLength();

	return HttpPostForm(lpURL, (LPVOID)lpPostData, (DWORD)_tcslen(lpPostData), m_szResponse);
}

LPCTSTR CHttpPost::GetResponse()
{
	return m_szResponse;
}

BOOL CHttpPost::Clear()
{
	m_szParam.Empty();
	m_szResponse.Empty();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
