//Base������
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <regex>
#include <map>

#include <windows.h>
#include <assert.h>
#include <atlconv.h>
#include <atlstr.h>
#include <wininet.h>
#include <atlfile.h>

#include <ScopeLock.h>//���������ٽ�������װ��
#include <PathManager.h>//·����غ���
#include <RegeditManager.h>//ע��������غ���
#include <MyOperFile.h>//�ļ���д���ļ���Ϣ��غ���
#include <Thread.h>//�����߳���
#include <tinyxml2.h>//XML����һ�η�װ

using namespace std;
using namespace PathManager;
using namespace RegeditManager;
using namespace tinyxml2;

//�������Ϊ�˹��windows�Դ���XMLDocument���ض���
#define XMLDocument tinyxml2::XMLDocument

//�������Ϊ�˷�ֹtinyxml2��д�뷽����д��xmlʱ������������� ʹ��_s��β�ĺ���ʱд������ݶ���UTF-8���������,����ʱ,��ʹ�ö�Ӧ�ı������,����û��ʹ��utf-8�����ķ���,����xml������ΪANSI��ʽ
#define SetText_UTF8(x) SetText(G2U(x))
#define NewElement_UTF8(x) NewElement(G2U(x))
#define SetAttribute_UTF8(x) SetAttribute(G2U(x))


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
17.����һ����ֻ�и��ڵ��xml�ļ�																						XMLDocument* CreateEmptyXMLFile(const char* xmlPath, const char* rootNodeName)
18.����xml�ļ�																											XMLDocument* LoadXMLFile(const char* xmlPath)
19.����xml����																											BOOL SaveXMLFile(XMLDocument* doc, const char* xmlPath)
20.��ȡXML����																											BOOL GetXMLDeclaration(XMLDocument* doc, string& strDecl)
21.Ѱ��XML�е�ĳ���ڵ�																									BOOL FindXMLNode(XMLElement* pRoot, const string nodeName, XMLElement*& pNode, const char* Attribute = NULL, const char* AttributeValue = NULL)
																														BOOL FindXMLNode(XMLElement* pRoot, const string nodeName, XMLElement*& pNode, AttributeList& attributelist = NULL, AttributeValueList& attributevaluelist = NULL)
22.1����"�ڵ�ָ��"����"�ڵ�ָ��ͽڵ���"����"�ڵ�ָ��ͽڵ�����ּ���ڵ�ĳ��(ĳЩ)���Ե�ֵ"����ȡ�ýڵ��ı�			BOOL GetXMLNodeText(XMLElement* pRoot, const char*& text, const string nodeName = "", const char* Attribute = NULL, const char* AttributeValue = NULL)
																														BOOL GetXMLNodeText(XMLElement* pRoot, const char*& text, const string nodeName = "", AttributeList& attributelist = NULL, AttributeValueList& attributevaluelist = NULL)

23.
*/

namespace Base
{
	//1.�жϵ�ǰ��¼���˻�����ͨ�û����ǹ���Ա ����ֵ:1����Ա 0�ǹ���Ա
	BOOL IsAdmin();


	//2.���ص�ǰϵͳ����
	LPCTSTR SetLangaueSyncOS();


	//3.һ������������̵ķ���,����1:·�� ����2:�������� ����3:�Ƿ����ԱȨ������ ����4:�Ƿ������߳�
	BOOL StartPrograme(LPCTSTR Path, LPCTSTR Parameters = L"", BOOL IsAdmin =FALSE, BOOL IsWaitForSingle = TRUE);


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
	void GetIniValue(LPCTSTR& Source, LPCTSTR Node, LPCTSTR Key, LPCTSTR IniPath);


	//10.�ı����� UTF-8��GB2312��ת��
	char* U2G(const char* utf8);


	//11.�ı����� GB2312��UTF-8��ת��
	char* G2U(const char* gb2312);


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


	//17.����һ����ֻ�и��ڵ��xml�ļ�,���Զ�����ڵ�����,���ش�����xml��XMLDocument*ָ��,���ָ��������������Դ�����xml���ж�д����
	//����1:Ҫ������xml��·��
	//����2:���ڵ�����
	XMLDocument* CreateEmptyXMLFile(const char* xmlPath, const char* rootNodeName);


