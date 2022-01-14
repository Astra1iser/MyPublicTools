//Base公共库
#pragma once
#pragma warning(disable:4129)//屏蔽不可转义的警告
#pragma warning(disable:4996)
#pragma warning(disable:4267)
#pragma warning(disable:4311)
#pragma warning(disable:4302)
#pragma warning(disable:4018)//屏蔽符号不匹配警告
#pragma warning(disable:4834)//屏蔽函数返回值类型被忽略警告
#pragma warning(disable:4244)//屏蔽强制类型转换可能丢失数据的警告



#include <iostream>
#include <string>
#include <vector>
#include <tuple>
#include <regex>
#include <map>
#include <list>
#include <csignal>

//#include <afxwin.h> //MFC的头文件,一般不要添加
#include <atlstr.h>
#include <windows.h>
#include <windowsx.h>
#include <assert.h>
#include <atlconv.h>
#include <wininet.h>
#include <atlfile.h>
#include <comutil.h>
#include <Psapi.h>
#include <tlhelp32.h>

#include <ScopeLock.h>//互斥锁（临界区）封装类
#include <PathManager.h>//路径相关函数
#include <RegeditManager.h>//注册表操作相关函数
#include <MyOperFile.h>//文件读写和文件信息相关函数
#include <DiretoryOperation.h>//提供更复杂的文件操作
#include <Thread.h>//线程类/线程池
#include <JsonHelper.h>//json解析库
#include <Mutex.h>//系统全局锁 Mutex(可作为线程锁和文件锁)
#include <FIFO.h> //管道的实现
#include <SharedMemory.h>//共享内存类的实现
#include <IniManager.h>//ini解析类
#include <XmlManager.h>//XML解析类
#include <LogWriter.h>//日志输出类
#include <Socket.h>//Socket连接类
#include <HttpPost.h>//实现http的post操作
#include <httpdown.h>//实现简单的HTTP下载
#include <WinFirewallControler.h>//windows防火墙相关操作
#include <SvcHelper.h>//windows服务类
#include <SystemternlHelper.h>//一些系统进程的相关函数
#include <Singleton.h>//类的单实例模板
#include <MakeCab.h>//cab制作类
#include <zip.h>//文件压缩方法
#include <unzip.h>//文件解压缩方法
#include <Guard.h>//程序崩溃处理类
#include <strconv.h>//字符编码类型转换类
#include <TxtReader.h>//用于按行读写文件文本
#include <XORFileEncrypt.h>//文件加解密类
#include <md5.h>


using namespace std;
using namespace PathManager;
using namespace RegeditManager;
using namespace FIFO;
using namespace SystemternlHelper;


//这个宏是为了规避windows自带的XMLDocument类重定义
#define XMLDocument tinyxml2::XMLDocument

//这个宏是为了防止tinyxml2的写入方法在写入xml时中文乱码的问题 使用_s结尾的函数时写入的数据都是UTF-8编码的数据,解析时,请使用对应的编码解析,本库没有使用utf-8解析的方法,所有xml解析均为ANSI格式
#define SetText_UTF8(x) SetText(G2U(x))
#define NewElement_UTF8(x) NewElement(G2U(x))
#define SetAttribute_UTF8(x) SetAttribute(G2U(x))

#define WRITE_LOG(x,...)	g_logger.Write(x,__VA_ARGS__)
#define WCOUT(x,...)		Wcout(x,__VA_ARGS__)

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

17.
*/

namespace Base
{
	//1.判断当前登录的账户是普通用户还是管理员 返回值:1管理员 0非管理员
	BOOL IsAdmin();


	//2.返回当前系统语言
	CString GetLangaueSyncOS();


