//Base公共库
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
 

#include <windows.h>
#include <assert.h>
#include <atlconv.h>
#include <atlstr.h>


#include <ScopeLock.h>//互斥锁（临界区）封装类
#include <PathManager.h>//路径相关函数
#include <RegeditManager.h>//注册表操作相关函数
#include <MyOperFile.h>//文件读写和文件信息相关函数
#include <Thread.h>//简易线程类
#include <tinyxml2.h>

using namespace std;
using namespace PathManager;
using namespace RegeditManager;


#ifndef _YCMBASE_H
#define _YCMBASE_H


namespace Base
{
	//判断当前登录的账户是普通用户还是管理员 返回值:1管理员 0非管理员
	BOOL IsAdmin();


	//一个快捷启动进程的方法,参数1:路径 参数2:启动参数 参数3:是否管理员权限启动 参数4:是否阻塞线程
	BOOL StartPrograme(CString Path, CString Parameters = L"", BOOL IsAdmin =FALSE, BOOL IsWaitForSingle = TRUE);

	
	//获得数组的元素个数,参数为任意数组的引用
	template <class Arr>
	int getArrayLen(Arr& array)
	{
		return (sizeof(array) / sizeof(array[0]));
	}


	//数组中元素的二分排序(从小到大),参数为任意数组的引用,传入参数后原数组顺序改变,暂时没有做形参是数组指针的重载
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


	//二分查找,第一个参数为需要查找的值,第二个参数为数组的引用,返回这个数组中需要查找的值的从小到大排序的第一个下标
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


	//双指针原地去除元素,参数1为引用数组,参数2为需要去除的元素值,返回去除的个数,去除的元素排在数组的后 "返回值个数"个,详情见注释
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


	//获取IniPath的ini文件中Node节点下Key的值,传入需要赋值的引用成员 此函数为重载函数,只会识别int和CString类型的引用
	void GetIniValue(int& Source, CString Node, CString Key, CString IniPath);
	//获取IniPath的ini文件中Node节点下Key的值,传入需要赋值的引用成员 此函数为重载函数,只会识别int和CString类型的引用
	void GetIniValue(CString& Source, CString Node, CString Key, CString IniPath);

	//xml的增删改查的帮助:
	// 1.创建
	//创建文档指针,一个xml文档被创建时必须有这个指针,这个指针控制xml文档的写入操作
	//			TiXmlDocument* writeDoc = new TiXmlDocument; //xml文档指针
	// 
	//文档格式声明指针,一个文档必须声明一下子,不然写入会有错误: 
	// 参数1:版本 
	// 参数2:编码(一般保持UTF-8就行)
	// 参数3:是否独立(一般保持yes)
	//			TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "yes"); 
	// 
	//创建节点指针
	// 参数:节点名(可以是变量)
	//			TiXmlElement* Element = new TiXmlElement("Info");//设置一个节点元素
	// 
	//创建当前节点指针的节点属性,节点指针的SetAttribute方法,一个节点可以设置多个节点属性,调用多次SetAttribute方法即可,最后使用节点指针的 Element->LinkEndChild(节点属性指针)写入
	//参数1:属性名
	//参数2:属性值
	//			Element->SetAttribute("num", n); //属性,值
	//
	//创建节点内容指针,一个节点可写入多个内容,最后使用节点指针的 Element->LinkEndChild(节点属性指针)写入
	//参数:文本内容
	//TiXmlText* Text = new TiXmlText("88"); //创建节点内容指针,内容为123
	//
	//2.刪除



}

#endif