	//18.����xml�ļ�,����xml��XMLDocument*ָ��,���ָ��������������Դ�����xml���ж�д����
	//����:Ҫ���ص�xml��·��
	XMLDocument* LoadXMLFile(const char* xmlPath);
	

	//19.����xml����,�˷�����ͬ��SaveFile()֮�����ڴ˴��������ɹ�,���ͷ�docָ�벢����TRUE,������ʧ��,�򷵻�FALSE����ʱ���ͷ�docָ��
	BOOL SaveXMLFile(XMLDocument* doc, const char* xmlSavePath);


	//20.��ȡXML����
	BOOL GetXMLDeclaration(XMLDocument* doc, string& strDecl);


	//21.1����"�ڵ������"����"�ڵ�����ּ���ڵ�ĳ�����Ե�ֵ"��Ѱ�ҽڵ�(�������������)
	//����1 ��Ҫ��ѯ��xml��ĳ���ڵ�(�����������ڵ���ֵܽڵ��е��ӽڵ�)��ָ��(���ʵ�ڲ�ȷ��,������xml���ڵ��ָ��,���Ǵ�ʱ����д����4�Ͳ���5�Ա㾫ȷ����)
	//����2 ��Ҫ��ѯ�Ľڵ�����
	//����3 �����ѯ���˷��صĽڵ�ĵ�ַ,û�鵽��ԭ���õĵ�ַ����
	//����4 Ҫ��ѯ�Ľڵ��ĳ����������  (�˲����ɲ���)
	//����5 Ҫ��ѯ�Ľڵ��ĳ���������Ե�ֵ	(�˲����ɲ���)
	BOOL FindXMLNode(XMLElement* pRoot, const string nodeName, XMLElement*& pNode, const char* Attribute = NULL, const char* AttributeValue = NULL);


	//21.2����"�ڵ������"����"�ڵ�����ּ���ڵ�ĳЩ���Ե�ֵ"��Ѱ�ҽڵ�(��������Զ����Ե�node���о�ȷ��λ,ע�����4�Ͳ���5��һһ��Ӧ��ϵ)(�������������)
	//����1 ��Ҫ��ѯ��xml��ĳ���ڵ�(�����������ڵ���ֵܽڵ��е��ӽڵ�)��ָ��(���ʵ�ڲ�ȷ��,������xml���ڵ��ָ��,���Ǵ�ʱ����д����4�Ͳ���5�Ա㾫ȷ����)
	//����2 ��Ҫ��ѯ�Ľڵ�����
	//����3 �����ѯ���˷��صĽڵ�ĵ�ַ,û�鵽��ԭ���õĵ�ַ����
	//����4 Ҫ��ѯ�Ľڵ��ĳЩ�������� ����:(const char* attributelist[] = { "class","user" };) (�˲����ɲ���)
	//����5 Ҫ��ѯ�Ľڵ��ĳЩ�������Ե�ֵ ����:(const char* attributevaluelist[] = { "15��","ԭ����" };) (�˲����ɲ���)
	template <class AttributeList, class AttributeValueList>
	BOOL FindXMLNode(XMLElement* pRoot, const string nodeName, XMLElement*& pNode, AttributeList& attributelist = NULL, AttributeValueList& attributevaluelist = NULL)
	{
		string type = typeid(attributelist).name();
		BOOL islist = type.find("char const * ");
		
		if (!(!islist && getArrayLen(attributelist) == getArrayLen(attributevaluelist)))
			return FALSE;

		const char* value = pRoot->Value();
		if (strcmp(pRoot->Value(), nodeName.c_str()) == 0)
		{
			if (attributelist != NULL && attributevaluelist != NULL)
			{
				int j = 0;
				for (int i = 0; i < getArrayLen(attributelist); i++)
				{
					string source = pRoot->Attribute(attributelist[i]);
					string target = attributevaluelist[i];
					if (source == target)
					{
						j++;
						if (j == getArrayLen(attributelist))
						{
							pNode = pRoot;
							return TRUE;
						}
					}
				}
			}
			else
			{
				pNode = pRoot;
				return TRUE;
			}
		}

		XMLElement* p = pRoot;
		for (p = p->FirstChildElement(); p != NULL; p = p->NextSiblingElement())
		{
			if (FindXMLNode(p, nodeName, pNode, attributelist, attributevaluelist))
			{
				return TRUE;
			}
		}
		return FALSE;
	}


