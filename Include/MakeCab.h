/*
* 如何使用CCabinet类制作cab文件
	CCabinet example;
	if (example.CreateCabinet(L"C:\\abc.cab"))	//创建cab缓存
	{
		if (example.AddFile2Cab(L"D:\\MyPublicTools\\MyPublicTools\\Release\\lock.exe"))	//向cab缓存中添加文件
		{
			example.FlushCab();	//将缓存转换为本地文件
		}
	}
*/


// MakeCab.h: interface for the CCabinet class.
// the  source code comes  from  EntAdmin  module
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CABINET_H__63932011_0E6F_48FA_90CC_386AB32BB218__INCLUDED_)
#define AFX_CABINET_H__63932011_0E6F_48FA_90CC_386AB32BB218__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <dos.h>
#include <sys/stat.h>

#include "fci.h"
#include "fdi.h"


/*
* 设定压缩后文件的大小，用于将大的文件压缩分割到几张软盘。
* 这里只是简单地压缩到一个文件，所以设为一个足够大的值。
*/
#define MEDIA_SIZE			1000000000

/*
* 当压缩文件达到此值时会自动FLUSH。
*/
#define FOLDER_THRESHOLD	900000

/*
* 压缩格式
*/
#define COMPRESSION_TYPE    tcompTYPE_MSZIP

class CCabinet  
{
public:
	//BOOL Decompress(char* cabName, char* destPath);
	//BOOL Compress(char* srcName, char* cabName);
	BOOL FlushCab();
	BOOL AddFile2Cab(LPCTSTR fileName, LPCTSTR shortName=NULL);
	BOOL CreateCabinet(LPCTSTR CABName);
	CCabinet();
	virtual ~CCabinet();

protected:
	HFCI m_hfci;                   // context
};

#endif // !defined(AFX_CABINET_H__63932011_0E6F_48FA_90CC_386AB32BB218__INCLUDED_)
