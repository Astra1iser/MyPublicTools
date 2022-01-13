/*
* ���ʹ��CCabinet������cab�ļ�
	CCabinet example;
	if (example.CreateCabinet(L"C:\\abc.cab"))	//����cab����
	{
		if (example.AddFile2Cab(L"D:\\MyPublicTools\\MyPublicTools\\Release\\lock.exe"))	//��cab����������ļ�
		{
			example.FlushCab();	//������ת��Ϊ�����ļ�
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
* �趨ѹ�����ļ��Ĵ�С�����ڽ�����ļ�ѹ���ָ�������̡�
* ����ֻ�Ǽ򵥵�ѹ����һ���ļ���������Ϊһ���㹻���ֵ��
*/
#define MEDIA_SIZE			1000000000

/*
* ��ѹ���ļ��ﵽ��ֵʱ���Զ�FLUSH��
*/
#define FOLDER_THRESHOLD	900000

/*
* ѹ����ʽ
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
