//Base������
#pragma once
#pragma warning(disable:4129)//���β���ת��ľ���
#pragma warning(disable:4996)
#pragma warning(disable:4267)
#pragma warning(disable:4311)
#pragma warning(disable:4302)


#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <regex>
#include <map>
#include <csignal>

//#include <afxwin.h>
#include <atlstr.h>
#include <windows.h>
#include <assert.h>
#include <atlconv.h>
#include <wininet.h>
#include <atlfile.h>


#include <ScopeLock.h>//���������ٽ�������װ��
#include <PathManager.h>//·����غ���
#include <RegeditManager.h>//ע��������غ���
#include <MyOperFile.h>//�ļ���д���ļ���Ϣ��غ���
#include <Thread.h>//�߳���/�̳߳�
#include <JsonHelper.h>//json������
#include <Mutex.h>//ϵͳȫ���� Mutex(����Ϊ�߳������ļ���)
#include <FIFO.h> //�ܵ���ʵ��
#include <SharedMemory.h>//�����ڴ����ʵ��
#include <IniManager.h>//ini������
#include <XmlManager.h>//XML������
#include <LogWriter.h>//��־�����
#include <Socket.h>//Socket������
#include <HttpPost.h>//ʵ��http��post����
#include <httpdown.h>


using namespace std;
using namespace PathManager;
using namespace RegeditManager;
using namespace FIFO;

//�������Ϊ�˹��windows�Դ���XMLDocument���ض���
#define XMLDocument tinyxml2::XMLDocument

//�������Ϊ�˷�ֹtinyxml2��д�뷽����д��xmlʱ������������� ʹ��_s��β�ĺ���ʱд������ݶ���UTF-8���������,����ʱ,��ʹ�ö�Ӧ�ı������,����û��ʹ��utf-8�����ķ���,����xml������ΪANSI��ʽ
#define SetText_UTF8(x) SetText(G2U(x))
#define NewElement_UTF8(x) NewElement(G2U(x))
#define SetAttribute_UTF8(x) SetAttribute(G2U(x))

#define WRITE_LOG(x,...)	g_logger.Write(x,__VA_ARGS__)

#ifndef _YCMBASE_H
#define _YCMBASE_H

//�ַ����� LPCSTR           LPCWSTR


/*
������Ŀ¼:
1.�жϵ�ǰ��¼���˻�����ͨ�û����ǹ���Ա																				BOOL IsAdmin()
2.��ǰϵͳ����																											LPCTSTR SetLangaueSyncOS()
3.һ������������̵ķ���																								BOOL StartPrograme(LPCTSTR Path, LPCTSTR Parameters = L"", BOOL IsAdmin =FALSE, BOOL IsWaitForSingle = TRUE)
4.��������Ԫ�ظ���																									int getArrayLen(Arr& array)
5.������Ԫ�صĶ�������																									void BinaryInsertSort
6.���ֲ���(�����������㷨 ����ʩ����)
7.˫ָ��ԭ��ȥ�������е�Ԫ��																							int DoublePointerRemoveValue(List& list, Value value)
8.��ȡIniPath��ini�ļ���Node�ڵ���Key��ֵ																				void GetIniValue(int& Source, LPCTSTR  Node, LPCTSTR Key, LPCTSTR IniPath)
																														void GetIniValue(LPCTSTR& Source, LPCTSTR Node, LPCTSTR Key, LPCTSTR IniPath)
9.����IniPath��ini�ļ���Node�ڵ���Key��ֵ(ʩ����)																				
10.�ı����� UTF-8��GB2312��ת��																							char* U2G(const char* utf8)
11.�ı����� GB2312��UTF-8��ת��																							char* G2U(const char* gb2312)
12.��wstringת��Ϊstring
13.��stringת��Ϊwstring
14.��LPCTSTRת��Ϊstring																								string LPCTSTR2string(LPCTSTR lpctstr)
15.������LPCTSRTƴ��																									LPCTSTR CombineLPCTSRT (T1 lpctstr1, T2 lpctstr2)
16.һ�����׵�ftp���ط���																								BOOL EasyDownLoadFile(LPCTSTR lpcszURL, LPCTSTR localFilePath)

17.
*/