	//test
	BOOL FindXMLNode1(XMLElement* pRoot, const string nodeName, XMLElement*& pNode, map<const char*, const char*> Attribution = {});









































	//22.1����"�ڵ�ָ��"����"�ڵ�ָ��ͽڵ���"����"�ڵ�ָ��ͽڵ�����ּ���ڵ�ĳ�����Ե�ֵ"����ȡ�ýڵ��ı�(�������������)
	//����1 ��Ҫ��ѯ��xml��ĳ���ڵ�(�����������ڵ���ֵܽڵ��е��ӽڵ�)��ָ��(���ʵ�ڲ�ȷ��,������xml���ڵ��ָ��,���Ǵ�ʱ����д����4�Ͳ���5�Ա㾫ȷ����)
	//����2 �����ѯ���˷��ص����ֵ�const char* ָ��,û�鵽��ԭ���õĵ�ַ����
	//����3 ��Ҫ��ѯ�Ľڵ�����(���Ϊ����ֻ��ѯ����1�Ľڵ���ı�)
	//����4 Ҫ��ѯ�Ľڵ��ĳ���������� (�˲����ɲ���)
	//����5 Ҫ��ѯ�Ľڵ��ĳ���������Ե�ֵ (�˲����ɲ���)
	BOOL GetXMLNodeText(XMLElement* pRoot, const char*& text, const string nodeName = "", const char* Attribute = NULL, const char* AttributeValue = NULL);


	//22.2����"�ڵ�ָ��"����"�ڵ�ָ��ͽڵ���"����"�ڵ�ָ��ͽڵ�����ּ���ڵ�ĳЩ���Ե�ֵ"����ȡ�ýڵ��ı�(��������Զ����Ե�node���о�ȷ��λ,ע�����4�Ͳ���5��һһ��Ӧ��ϵ)(�������������)
	//����1 ��Ҫ��ѯ��xml��ĳ���ڵ�(�����������ڵ���ֵܽڵ��е��ӽڵ�)��ָ��(���ʵ�ڲ�ȷ��,������xml���ڵ��ָ��,���Ǵ�ʱ����д����4�Ͳ���5�Ա㾫ȷ����)
	//����2 �����ѯ���˷��ص����ֵ�const char* ָ��,û�鵽��ԭ���õĵ�ַ����
	//����3 ��Ҫ��ѯ�Ľڵ�����(���Ϊ����ֻ��ѯ����1�Ľڵ���ı�)
	//����4 Ҫ��ѯ�Ľڵ��ĳЩ�������� ����:(const char* attributelist[] = { "class","user" };) (�˲����ɲ���)
	//����5 Ҫ��ѯ�Ľڵ��ĳЩ�������Ե�ֵ ����:(const char* attributevaluelist[] = { "15��","ԭ����" };) (�˲����ɲ���)
	template <class AttributeList, class AttributeValueList>
	BOOL GetXMLNodeText(XMLElement* pRoot, const char*& text, const string nodeName = "", AttributeList& attributelist = NULL, AttributeValueList& attributevaluelist = NULL)
	{
		if (!pRoot)
		{
			return FALSE;
		}

		if ("" == nodeName)
		{
			text = pRoot->GetText();
			return TRUE;
		}

		XMLElement* pNode = NULL;
		if (FindXMLNode(pRoot, nodeName, pNode, attributelist, attributevaluelist))
		{		
			if (NULL != pNode)
			{
				text = pNode->GetText();
				return TRUE;
			}
			return FALSE;
		}
		else
		{
			return FALSE;
		}
	}


