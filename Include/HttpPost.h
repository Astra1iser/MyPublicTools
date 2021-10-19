#pragma once

//#include <atlbase.h>
//#include <atlapp.h>
//#include <atlmisc.h>
#include <atlconv.h>

BOOL HttpPostForm(LPCTSTR szURL, LPVOID lpPostData, DWORD dwDataSize, CString& strResponse);

class CHttpPost
{
public:
	BOOL AddParam(LPCTSTR lpKey, LPCTSTR lpValue, BOOL bEscape=FALSE);
	BOOL Post(LPCTSTR lpURL);
	LPCTSTR GetResponse();
	CString m_szResponse;
	BOOL Clear();

protected:
	CString m_szParam;

private:
};

//////////////////////////////////////////////////////////////////////////
/*
CHttpPost post;
post.AddParam( "name", "BBB" );
for( int i = 0; i < nCount;i ++ )
{
	post.AddParam( "title0", "AAA" );
	post.AddParam( "url0", "AAAA" );
}

post.Post( "http://www.qihoo.com/forum/import/multi_elite_add.html" );
*/
//http://www.qihoo.com/forum/import/multi_elite_add.html