namespace Base
{
	//1.�жϵ�ǰ��¼���˻�����ͨ�û����ǹ���Ա ����ֵ:1����Ա 0�ǹ���Ա
	BOOL IsAdmin();


	//2.���ص�ǰϵͳ����
	CString SetLangaueSyncOS();


	//3.һ������������̵ķ���,����1:·�� ����2:�������� ����3:�Ƿ����ԱȨ������ ����4:�Ƿ������߳�
	BOOL StartPrograme(LPCTSTR Path, LPCTSTR Parameters = L"", BOOL IsAdmin =FALSE, BOOL IsWaitForSingle = TRUE, _Out_opt_ HANDLE ProgrameHandle = NULL);


	//4.��������Ԫ�ظ���,����Ϊ�������������
	template <class Arr>
	int getArrayLen(Arr& array)
	{
		return (sizeof(array) / sizeof(array[0]));
	}


	//5.������Ԫ�صĶ�������(��С����),����Ϊ�������������,���������ԭ����˳��ı�,��ʱû�����β�������ָ�������
	template <class List>
	void BinaryInsertSort(List& list)
	{
		int j, mid;
		auto temp = list[0];

		for (int i = 0; i < getArrayLen(list); i++)
		{
			temp = list[i];
			int low = 0;
			int high = i - 1;
			while (low <= high)
			{
				mid = (low + high) / 2;
				if (list[mid] > temp)
					high = mid - 1;
				else
					low = mid + 1;
			}
			for (j = i - 1; j >= low; j--)
			{
				list[j + 1] = list[j];
			}
			list[low] = temp;
		}
	}


	//6.���ֲ���,��һ������Ϊ��Ҫ���ҵ�ֵ,�ڶ�������Ϊ���������,���������������Ҫ���ҵ�ֵ�Ĵ�С��������ĵ�һ���±�
/*	template <class Value, class List>
	int FindValueInList(Value value, List& list)
	{
		List& buffer = list;
		BinaryInsertSort(buffer);

		//vector<int>ValueNumber;
		int SumValue = 0;

		int low = 0, mid, find = 0;
		int high = getArrayLen(buffer)-1;
		while (low <= high)
		{
			mid = (low + high) / 2;
			if (buffer[mid] == value)
			{
				//ValueNumber.push_back(mid);
				return (mid + 1);
			}
			if (buffer[mid] > value)
				high = mid - 1;
			else
				low = mid + 1;
		}
		return 0;
	}
*/


	//7.˫ָ��ԭ��ȥ�������е�Ԫ��,����1Ϊ��������,����2Ϊ��Ҫȥ����Ԫ��ֵ,����ȥ���ĸ���,ȥ����Ԫ����������ĺ� "����ֵ����"��,�����ע��
	//��    int a[] = {2,7,5,2,4,2};
	//		int aa = DoublePointerRemoveValue(a, 2);
	//ִ�к������aΪ{7,5,4,2,2} aaֵΪ3
	template <class List,class Value>
	int DoublePointerRemoveValue(List& list, Value value)
	{
		int high = getArrayLen(list), low = 0;
		int i = 0;
		for (int j = 0; j < high; j++)
		{
			if (list[j] != value)
			{
				list[i] = list[j];
				i++;
			}
		}
		//for (int end = i; end < high; end++)
		//	list[end] = 0;
		return i;
	}


	//8.1��ȡIniPath��ini�ļ���Node�ڵ���Key��ֵ,������Ҫ��ֵ�����ó�Ա �˺���Ϊ���غ���,ֻ��ʶ��int��CString���͵�����
	void GetIniValue(int& Source, LPCTSTR  Node, LPCTSTR Key, LPCTSTR IniPath);
	//8.2��ȡIniPath��ini�ļ���Node�ڵ���Key��ֵ,������Ҫ��ֵ�����ó�Ա �˺���Ϊ���غ���,ֻ��ʶ��int��CString���͵�����
	void GetIniValue(CString& Source, LPCTSTR Node, LPCTSTR Key, LPCTSTR IniPath);

