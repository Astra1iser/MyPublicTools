//Base������
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <tuple>
 

#include <windows.h>
#include <assert.h>
#include <atlconv.h>
#include <atlstr.h>


#include <ScopeLock.h>//���������ٽ�������װ��
#include <PathManager.h>//·����غ���
#include <RegeditManager.h>//ע��������غ���
#include <MyOperFile.h>//�ļ���д���ļ���Ϣ��غ���
#include <Thread.h>//�����߳���
#include <tinyxml2.h>

using namespace std;
using namespace PathManager;
using namespace RegeditManager;


#ifndef _YCMBASE_H
#define _YCMBASE_H


namespace Base
{
	//�жϵ�ǰ��¼���˻�����ͨ�û����ǹ���Ա ����ֵ:1����Ա 0�ǹ���Ա
	BOOL IsAdmin();


	//һ������������̵ķ���,����1:·�� ����2:�������� ����3:�Ƿ����ԱȨ������ ����4:�Ƿ������߳�
	BOOL StartPrograme(CString Path, CString Parameters = L"", BOOL IsAdmin =FALSE, BOOL IsWaitForSingle = TRUE);

	
	//��������Ԫ�ظ���,����Ϊ�������������
	template <class Arr>
	int getArrayLen(Arr& array)
	{
		return (sizeof(array) / sizeof(array[0]));
	}


	//������Ԫ�صĶ�������(��С����),����Ϊ�������������,���������ԭ����˳��ı�,��ʱû�����β�������ָ�������
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


	//���ֲ���,��һ������Ϊ��Ҫ���ҵ�ֵ,�ڶ�������Ϊ���������,���������������Ҫ���ҵ�ֵ�Ĵ�С��������ĵ�һ���±�
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


	//˫ָ��ԭ��ȥ��Ԫ��,����1Ϊ��������,����2Ϊ��Ҫȥ����Ԫ��ֵ,����ȥ���ĸ���,ȥ����Ԫ����������ĺ� "����ֵ����"��,�����ע��
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


	//��ȡIniPath��ini�ļ���Node�ڵ���Key��ֵ,������Ҫ��ֵ�����ó�Ա �˺���Ϊ���غ���,ֻ��ʶ��int��CString���͵�����
	void GetIniValue(int& Source, CString Node, CString Key, CString IniPath);
	//��ȡIniPath��ini�ļ���Node�ڵ���Key��ֵ,������Ҫ��ֵ�����ó�Ա �˺���Ϊ���غ���,ֻ��ʶ��int��CString���͵�����
	void GetIniValue(CString& Source, CString Node, CString Key, CString IniPath);

	//xml����ɾ�Ĳ�İ���:
	// 1.����
	//�����ĵ�ָ��,һ��xml�ĵ�������ʱ���������ָ��,���ָ�����xml�ĵ���д�����
	//			TiXmlDocument* writeDoc = new TiXmlDocument; //xml�ĵ�ָ��
	// 
	//�ĵ���ʽ����ָ��,һ���ĵ���������һ����,��Ȼд����д���: 
	// ����1:�汾 
	// ����2:����(һ�㱣��UTF-8����)
	// ����3:�Ƿ����(һ�㱣��yes)
	//			TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "UTF-8", "yes"); 
	// 
	//�����ڵ�ָ��
	// ����:�ڵ���(�����Ǳ���)
	//			TiXmlElement* Element = new TiXmlElement("Info");//����һ���ڵ�Ԫ��
	// 
	//������ǰ�ڵ�ָ��Ľڵ�����,�ڵ�ָ���SetAttribute����,һ���ڵ�������ö���ڵ�����,���ö��SetAttribute��������,���ʹ�ýڵ�ָ��� Element->LinkEndChild(�ڵ�����ָ��)д��
	//����1:������
	//����2:����ֵ
	//			Element->SetAttribute("num", n); //����,ֵ
	//
	//�����ڵ�����ָ��,һ���ڵ��д��������,���ʹ�ýڵ�ָ��� Element->LinkEndChild(�ڵ�����ָ��)д��
	//����:�ı�����
	//TiXmlText* Text = new TiXmlText("88"); //�����ڵ�����ָ��,����Ϊ123
	//
	//2.�h��



}

#endif

