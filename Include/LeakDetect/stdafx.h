#pragma once

#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit
#define _CONVERSION_DONT_USE_THREAD_LOCALE
#define _WTL_NO_CSTRING
#define _WTL_NO_WTYPES

// Windows ͷ�ļ�:
#include <windows.h>

//ATLͷ�ļ�
#include <atlstr.h>
#include <vector>
#include <map>

#include <iostream>
#include <strsafe.h>
using namespace std;