	void SetIniValue(LPCTSTR Node, LPCTSTR Key, LPCTSTR Value, LPCTSTR IniPath);



	string ReadFileCoding(CString FilePath);


	wstring UTF8ToUnicode(const char* strSrc);

	string UnicodeToAnsi(const WCHAR* strSrc);


	string UTF8ToAnsi(const char* strSrc);




	//10.�ı����� UTF-8��GB2312��ת��
	char* U2G(const char* utf8);


	//11.�ı����� GB2312��UTF-8��ת��
	char* G2U(const char* gb2312);




	void CharToWchar(const char* constCharString, TCHAR* outWchar);

	void WCharToChar(TCHAR* InWchar, char* OutStr);


	//12.��wstringת��Ϊstring
	string wstring2string(wstring wstr);


	//13.��stringת��Ϊwstring
	wstring string2wstring(string str);


	//14.��LPCTSTRת��Ϊstring
	string LPCTSTR2string(LPCTSTR lpctstr);


	//15.������LPCTSRTƴ��
	//����1:LPCTSRT����1
	//����2:LPCTSRT����2
	template<class T1, class T2>
	LPCTSTR CombineLPCTSRT (T1 lpctstr1, T2 lpctstr2)
	{
		return CString(lpctstr1) + CString(lpctstr2);
	}


	//16.һ�����׵�ftp���ط��� ע���뾡���ܵ�Я���������ӵ�Э��(http:\\|https:\\|ftp:\\),��Ȼ�������ᰴ��https,ftp,http��˳��������(�����������صĲ��ﲻͬ)
	//����1:���ص��ļ�url
	//����2:��ŵı���·�����ļ���
	BOOL EasyDownLoadFile(LPCTSTR lpcszURL, LPCTSTR localFilePath);


	//���CString�ַ���,����������
	void Wcout(CString string);


	// ����һ������ֵ���õ�����
	template<class T, class Y>
	auto funcExample(T& t, Y& y) -> decltype(t + y)
	{
		return t + y;
	}


	//decltype�������Զ��Ƶ�������
	template<class T>
	class classExample
	{
	private:
		decltype(T().begin()) m_it; //���� T()����˼�Ǵ������һ��T���͵Ķ���,�����decltypeҲ���Ƶ��� m_it��һ������������
	public:
		void print(T& t)
		{
			for (m_it = t.begin(); m_it != t.end(); m_it++)
				cout << *m_it << endl;
		}

	};

	//typedef int(*func) (double, string); //����һ��func�ĺ���ָ��  �����ķ���ֵ��int ����1������double����2������string
	//using func1 = int(*) (double, string); //ʹ��using���庯��ָ�� ������ϱ�����һ��

	//int mytest(double a, string b)
	//{
	//	return a;
	//}
	//������������� ���ǿ���ʹ�� func f = mytest;  f(10.1,"L");��ʹ�ú��� 


	


	//��ȡ�ļ��汾��
	CString GetFileVersion(LPCSTR filename);
	





	


	////����һ���ں���
	//HANDLE g_hMutex = NULL;
	//#define MUTEX_NAME	_T("Global//Q360LeakRepairMutex")
	//#define	WINDOW_CLASS			L"360EntNoticeMainClass"

	//BOOL IsInstanceExist()
	//{
	//	g_hMutex = CreateMutex(NULL, TRUE, MUTEX_NAME);
	//	WaitForSingleObject(g_hMutex, INFINITE);








	//	if (NULL != g_hMutex)
	//	{
	//		if (GetLastError() == ERROR_ALREADY_EXISTS)
	//		{
	//			HWND hWnd = ::FindWindow(WINDOW_CLASS, NULL);
	//			if (hWnd)
	//			{
	//				::ShowWindow(hWnd, SW_RESTORE);
	//				::SetForegroundWindow(hWnd);
	//			}
	//			return TRUE;
	//		}
	//	}

	//	return FALSE;
	//}


	
}

#endif

