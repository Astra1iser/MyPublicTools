//Base公共库
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

#include <ScopeLock.h>//互斥锁（临界区）封装类
#include <PathManager.h>//路径相关函数
#include <RegeditManager.h>//注册表操作相关函数
#include <MyOperFile.h>//文件读写和文件信息相关函数
#include <Thread.h>//简易线程类
#include <tinyxml2.h>//XML操作一次封装

using namespace std;
using namespace PathManager;
using namespace RegeditManager;
using namespace tinyxml2;

//这个宏是为了规避windows自带的XMLDocument类重定义
#define XMLDocument tinyxml2::XMLDocument

//这个宏是为了防止tinyxml2的写入方法在写入xml时中文乱码的问题 使用_s结尾的函数时写入的数据都是UTF-8编码的数据,解析时,请使用对应的编码解析,本库没有使用utf-8解析的方法,所有xml解析均为ANSI格式
#define SetText_UTF8(x) SetText(G2U(x))
#define NewElement_UTF8(x) NewElement(G2U(x))
#define SetAttribute_UTF8(x) SetAttribute(G2U(x))


#ifndef _YCMBASE_H
#define _YCMBASE_H

//字符串用 LPCSTR           LPCWSTR


/*
公共库目录:
1.判断当前登录的账户是普通用户还是管理员																				BOOL IsAdmin()
2.当前系统语言																											LPCTSTR SetLangaueSyncOS()
3.一个快捷启动进程的方法																								BOOL StartPrograme(LPCTSTR Path, LPCTSTR Parameters = L"", BOOL IsAdmin =FALSE, BOOL IsWaitForSingle = TRUE)
4.获得数组的元素个数																									int getArrayLen(Arr& array)
5.数组中元素的二分排序																									void BinaryInsertSort
6.二分查找(或其他查找算法 正在施工中)
7.双指针原地去除数组中的元素																							int DoublePointerRemoveValue(List& list, Value value)
8.获取IniPath的ini文件中Node节点下Key的值																				void GetIniValue(int& Source, LPCTSTR  Node, LPCTSTR Key, LPCTSTR IniPath)
																														void GetIniValue(LPCTSTR& Source, LPCTSTR Node, LPCTSTR Key, LPCTSTR IniPath)
9.更新IniPath的ini文件中Node节点下Key的值(施工中)																				
10.文本编码 UTF-8到GB2312的转换																							char* U2G(const char* utf8)
11.文本编码 GB2312到UTF-8的转换																							char* G2U(const char* gb2312)
12.将wstring转换为string
13.将string转换为wstring
14.将LPCTSTR转换为string																								string LPCTSTR2string(LPCTSTR lpctstr)
15.将俩个LPCTSRT拼接																									LPCTSTR CombineLPCTSRT (T1 lpctstr1, T2 lpctstr2)
16.一个简易的ftp下载方法																								BOOL EasyDownLoadFile(LPCTSTR lpcszURL, LPCTSTR localFilePath)
17.创建一个且只有根节点的xml文件																						XMLDocument* CreateEmptyXMLFile(const char* xmlPath, const char* rootNodeName)
18.载入xml文件																											XMLDocument* LoadXMLFile(const char* xmlPath)
19.保存xml内容																											BOOL SaveXMLFile(XMLDocument* doc, const char* xmlPath)
20.读取XML声明																											BOOL GetXMLDeclaration(XMLDocument* doc, string& strDecl)
21.寻找XML中的某个节点																									BOOL FindXMLNode(XMLElement* pRoot, const string nodeName, XMLElement*& pNode, const char* Attribute = NULL, const char* AttributeValue = NULL)
																														BOOL FindXMLNode(XMLElement* pRoot, const string nodeName, XMLElement*& pNode, AttributeList& attributelist = NULL, AttributeValueList& attributevaluelist = NULL)
22.1根据"节点指针"或者"节点指针和节点名"或者"节点指针和节点的名字及其节点某个(某些)属性的值"，获取该节点文本			BOOL GetXMLNodeText(XMLElement* pRoot, const char*& text, const string nodeName = "", const char* Attribute = NULL, const char* AttributeValue = NULL)
																														BOOL GetXMLNodeText(XMLElement* pRoot, const char*& text, const string nodeName = "", AttributeList& attributelist = NULL, AttributeValueList& attributevaluelist = NULL)

23.
*/

