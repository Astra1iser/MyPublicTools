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

using namespace std;
using namespace PathManager;
using namespace RegeditManager;


#ifndef _YCMBASE_H
#define _YCMBASE_H
namespace Base
{
	//�жϵ�ǰ��¼���˻�����ͨ�û����ǹ���Ա ����ֵ:1����Ա 0�ǹ���Ա
	BOOL IsAdmin();


}

#endif