	//23.1����"�ڵ�ָ��"����"�ڵ�ָ��ͽڵ���"����"�ڵ�ָ��ͽڵ�����ּ���ڵ�ĳ�����Ե�ֵ"����ȡ�ڵ�����(�������������)
	//����1 ��Ҫ��ѯ��xml��ĳ���ڵ�(�����������ڵ���ֵܽڵ��е��ӽڵ�)��ָ��(���ʵ�ڲ�ȷ��,������xml���ڵ��ָ��,���Ǵ�ʱ����д����4�Ͳ���5�Ա㾫ȷ����)
	//����2 �����ѯ���˷������Ե�map<string, string>,û�鵽��ԭ���õ����ݲ���
	//����3 ��Ҫ��ѯ�Ľڵ�����(���Ϊ����ֻ��ѯ����1�Ľڵ������)(�������ѭһ��xml��ͬ���ڵ������������ͬ�Ľṹ������xml,��Ϊ�˷���ֻ�᷵�ز��ҵ��ĵ�һ��ͬ���ڵ����������ֵ,��������ȡĳЩͬ���ڵ�����Թ��ɵĻ�)
	//����4 Ҫ��ѯ�Ľڵ��ĳ���������� ((���һ�����Զ���֪����˲����ɲ���))
	//����5 Ҫ��ѯ�Ľڵ��ĳ���������Ե�ֵ (���һ�����Զ���֪����˲����ɲ���)
	BOOL GetXMLNodeAttribute(XMLElement* pRoot, map<string, string>& mapAttribute, const string nodeName = "", const char* Attribute = NULL, const char* AttributeValue = NULL);
	

	//23.1����"�ڵ�ָ��"����"�ڵ�ָ��ͽڵ���"����"�ڵ�ָ��ͽڵ�����ּ���ڵ�ĳЩ���Ե�ֵ"����ȡ�ڵ�����(�������������)
	//����1 ��Ҫ��ѯ��xml��ĳ���ڵ�(�����������ڵ���ֵܽڵ��е��ӽڵ�)��ָ��(���ʵ�ڲ�ȷ��,������xml���ڵ��ָ��,���Ǵ�ʱ����д����4�Ͳ���5�Ա㾫ȷ����)
	//����2 �����ѯ���˷������Ե�map<string, string>,û�鵽��ԭ���õ����ݲ���
	//����3 ��Ҫ��ѯ�Ľڵ�����(���Ϊ����ֻ��ѯ����1�Ľڵ������)(�������ѭһ��xml��ͬ���ڵ������������ͬ�Ľṹ������xml,��Ϊ�˷���ֻ�᷵�ز��ҵ��ĵ�һ��ͬ���ڵ����������ֵ,��������ȡĳЩͬ���ڵ�����Թ��ɵĻ�)
	//����4 Ҫ��ѯ�Ľڵ��ĳЩ�������� ����:(const char* attributelist[] = { "class","user" };) (���һ�����Զ���֪����˲����ɲ���)
	//����5 Ҫ��ѯ�Ľڵ��ĳЩ�������Ե�ֵ ����:(const char* attributevaluelist[] = { "15��","ԭ����" };) (���һ�����Զ���֪����˲����ɲ���)
	template <class AttributeList, class AttributeValueList>
	BOOL GetXMLNodeAttribute(XMLElement* pRoot,  map<string, string>& mapAttribute, const string nodeName = "", AttributeList& attributelist = NULL, AttributeValueList& attributevaluelist = NULL)
	{
		if (!pRoot)
		{
			return FALSE;
		}

		if ("" == nodeName)
		{
			const XMLAttribute* pAttr = NULL;
			for (pAttr = pRoot->FirstAttribute(); pAttr != NULL; pAttr = pAttr->Next())
			{
				string name = pAttr->Name();
				string value = pAttr->Value();
				mapAttribute.insert(make_pair(name, value));
			}
			return TRUE;
		}

		XMLElement* pNode = NULL;
		if (FindXMLNode(pRoot, nodeName, pNode, attributelist, attributevaluelist))
		{
			if (NULL != pNode)
			{
				const XMLAttribute* pAttr = NULL;
				for (pAttr = pNode->FirstAttribute(); pAttr != NULL; pAttr = pAttr->Next())
				{
					string name = pAttr->Name();
					string value = pAttr->Value();
					mapAttribute.insert(make_pair(name, value));
				}
			}
			return true;
		}
		return false;
	}