namespace Base
{
	//1.判断当前登录的账户是普通用户还是管理员 返回值:1管理员 0非管理员
	BOOL IsAdmin();


	//2.返回当前系统语言
	LPCTSTR SetLangaueSyncOS();


	//3.一个快捷启动进程的方法,参数1:路径 参数2:启动参数 参数3:是否管理员权限启动 参数4:是否阻塞线程
	BOOL StartPrograme(LPCTSTR Path, LPCTSTR Parameters = L"", BOOL IsAdmin =FALSE, BOOL IsWaitForSingle = TRUE);


	//4.获得数组的元素个数,参数为任意数组的引用
	template <class Arr>
	int getArrayLen(Arr& array)
	{
		return (sizeof(array) / sizeof(array[0]));
	}


	//5.数组中元素的二分排序(从小到大),参数为任意数组的引用,传入参数后原数组顺序改变,暂时没有做形参是数组指针的重载
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


	//6.二分查找,第一个参数为需要查找的值,第二个参数为数组的引用,返回这个数组中需要查找的值的从小到大排序的第一个下标
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


	//7.双指针原地去除数组中的元素,参数1为引用数组,参数2为需要去除的元素值,返回去除的个数,去除的元素排在数组的后 "返回值个数"个,详情见注释
	//如    int a[] = {2,7,5,2,4,2};
	//		int aa = DoublePointerRemoveValue(a, 2);
	//执行后的数组a为{7,5,4,2,2} aa值为3
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


	//8.1获取IniPath的ini文件中Node节点下Key的值,传入需要赋值的引用成员 此函数为重载函数,只会识别int和CString类型的引用
	void GetIniValue(int& Source, LPCTSTR  Node, LPCTSTR Key, LPCTSTR IniPath);
	//8.2获取IniPath的ini文件中Node节点下Key的值,传入需要赋值的引用成员 此函数为重载函数,只会识别int和CString类型的引用
	void GetIniValue(LPCTSTR& Source, LPCTSTR Node, LPCTSTR Key, LPCTSTR IniPath);


	//10.文本编码 UTF-8到GB2312的转换
	char* U2G(const char* utf8);


	//11.文本编码 GB2312到UTF-8的转换
	char* G2U(const char* gb2312);


	//12.将wstring转换为string
	string wstring2string(wstring wstr);


	//13.将string转换为wstring
	wstring string2wstring(string str);


	//14.将LPCTSTR转换为string
	string LPCTSTR2string(LPCTSTR lpctstr);


	//15.将俩个LPCTSRT拼接
	//参数1:LPCTSRT变量1
	//参数2:LPCTSRT变量2
	template<class T1, class T2>
	LPCTSTR CombineLPCTSRT (T1 lpctstr1, T2 lpctstr2)
	{
		return CString(lpctstr1) + CString(lpctstr2);
	}


	//16.一个简易的ftp下载方法 注意请尽可能的携带下载链接的协议(http:\\|https:\\|ftp:\\),不然本方法会按照https,ftp,http的顺序尝试下载(这可能造成下载的产物不同)
	//参数1:下载的文件url
	//参数2:存放的本地路径和文件名
	BOOL EasyDownLoadFile(LPCTSTR lpcszURL, LPCTSTR localFilePath);


	//17.创建一个且只有根节点的xml文件,可自定义根节点名称,返回创建的xml的XMLDocument*指针,这个指针可以用来继续对创建的xml进行读写操作
	//参数1:要创建的xml的路径
	//参数2:根节点名称
	XMLDocument* CreateEmptyXMLFile(const char* xmlPath, const char* rootNodeName);


	//18.载入xml文件,返回xml的XMLDocument*指针,这个指针可以用来继续对创建的xml进行读写操作
	//参数:要加载的xml的路径
	XMLDocument* LoadXMLFile(const char* xmlPath);
	

	//19.保存xml内容,此方法不同于SaveFile()之处在于此处如果保存成功,则释放doc指针并返回TRUE,若保存失败,则返回FALSE但此时不释放doc指针
	BOOL SaveXMLFile(XMLDocument* doc, const char* xmlSavePath);


	//20.读取XML声明
	BOOL GetXMLDeclaration(XMLDocument* doc, string& strDecl);


