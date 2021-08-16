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

using namespace std;
using namespace PathManager;
using namespace RegeditManager;


#ifndef _YCMBASE_H
#define _YCMBASE_H
namespace Base
{
	//判断当前登录的账户是普通用户还是管理员 返回值:1管理员 0非管理员
	BOOL IsAdmin();


}

#endif