	//24.1�޸�(�������»򴴽�,������׷��,����׷�����Ȼ�ȡ���Լ�׷��Ȼ�����)�ڵ��ı�
	BOOL SetXMLNodeText(XMLDocument* doc, const char* xmlSavePath, XMLElement* pRoot, const string text, const string nodeName="", const char* Attribute = NULL, const char* AttributeValue = NULL);


	//24.2
	template <class AttributeList, class AttributeValueList>
	BOOL SetXMLNodeText(XMLDocument* doc, const char* xmlSavePath, XMLElement* pRoot, const string text, const string nodeName = "", AttributeList& attributelist = NULL, AttributeValueList& attributevaluelist = NULL)
	{
		if (NULL == pRoot)
		{
			return FALSE;
		}

		if ("" == nodeName)
		{
			try
			{

				XMLNode* pText = pRoot->LastChild();
				if (NULL != pText)
				{
					pText->SetValue(text.c_str());
				}
				else
				{
					//XMLText* pText = doc->NewText(text.c_str());
					//pRoot->InsertFirstChild(pText);
					pRoot->InsertNewText(text.c_str());
				}
			}
			catch (...)
			{
				//XMLText* pText = doc->NewText(text.c_str());
				//pRoot->InsertFirstChild(pText);
				pRoot->InsertNewText(text.c_str());
			}
			if (SaveXMLFile(doc, xmlSavePath))
			{
				return TRUE;
			}
			return FALSE;
		}

		XMLElement* pNode = NULL;
		if (FindXMLNode(pRoot, nodeName, pNode, attributelist, attributevaluelist))
		{
			try
			{
				XMLNode* pText = pNode->LastChild();
				if (NULL != pText)
				{
					pText->SetValue(text.c_str());
					//pNode->InsertAfterChild(pText, pText);
				}
				else
				{
					//XMLText* pText = doc->NewText(text.c_str());
					//pNode->InsertEndChild(pText);
					pNode->InsertNewText(text.c_str());
				}
			}
			catch (...)
			{
				/*XMLText* pText = doc->NewText(text.c_str());
				pNode->InsertFirstChild(pText);*/
				pNode->InsertNewText(text.c_str());
			}
			if (SaveXMLFile(doc, xmlSavePath))
			{
				return TRUE;
			}
		}
		return FALSE;
	}

	//25.1 �޸�(�������»򴴽�)�ڵ�����
	BOOL SetXMLNodeAttribution(XMLDocument* doc, const char* xmlSavePath, XMLElement* pRoot, map<string, string>& mapAttribute, const string nodeName = "", const char* Attribute = NULL, const char* AttributeValue = NULL);