	//21.1根据"节点的名字"或者"节点的名字及其节点某个属性的值"来寻找节点(这个函数有重载)
	//参数1 需要查询的xml中某个节点(仅会遍历这个节点和兄弟节点中的子节点)的指针(如果实在不确定,可以填xml根节点的指针,但是此时请填写参数4和参数5以便精确查找)
	//参数2 需要查询的节点名字
	//参数3 如果查询到了返回的节点的地址,没查到则原引用的地址不变
	//参数4 要查询的节点的某个具体属性  (此参数可不填)
	//参数5 要查询的节点的某个具体属性的值	(此参数可不填)
	BOOL FindXMLNode(XMLElement* pRoot, const string nodeName, XMLElement*& pNode, const char* Attribute = NULL, const char* AttributeValue = NULL);


	//21.2根据"节点的名字"或者"节点的名字及其节点某些属性的值"来寻找节点(此重载针对多属性的node进行精确定位,注意参数4和参数5有一一对应关系)(这个函数有重载)
	//参数1 需要查询的xml中某个节点(仅会遍历这个节点和兄弟节点中的子节点)的指针(如果实在不确定,可以填xml根节点的指针,但是此时请填写参数4和参数5以便精确查找)
	//参数2 需要查询的节点名字
	//参数3 如果查询到了返回的节点的地址,没查到则原引用的地址不变
	//参数4 要查询的节点的某些具体属性 例如:(const char* attributelist[] = { "class","user" };) (此参数可不填)
	//参数5 要查询的节点的某些具体属性的值 例如:(const char* attributevaluelist[] = { "15班","原椿茗" };) (此参数可不填)
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









































	//22.1根据"节点指针"或者"节点指针和节点名"或者"节点指针和节点的名字及其节点某个属性的值"，获取该节点文本(这个函数有重载)
	//参数1 需要查询的xml中某个节点(仅会遍历这个节点和兄弟节点中的子节点)的指针(如果实在不确定,可以填xml根节点的指针,但是此时请填写参数4和参数5以便精确查找)
	//参数2 如果查询到了返回的文字的const char* 指针,没查到则原引用的地址不变
	//参数3 需要查询的节点名字(如果为空则只查询参数1的节点的文本)
	//参数4 要查询的节点的某个具体属性 (此参数可不填)
	//参数5 要查询的节点的某个具体属性的值 (此参数可不填)
	BOOL GetXMLNodeText(XMLElement* pRoot, const char*& text, const string nodeName = "", const char* Attribute = NULL, const char* AttributeValue = NULL);


	//22.2根据"节点指针"或者"节点指针和节点名"或者"节点指针和节点的名字及其节点某些属性的值"，获取该节点文本(此重载针对多属性的node进行精确定位,注意参数4和参数5有一一对应关系)(这个函数有重载)
	//参数1 需要查询的xml中某个节点(仅会遍历这个节点和兄弟节点中的子节点)的指针(如果实在不确定,可以填xml根节点的指针,但是此时请填写参数4和参数5以便精确查找)
	//参数2 如果查询到了返回的文字的const char* 指针,没查到则原引用的地址不变
	//参数3 需要查询的节点名字(如果为空则只查询参数1的节点的文本)
	//参数4 要查询的节点的某些具体属性 例如:(const char* attributelist[] = { "class","user" };) (此参数可不填)
	//参数5 要查询的节点的某些具体属性的值 例如:(const char* attributevaluelist[] = { "15班","原椿茗" };) (此参数可不填)
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


	//23.1根据"节点指针"或者"节点指针和节点名"或者"节点指针和节点的名字及其节点某个属性的值"，获取节点属性(这个函数有重载)
	//参数1 需要查询的xml中某个节点(仅会遍历这个节点和兄弟节点中的子节点)的指针(如果实在不确定,可以填xml根节点的指针,但是此时请填写参数4和参数5以便精确查找)
	//参数2 如果查询到了返回属性的map<string, string>,没查到则原引用的内容不变
	//参数3 需要查询的节点名字(如果为空则只查询参数1的节点的属性)(请务必遵循一个xml中同名节点的属性名都相同的结构来构造xml,因为此方法只会返回查找到的第一个同名节点的属性名和值,如果你想获取某些同名节点的属性构成的话)
	//参数4 要查询的节点的某个具体属性 ((如果一个属性都不知道则此参数可不填))
	//参数5 要查询的节点的某个具体属性的值 (如果一个属性都不知道则此参数可不填)
	BOOL GetXMLNodeAttribute(XMLElement* pRoot, map<string, string>& mapAttribute, const string nodeName = "", const char* Attribute = NULL, const char* AttributeValue = NULL);
	