	/*
	一个快捷启动进程的方法
	参数1:进程全路径
	参数2:返回启动成功的进程句柄(失败则句柄不变)
	参数3:启动参数
	参数4:是否管理员权限启动
	参数5:是否阻塞线程,就是启动进程后调用此函数的进程进入wait状态,直到启动的进程的句柄被释放或改变
	这里有一个问题,就是能否返回句柄的问题,如果当前的进程权限小于启动进程的权限,则返回的句柄为空,只有当前作为父进程的权限大于子进程时才能返回子进程句柄
	*/
	BOOL StartPrograme(CString Path, _Out_opt_ HANDLE& ProgrameHandle, CString Parameters = L"", BOOL IsAdmin =FALSE, BOOL IsWaitForSingle = FALSE);


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
	void GetIniValue(CString& Source, LPCTSTR Node, LPCTSTR Key, LPCTSTR IniPath);

	void SetIniValue(LPCTSTR Node, LPCTSTR Key, LPCTSTR Value, LPCTSTR IniPath);



	string ReadFileCoding(CString FilePath);


	wstring UTF8ToUnicode(const char* strSrc);

	string UnicodeToAnsi(const WCHAR* strSrc);


	string UTF8ToAnsi(const char* strSrc);




	//10.文本编码 UTF-8到GB2312的转换
	char* U2G(const char* utf8);


	//11.文本编码 GB2312到UTF-8的转换
	char* G2U(const char* gb2312);




	void CharToWchar(const char* constCharString, TCHAR* outWchar);

	void WCharToChar(TCHAR* InWchar, char* OutStr);


	//12.将wstring转换为string
	string wstring2string(wstring wstr);


	//13.将string转换为wstring
	wstring string2wstring(string str);


	//14.将LPCTSTR转换为string
	string LPCTSTR2string(LPCTSTR lpctstr);

	//string转换为LPTSTR
	LPTSTR string2LPTSTR(string str);


	//返回错误信息和错误码
	string GetSysErrorMessage(_Out_opt_ int* pErrCode);

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


	//输出CString字符串,最后带个换行
	void Wcout(CString string, ...);


	// 这是一个返回值后置的例子
	template<class T, class Y>
	auto funcExample(T& t, Y& y) -> decltype(t + y)
	{
		return t + y;
	}


	//decltype在类内自动推导的例子
	template<class T>
	class classExample
	{
	private:
		decltype(T().begin()) m_it; //这里 T()的意思是传入的是一个T类型的对象,而最后decltype也会推导出 m_it是一个迭代器类型
	public:
		void print(T& t)
		{
			for (m_it = t.begin(); m_it != t.end(); m_it++)
				cout << *m_it << endl;
		}

	};

	//typedef int(*func) (double, string); //定义一个func的函数指针  函数的返回值是int 参数1类型是double参数2类型是string
	//using func1 = int(*) (double, string); //使用using定义函数指针 意义和上边那条一样

	//int mytest(double a, string b)
	//{
	//	return a;
	//}
	//对于上面的例子 我们可以使用 func f = mytest;  f(10.1,"L");来使用函数 


	
	//获取文件版本号
	CString GetFileVersion(LPCSTR filename);
	
	/*
	版本号比较
	参数1:左值
	参数2:右值
	参数3:比较符号
	参数4:结果
	返回值:执行是否成功
	*/
	BOOL CompareVersion(/*LARGE_INTEGER llLeft,*/ LPCTSTR lpLeftVersion, LPCTSTR lpRightVersion, CString& sOpt, BOOL& bResult);

	//文件夹是否可写
	BOOL IsDirectoryWritable(CString lpwszPath);


	//时间戳转换,系统时间转换时间戳(美国夏令时)
	//SYSTEMTIME 格式:年月周(0为周一6为周日)日时分秒毫秒
	void SystemTimeToTimet(SYSTEMTIME st, time_t* pt);

	//时间戳转换,时间戳转换为系统时间(美国夏令时)
	void TimetToSystemTime(time_t t, LPSYSTEMTIME pst);

	//查看对应pid的进程的用户名和完整性级别
	CString GetProcessUserNameAndIntegrity(DWORD dwPid, DWORD* pdwLevel);

	//查看对应pid的用户名
	CString WhoIsUser(DWORD dwPid);