	//25.2
	template <class AttributeList, class AttributeValueList>
	BOOL SetXMLNodeAttribution(XMLDocument* doc, const char* xmlSavePath, XMLElement* pRoot, map<string, string>& mapAttribute, const string nodeName = "", AttributeList& attributelist = NULL, AttributeValueList& attributevaluelist = NULL)
	{
		if (NULL == pRoot)
		{
			return FALSE;
		}

		if ("" == nodeName)
		{
			const XMLAttribute* pAttr = pRoot->FirstAttribute();

			if (NULL == pAttr)
			{
				for (auto it = mapAttribute.begin(); it != mapAttribute.end(); ++it)
				{
					if ("" != it->first.c_str())
					{
						if ("" != it->second.c_str())
						{
							pRoot->SetAttribute(it->first.c_str(), it->second.c_str());
						}
						else
						{
							return FALSE;
						}
					}
					else
					{
						return FALSE;
					}
				}
			}

			char* strName = NULL;
			for (; pAttr != NULL; pAttr = pAttr->Next())
			{
				strName = const_cast<char*>(pAttr->Name());
				map<string, string> isnofind = {};
				for (auto it = mapAttribute.begin(); it != mapAttribute.end(); ++it)
				{
					if (strName == it->first)
					{
						if ("" != it->second.c_str())
						{
							pRoot->SetAttribute(strName, it->second.c_str());
						}
						else
						{
							pRoot->DeleteAttribute(strName);
						}
					}
					else
					{
						auto pr = std::make_pair(it->first, it->second);
						isnofind.insert(pr);
					}

				}

				if (0 != isnofind.size())
				{
					for (auto it = isnofind.begin(); it != isnofind.end(); ++it)
					{

						if ("" != it->first.c_str())
						{
							if ("" != it->second.c_str())
							{
								pRoot->SetAttribute(it->first.c_str(), it->second.c_str());
							}
							else
							{
								return FALSE;
							}
						}
						else
						{
							return FALSE;
						}
					}
				}
			}

			if (SaveXMLFile(doc, xmlSavePath))
			{
				return TRUE;
			}
			return FALSE;
		}

		XMLElement* pNode = NULL;
		if (FindXMLNode(pRoot, nodeName, pNode, attributelist, attributevaluelist))
		{
			const XMLAttribute* pAttr = pNode->FirstAttribute();

			if (NULL == pAttr)
			{
				for (auto it = mapAttribute.begin(); it != mapAttribute.end(); ++it)
				{
					if ("" != it->first.c_str())
					{
						if ("" != it->second.c_str())
						{
							pNode->SetAttribute(it->first.c_str(), it->second.c_str());
						}
						else
						{
							return FALSE;
						}
					}
					else
					{
						return FALSE;
					}
				}
			}

			char* strName = NULL;
			for (; pAttr != NULL; pAttr = pAttr->Next())
			{
				strName = const_cast<char*>(pAttr->Name());
				map<string, string> isnofind = {};
				for (auto it = mapAttribute.begin(); it != mapAttribute.end(); ++it)
				{
					if (strName == it->first)
					{
						if ("" != it->second.c_str())
						{
							pNode->SetAttribute(strName, it->second.c_str());
						}
						else
						{
							pNode->DeleteAttribute(strName);
						}
					}
					else
					{
						auto pr = std::make_pair(it->first, it->second);
						isnofind.insert(pr);
					}

				}

				if (0 != isnofind.size())
				{
					for (auto it = isnofind.begin(); it != isnofind.end(); ++it)
					{

						if ("" != it->first.c_str())
						{
							if ("" != it->second.c_str())
							{
								pNode->SetAttribute(it->first.c_str(), it->second.c_str());
							}
							else
							{
								return FALSE;
							}
						}
						else
						{
							return FALSE;
						}
					}
				}
			}

			if (SaveXMLFile(doc, xmlSavePath))
			{
				return TRUE;
			}
			return FALSE;
		}
		return FALSE;
	}

	//26.1�ڽڵ��²����ӽڵ�
	BOOL SetXMLNewNode(XMLDocument* doc, const char* xmlSavePath, XMLElement* pRoot, const char* newNodeName, map<string, string>newMapAttribute = {}, const char* newText = "", const string nodeName = "", const char* Attribute = NULL, const char* AttributeValue = NULL);

	//26.2
	template <class AttributeList, class AttributeValueList>
	BOOL SetXMLNewNode(XMLDocument* doc, const char* xmlSavePath, XMLElement* pRoot, const char* newNodeName, map<string, string>newMapAttribute = {}, const char* newText = "", const string nodeName = "", AttributeList& attributelist = NULL, AttributeValueList& attributevaluelist = NULL)
	{
		if (NULL == pRoot)
		{
			return FALSE;
		}
		if ("" == nodeName)
		{
			if ("" != newNodeName)
			{
				XMLElement* pNewNode = pRoot->InsertNewChildElement(newNodeName);
				if (0 != newMapAttribute.size())
				{
					SetXMLNodeAttribution(doc, xmlSavePath, pNewNode, newMapAttribute);
				}
				if ("" != newText)
				{
					SetXMLNodeText(doc, xmlSavePath, pNewNode, newText);
				}

				if (SaveXMLFile(doc, xmlSavePath))
				{
					return TRUE;
				}
			}
			return FALSE;
		}

		XMLElement* pNode = NULL;
		if (FindXMLNode(pRoot, nodeName, pNode, attributelist, attributevaluelist))
		{
			if ("" != newNodeName)
			{
				XMLElement* pNewNode = pNode->InsertNewChildElement(newNodeName);
				if (0 != newMapAttribute.size())
				{
					SetXMLNodeAttribution(doc, xmlSavePath, pNewNode, newMapAttribute);
				}
				if ("" != newText)
				{
					SetXMLNodeText(doc, xmlSavePath, pNewNode, newText);
				}

				if (SaveXMLFile(doc, xmlSavePath))
				{
					return TRUE;
				}
			}
			else
			{
				return FALSE;
			}
		}
		return FALSE;
	}