	//23.1根据"节点指针"或者"节点指针和节点名"或者"节点指针和节点的名字及其节点某些属性的值"，获取节点属性(这个函数有重载)
	//参数1 需要查询的xml中某个节点(仅会遍历这个节点和兄弟节点中的子节点)的指针(如果实在不确定,可以填xml根节点的指针,但是此时请填写参数4和参数5以便精确查找)
	//参数2 如果查询到了返回属性的map<string, string>,没查到则原引用的内容不变
	//参数3 需要查询的节点名字(如果为空则只查询参数1的节点的属性)(请务必遵循一个xml中同名节点的属性名都相同的结构来构造xml,因为此方法只会返回查找到的第一个同名节点的属性名和值,如果你想获取某些同名节点的属性构成的话)
	//参数4 要查询的节点的某些具体属性 例如:(const char* attributelist[] = { "class","user" };) (如果一个属性都不知道则此参数可不填)
	//参数5 要查询的节点的某些具体属性的值 例如:(const char* attributevaluelist[] = { "15班","原椿茗" };) (如果一个属性都不知道则此参数可不填)
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


	//24.1修改(包含更新或创建,不包含追加,若想追加请先获取后自己追加然后更新)节点文本
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

	//25.1 修改(包含更新或创建)节点属性
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

	//26.1在节点下插入子节点
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

	//27.1删除节点
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
			//pRoot->DeleteChild(NodeName); //删除指定节点
			pRoot->DeleteChildren();//删除节点的所有孩子节点


			if (SaveXMLFile(doc, xmlSavePath))
			{
				return TRUE;
			}
			return FALSE;
		}
		XMLElement* pNode = NULL;
		if (FindXMLNode(pRoot, nodeName, pNode, attributelist, attributevaluelist))
		{

			//pRoot->DeleteChild(NodeName); //删除指定节点
			pNode->DeleteChildren();//删除节点的所有孩子节点


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

	//这里本身是要做一个封装xml的互斥量锁  暂时先放着 后边再完善

	//class XMLManager
	//{
	//	public:

	//		XMLManager(const char* xmlPath)
	//		{
	//			InitializeSRWLock(&g_srwLock);//初始化文件锁
	//			InitializeCriticalSection(&g_cs);//初始化临界区
	//			m_xmlPath = xmlPath;
	//		}

	//		~XMLManager()
	//		{

	//			DeleteCriticalSection(&g_cs);//删除临界区
	//			ReleaseSRWLockExclusive(&g_srwLock);//释放写文件锁
	//			ReleaseSRWLockShared(&g_srwLock);//释放读文件锁
	//			delete this;
	//		}

	//		void ReadLock()
	//		{
	//			//读者申请读取文件
	//			AcquireSRWLockShared(&g_srwLock);//请求读文件锁
	//			EnterCriticalSection(&g_cs);//创建临界区
	//		}
	//		void ReadUnlock()
	//		{
	//			//读者结束读取文件
	//			ReleaseSRWLockShared(&g_srwLock);//释放读文件锁
	//			LeaveCriticalSection(&g_cs);//释放临界区
	//		}
	//		void WriteLock()
	//		{
	//			AcquireSRWLockExclusive(&g_srwLock);//请求写文件锁
	//			EnterCriticalSection(&g_cs);//创建临界区
	//		}
	//		void WriteUnlock()
	//		{
	//			ReleaseSRWLockExclusive(&g_srwLock);//释放写文件锁
	//			LeaveCriticalSection(&g_cs);//释放临界区
	//		}


	//		XMLDocument* LoadXMLFile(const char* xmlPath);





	//	private:

	//		CRITICAL_SECTION g_cs;
	//		SRWLOCK          g_srwLock;
	//		const char*		m_xmlPath;
	//};
	
	


	////这是一个内核锁
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