	//将dos路径转换为windows路径
	//BOOL DosPathToNtPath(LPTSTR pszDosPath, LPTSTR pszNtPath);

	//获取对应pid的进程的全路径
	BOOL GetProcessFullPath(DWORD dwPID, TCHAR* pszFullPath);

	/*
	* 进程权限修改,包括uac等权限
	* 参数1:权限名 如:SE_DEBUG_NAME,详情参考 NT Defined Privileges (winnt.h)
	* 参数2:启用还是关闭
	*/
	int EnablePrivilege(LPCWSTR lpszPrivilegeName, BOOL bEnable);

	//office系列软件是否运行中
	BOOL IsOfficeProcRuning();

	//关闭所有office进程
	BOOL KillOfficeProc();

	//根据pid获取子进程pid
	vector<DWORD> GetProceeIDfromParentID(DWORD& dwParentProcessId);

	/*
	* 根据pid关闭某进程
	* 参数1:要关闭的进程的pid
	* 参数2:是否要关闭这个进程的子进程,默认为不关闭
	*/
	void TerminateProcessByPid(DWORD dwPid, BOOL andChildProcess = FALSE);

	/*
	* 根据进程名关闭某进程
	* 参数1:要关闭的进程的名称(带后缀)
	* 参数2:是否要关闭这个进程的子进程,默认为关闭
	*/
	BOOL TerminateProcessByName(CString ProcName, BOOL andChildProcess = TRUE);

	//判断字符串是否为数字,若是数字则转化这个数字并传出,否则传入的引用的初始值不变
	BOOL Str2Int(_In_ string str, _Out_ int& resNum);


	//查询当前环境中又几块桌面(桌面总数=主屏幕+虚拟桌面个数)
	int GetCurrScreenNmber();

	/*
	* 获取进程加载的模块名(里面也可以返回模块名, 模块路径, 模块地址, 具体使用时按需使用)
	*	//模块快照
		vector<tstring> patch;
		ProcessModule(GetCurrentProcessId(),patch);
		cout << "******************当前模块*****************" << endl;
		for (int i = 0; i < patch.size(); i++)
		{
			Wcout(patch[i].c_str());
		}
	*/
	typedef basic_string<TCHAR, char_traits<TCHAR>, allocator<TCHAR> > tstring;
	BOOL ProcessModule(DWORD pid, _Out_ vector<tstring>& vtTstring);

	/*
	* 根据一个已存在的进程的进程名来获取token以获取相同的权限来启动另一个进程
	* 参数1: 启动的进程的命令行
	* 参数2: 启动的进程的参数
	* 参数3: 源进程名(用来获取token)
	*/
	BOOL RunAppInAuthorityByProessName(LPCWSTR lpFile, LPCWSTR lpParam, LPCWSTR srcProcess);

	/*
	* 根据一个已存在的进程的进程id来获取token以获取相同的权限来启动另一个进程
	* 参数1: 启动的进程的命令行
	* 参数2: 启动的进程的参数
	* 参数3: 源进程PID(用来获取token)
	*/
	BOOL RunAppInAuthorityByProessID(LPCWSTR lpFile, LPCWSTR lpParam, DWORD srcPid);

	/*
	* 从一个有规律的字符串中分割出子字符串并存放至vector容器
	* 参数1:要分割的字符串
	* 参数2:要存放的容器的引用
	* 参数3:分隔符是什么
	*/
	void SplitString(CString strLine, std::vector<CString>& vec_rlt, const CString& strSep /* = T("|")*/);

	/*
	* 从一个有规律的字符串中分割出子字符串并存放至list容器
	* 参数1:要分割的字符串
	* 参数2:要存放的列表的引用
	* 参数3:分隔符是什么
	*/
	void SplitString(CString strLine, std::list<CString>& vec_rlt, const CString& strSep /* = T("|")*/);


	/*
	* 获取系统上次启动时间(时间戳)
	*/
	DWORD SomGetSystemRebootTime();

	/*
	* 获取当前系统时间(时间戳)
	*/
	DWORD SomGetCurrentTime();

}

#endif