	//27.1ɾ���ڵ�
	BOOL DeleteXMLNode(XMLDocument* doc, const char* xmlSavePath, XMLElement* pRoot, const string nodeName = "", const char* Attribute = NULL, const char* AttributeValue = NULL);
	
	//27.2
	template <class AttributeList, class AttributeValueList>
	BOOL DeleteXMLNode(XMLDocument* doc, const char* xmlSavePath, XMLElement* pRoot, const char* delNodeName, const string nodeName = "", AttributeList& attributelist = NULL, AttributeValueList& attributevaluelist = NULL)
	{
		if (NULL == pRoot)
		{
			return FALSE;
		}
		if ("" == nodeName)
		{
			//pRoot->DeleteChild(NodeName); //ɾ��ָ���ڵ�
			pRoot->DeleteChildren();//ɾ���ڵ�����к��ӽڵ�


			if (SaveXMLFile(doc, xmlSavePath))
			{
				return TRUE;
			}
			return FALSE;
		}
		XMLElement* pNode = NULL;
		if (FindXMLNode(pRoot, nodeName, pNode, attributelist, attributevaluelist))
		{

			//pRoot->DeleteChild(NodeName); //ɾ��ָ���ڵ�
			pNode->DeleteChildren();//ɾ���ڵ�����к��ӽڵ�


			if (SaveXMLFile(doc, xmlSavePath))
			{
				return TRUE;
			}
			return FALSE;
		}
		return FALSE;
	}

	//27.3
	BOOL DeleteXMLNode(XMLElement* fatherNode, XMLElement* childrenNode);

	//���ﱾ����Ҫ��һ����װxml�Ļ�������  ��ʱ�ȷ��� ���������

	//class XMLManager
	//{
	//	public:

	//		XMLManager(const char* xmlPath)
	//		{
	//			InitializeSRWLock(&g_srwLock);//��ʼ���ļ���
	//			InitializeCriticalSection(&g_cs);//��ʼ���ٽ���
	//			m_xmlPath = xmlPath;
	//		}

	//		~XMLManager()
	//		{

	//			DeleteCriticalSection(&g_cs);//ɾ���ٽ���
	//			ReleaseSRWLockExclusive(&g_srwLock);//�ͷ�д�ļ���
	//			ReleaseSRWLockShared(&g_srwLock);//�ͷŶ��ļ���
	//			delete this;
	//		}

	//		void ReadLock()
	//		{
	//			//���������ȡ�ļ�
	//			AcquireSRWLockShared(&g_srwLock);//������ļ���
	//			EnterCriticalSection(&g_cs);//�����ٽ���
	//		}
	//		void ReadUnlock()
	//		{
	//			//���߽�����ȡ�ļ�
	//			ReleaseSRWLockShared(&g_srwLock);//�ͷŶ��ļ���
	//			LeaveCriticalSection(&g_cs);//�ͷ��ٽ���
	//		}
	//		void WriteLock()
	//		{
	//			AcquireSRWLockExclusive(&g_srwLock);//����д�ļ���
	//			EnterCriticalSection(&g_cs);//�����ٽ���
	//		}
	//		void WriteUnlock()
	//		{
	//			ReleaseSRWLockExclusive(&g_srwLock);//�ͷ�д�ļ���
	//			LeaveCriticalSection(&g_cs);//�ͷ��ٽ���
	//		}


	//		XMLDocument* LoadXMLFile(const char* xmlPath);





	//	private:

	//		CRITICAL_SECTION g_cs;
	//		SRWLOCK          g_srwLock;
	//		const char*		m_xmlPath;
	//};
	
	


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

