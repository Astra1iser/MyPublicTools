// MyPublicTools.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "stdafx.h"
//MyMutex m_writeLock;
//MyMutex m_writeLock2;
//
//Semaphore sam(0);
int abcc = 0;
//
class Example1 : public Thread
{
public:
	//重写Run;
	
	void Run() override
	{
		//InstanceMutex MySystemMutex(L"mylock",TRUE);
		int a = 0;

		
		while (this->bRun)
		{
			//sam.signal();
			printf("Hello thread1\n");
			//Sleep(1000);
			a++;
			
			//m_writeLock.Lock();
			//MySystemMutex.CreateInstanceMutex();
			//int ddd = abcc;
			//ddd++;
			Sleep(30);
			//abcc=ddd;
			abcc++;
			cout << abcc << endl;
			//MySystemMutex.DeleteInstanceMutex();
			//m_writeLock.Unlock();
            cout << this_thread::get_id();
			if (a == 100)
			{
				
				break;
			}
			
		}
        return;
	}

};
//
//class Example2 : public Thread
//{
//public:
//	//重写Run;
//	void Run()
//	{
//		int a = 0;
//
//		sam.wait();
//		while (this->bRun)
//		{
//
//			printf("Hello thread2\n");
//			//Sleep(1000);
//			a++;
//			
//			//m_writeLock2.Lock();
//			//m_writeLock.Lock();
//			Sleep(30);
//			abcc++;
//			cout << abcc << endl;
//			//m_writeLock2.Unlock();
//			//m_writeLock.Unlock();
//			
//			if (a == 100)
//				break;
//		}
//	}
//};
//
//int main()
//{   //https://blog.csdn.net/qq_40945965/article/details/86831178
//    //https://www.cnblogs.com/happykoukou/p/6307257.html
//    //https://www.cnblogs.com/whwywzhj/p/8478628.html
//
//
//    //CHttpDownload abc;
//    //abc.DownloadToFile(L"https://tools.skylarqa.com/test/yuanchunming/csv数据处理.zip?download=true",L"C:\\Users\\yuanchunming01\\Desktop\\MyPublicTools\\123.html");
//
//	//Wcout(GetFileNameFromPath(L"C:\\Users\\yuanchunming01\\Desktop\\MyPublicTools\\新建文件夹\\999.txt.111", true));
//	CString abc;
//	GetIniValue(abc, L"XshellOptions", L"MouseRightButton", L"C:\\Users\\yuanchunming01\\Desktop\\MyPublicTools\\Xshell.ini");
//
//	//Example1 e1,e3,e4;
//	//Example2 e2;
//
//	//启动线程;
//	//e1.Start();
//	//e2.Start();
//	//e3.Start();
//	//e4.Start();
//	//e2.Start();
//	//getchar();
//	//cout << abcc << endl;
//	system("pause");
//
//
//	//InstanceMutex abc(L"mysuo");
//	//abc.CreateInstanceMutex();
//	//system("pause");
//	//abc.DeleteInstanceMutex();
//	//system("pause");
//
//
//}

class abc
{
public:
     virtual void taskFunc(void* arg)
    {
        int num = *(int*)arg;
        cout << "thread: " << "number=" << num << endl;
        Sleep(1000);
    }
};




class abd :public abc
{
public:
    void taskFunc(void* arg)
    {
        int num = *(int*)arg;
        cout << "thread: " << "number=" << num+1 << endl;
        Sleep(2000);
    }
};




void taskFunc2(void* arg)
{
    int num = *(int*)arg;

    Sleep(4000);
    cout << "thread: " << "number=" << num << endl;
}


int func(int x)
{
    int countx = 0;
    while (x)
    {
        countx++;
        x = x & (x - 1);
    }
    return countx;



        
}
//int main()
//{
//    /*int* num = new int(100);
//    abc* ddd = new abd();
//    ddd->taskFunc(num);*/
//
//    //abc* aaa = new abc();
//
//    ThreadPool pool(3);
//    int i;
//    for (i = 0; i < 30; ++i)
//    {
//        int* num = new int(i + 100);
//        pool.AddTask(taskFunc2, (void*)num);
//        Sleep(1000);
//    }
//
//
//    //int* num = new int(100);
//    //pool.AddTask(taskFunc2, (void*)num);
//
//
//    //while (true)
//    //{
//    //    //int i = 0;
//    //    int a = 0;
//    //    cin >> a;
//    //    if (a == 1)
//    //    {
//    //        
//    //        for (int i = 0; i < 5; i++)
//    //        {
//    //            int* num = new int(i);
//    //            pool->AddTask(taskFunc2, (void*)num);
//    //        }
//
//    //    }
//    //}
//
//
//    //for (; i < 200; ++i)
//    //{
//    //    Sleep(1);
//    //    int* num = new int(i + 100);
//    //    pool.AddTask(taskFunc2, (void*)num);
//    //}
//
//
//    //int abc = func(9999);
//
//
//
//
//    getchar();
//    return 0;
//}

//int main()
//{
//	char buff[256];
//
//	HANDLE h_Pipe = CreateFIFO(L"mypipe");
//
//	ConnectFIFO(h_Pipe);
//	
//
//	while (true)
//	{
//
//		if (ReadFIFO(h_Pipe, buff) == FALSE)
//			break;
//		else
//		{
//			char d[256] = "i am server ,hello client";
//			WriteFIFO(h_Pipe, d);
//			Sleep(1000);
//		}
//	}
//
//	CloseHandle(h_Pipe);										//关闭管道释放资源
//
//	system("pause");
//}




//#include<iostream>
//#include<windows.h>
//#include<tchar.h>
//using namespace std;



//int main(int argc, char const* argv[])
//{
    //SharedMemory* abc = new SharedMemory(L"ShipMem");
    //abc->CreateSharedMemory(sizeof(string),L"ServerWriteOver", L"ClientReadOver");
    //string i;
    ////do
    ////{
    //    cin >> i;

    //    SetEvent(abc->hClientReadOver);
    //    abc->SetSharedMemory(i);
    //   
    ////} while (true);

    //    string o;
    //    abc->OpenSharedMemory(L"ServerWriteOver", L"ClientReadOver");
    //    abc->GetSharedMemory(o);
    //    cout << o<<endl;

    


    //do {
    //    SetEvent(hClientReadOver);
    //    WaitForSingleObject(hServerWriteOver, INFINITE);
    //    i = *lp;
    //    cout << i << endl;
    //    ResetEvent(hServerWriteOver);
    //} while (1);


    //StartPrograme(L"C:\\Users\\yuanchunming01\\Desktop\\Procmon64.exe");


    //return 0;

//}



//#include "stdafx.h"
//
//using namespace std;

//CString GetFileVersion(LPCSTR filename)
//{
//    string asVer = "";
//    VS_FIXEDFILEINFO* pVsInfo;
//    unsigned int iFileInfoSize = sizeof(VS_FIXEDFILEINFO);
//    int iVerInfoSize = GetFileVersionInfoSizeA(filename, NULL);
//    if (iVerInfoSize != 0)
//    {
//        char* pBuf = NULL;
//
//        while (!pBuf)
//        {
//            pBuf = new char[iVerInfoSize];
//        }
//        if (GetFileVersionInfoA(filename, 0, iVerInfoSize, pBuf))
//        {
//            if (VerQueryValueA(pBuf, "\\", (void**)&pVsInfo, &iFileInfoSize))
//            {
//                //sprintf(pBuf, "%d.%d.%d.%d", HIWORD(pVsInfo->dwFileVersionMS), LOWORD(pVsInfo->dwFileVersionMS), HIWORD(pVsInfo->dwFileVersionLS), LOWORD(pVsInfo->dwFileVersionLS));
//                asVer = pBuf;
//            }
//        }
//        delete pBuf;
//    }
//    return CString(asVer.c_str());
//}







//int main(int argc, _TCHAR* argv[])
//{
//    //string strFilePath = "C:\\Users\\yuanchunming01\\Desktop\\Procmon.exe";
//    //CString version = GetFileVersion(strFilePath.c_str());
//
//    //Wcout(version);
//
//    //IniManager ini(L"C:\\Users\\yuanchunming01\\Desktop\\123.ini");
//
//    //cout << ini.GetString(L"abc", L"aaa", 0);
//    //Wcout(ini.GetString(L"abc", L"aaa"));
//    //ini.AppendString(L"abc", L"aaa", L"卧槽");
//    //cout << ini.IsSectionExist(L"abcc") << endl;
//    //ini.CopySection(L"abc", L"aaa", 0);
//    //Wcout(ini.GetString(L"abc", L"bbb", 0));
//    //Wcout(CString(ini.GetChar(L"abc", L"aaa", 0)));
//    //cout<<ini.WriteDouble(L"abc", L"aaa", 11.321);
//    //cout << ini.GetInt(L"abc", L"aaa", 0)<<endl;
//    //cout << ReadFileCoding(L"C:\\Users\\yuanchunming01\\Desktop\\123.ini");
//    //XmlManager xml("卧槽.xml");
//    //string abc;
//    //xml.GetXMLDeclaration(abc);
//    //XMLElement* aaa;
//    //xml.FindXMLNode(aaa,"lib1");
//
//    //XMLElement* bbb;
//    //xml.FindXMLNode(bbb, "lib2");
//    //string text;
//    ////xml.FindXMLNode(bbb, "item", { {"id","1"} }, bbb);
//
//    ////xml.GetXMLNodeText(text, bbb,"table");
//
//    //XMLElement* ccc;
//    //xml.FindXMLNode(ccc, "item", { {"id","1"} },bbb);
//    ////xml.FindXMLNode(ccc, "table", {},ccc);
//
//    ////xml.GetXMLNodeText(text, ccc);
//    //xml.SetXMLNodeAttribution({{"d","3"}}, ccc, "table", {});
//    //xml.SetXMLNodeAttribution({ {"e","3"} }, ccc, "table", {});
//    //xml.DeleteXMLNodeAttribution({ "d" }, ccc, "table", {});
//
//    //g_logger.Init(L"C:\\Users\\yuanchunming01\\Desktop\\rizhi.log");
//
//    while (true)
//    {
//        Sleep(1000);
//        WRITE_LOG(L"%s",L"我我我我我我我我");
//    }
//    
//
//
//
//    getchar(); 
//    system("pause");
//    return 0;
//}

MySocket aaaaaaa;
static int nLoop = 1;

class Example3 : public Thread
{
public:
	//重写Run;

	void Run() override
	{
		aaaaaaa.InitSender(7001, "172.24.83.25");
		//aaaaaaa.InitSender(7001, "10.41.4.49");
		char buff[MAX_BUF_LEN];
		memset(buff, 0, MAX_BUF_LEN);

		while (this->bRun)
		{
			//nLoop++;

			//sprintf(buff, "Message %d", nLoop);
			//aaaaaaa.Send(buff);
			aaaaaaa.Send(nLoop);
			Sleep(5000);
		}
		return;
	}

};

class Example4 : public Thread
{
public:
	//重写Run;

	void Run() override
	{
		aaaaaaa.InitReceiver(7001);
		int nLoop = 0;


		while (this->bRun)
		{
			cin >> nLoop;
		}
		return;
	}

};


//
//#include <sstream>
//#include "stdafx.h"
//



int isNum(string str)
{
	//stringstream sin(str);
	//double d;
	//char c;
	//int e;
	//if (!(sin >> d))
	//{
	//	/*解释：
	//		sin>>t表示把sin转换成double的变量（其实对于int和float型的都会接收），
	//		如果转换成功，则值为非0，如果转换不成功就返回为0
	//	*/
	//	cout << "请别输入字符!" << endl;
	//	return false;
	//}
	//if (sin >> c)
	//{
	//	/*解释：
	//	此部分用于检测错误输入中，数字加字符串的输入形式（例如：34.f），在上面的的部分（sin>>t）
	//	已经接收并转换了输入的数字部分，在stringstream中相应也会把那一部分给清除，
	//	此时接收的是.f这部分，所以条件成立，返回false
	//	  */
	//	cout << "请别输入字符!" << endl;
	//	return false;
	//}
	//sin >> e;
	//nLoop = e;
	//return true;
	try 
	{
		stoi(str);
	}
	catch (...)
	{
		cout << "输入了非数字!" << endl;
		return 1;
	}

	return stoi(str);


}


HANDLE SimpleShellExec(CString lpFileName, LPCWSTR lpParam, CString lpDirectory, WORD wShowWindow)
{
#ifndef SEE_MASK_FORCENOIDLIST
#define SEE_MASK_FORCENOIDLIST 0x1000
#endif

	SHELLEXECUTEINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cbSize = sizeof(si);
	si.lpFile = lpFileName;
	si.lpParameters = lpParam;
	si.lpDirectory = lpDirectory;
	si.nShow = wShowWindow;
	si.fMask = SEE_MASK_FLAG_NO_UI | SEE_MASK_NOCLOSEPROCESS | SEE_MASK_FORCENOIDLIST;

	ShellExecuteEx(&si);

	return si.hProcess;
}


int TestLogic()
{
	throw;
	return 0;
}

void MyQuit()
{
	cout << "lalalalalal" << endl;
	abort();
}

void fun2(int b);

void fun1( int b)
{
	int a = b;
	fun2(a);
}

void fun2(int b)
{
	int a = b;
	fun1(a);
}

void myquit()
{
	for (int i = 0; i < 100; i++)
	{
		cout << "i'm exit:" <<i<< endl;
		Sleep(500);
	}
	
}



void func()
{
	__try
	{
		//abort();
		int a;
		cin >> a;
		int b = 5 / a;
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		cout << "触发了";
		MessageBox(NULL, L"11", L"111", 0);
	}

}

typedef BOOL(*pFun)(LPCTSTR);



enum list
{
	a = 0,
	b,
	c,
	d
};



int _EnablePrivilege(LPCWSTR lpszPrivilegeName, BOOL bEnable)
{
	int nResult = FALSE;
	int nRetCode = FALSE;
	HANDLE hToken = NULL;
	TOKEN_PRIVILEGES tkp = { 0 };

	do
	{
		nRetCode = ::OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);
		if (!nRetCode)
			break;

		nRetCode = ::LookupPrivilegeValue(NULL, lpszPrivilegeName, &tkp.Privileges[0].Luid);
		if (!nRetCode)
			break;

		tkp.PrivilegeCount = 1;
		tkp.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;
		nRetCode = ::AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof(tkp), NULL, NULL);
		if (!nRetCode)
			break;

		nResult = TRUE;
	} while (FALSE);

	if (hToken != NULL)
	{
		CloseHandle(hToken);
	}

	return nResult;
}







int main(int argc, _TCHAR* argv[])
{

	//Wcout(WhoIsUser(getpid()));

	//cout << "C++异常"<<endl;

	//WORD wVersionRequested;
	//WSADATA wsaData;
	//int err;

	//wVersionRequested = MAKEWORD(2, 2);
	//err = WSAStartup(wVersionRequested, &wsaData);
	//if (err != 0)
	//{
	//	return -1;
	//}
	//if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
	//{
	//	WSACleanup();
	//	return -1;
	//}

	//SOCKET clientSock = socket(AF_INET, SOCK_DGRAM, 0);
	//if (INVALID_SOCKET == clientSock)
	//{
	//	err = WSAGetLastError();
	//	printf("socket error:%d\n", err);
	//	WSACleanup();
	//	return -1;
	//}






	////创建本地地址信息并绑定
	//SOCKADDR_IN sin;
	//sin.sin_family = AF_INET;
	//sin.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	//sin.sin_port = htons(7001);
	//if (bind(clientSock, (SOCKADDR*)&sin, sizeof(sin)) != 0)
	//{
	//	err = WSAGetLastError();
	//	printf("bind error:%d\n", err);
	//	closesocket(clientSock);
	//	WSACleanup();
	//	return -1;
	//}

	////SOCKADDR_IN clientAddr;
	////int addrLen = sizeof(clientAddr);
	//int addrLen = sizeof(SOCKADDR_IN);
	//int nSize;
	//char recvBuff[1024];
	//memset(recvBuff, 0, 1024);

	//while (1) {
	//	//nSize = recvfrom(clientSock, recvBuff, 1024, 0, (SOCKADDR*)&clientAddr, &addrLen);
	//	nSize = recvfrom(clientSock, recvBuff, 1024, 0, (SOCKADDR*)&sin, &addrLen);
	//	if (nSize == SOCKET_ERROR)
	//	{
	//		err = WSAGetLastError();
	//		printf("recv error:%d\n", err);
	//		break;
	//	}
	//	recvBuff[nSize] = '\0';
	//	printf("%s\n", recvBuff);

	//}

	//closesocket(clientSock);
	//WSACleanup();

	//Example3 ddd;
	//Example4 dddd;
	//ddd.Start();
	//dddd.Start();

	

	//HANDLE h_Pipe = WaitFIFO(L"mypipe");
	//while (h_Pipe)
	//{
	//	int a = 0;
	//	ReadFIFO(h_Pipe, a);
	//	cout << a << endl;
	//	break;
	//}

	//string dd;

	//while (true)
	//{
	//	cin >> dd;
	//	nLoop = isNum(dd);
	//}

	//Wcout(GetFileVersion("C:\\Users\\yuanchunming01\\Desktop\\BootLeakFixer.tpi"));

	//HANDLE FF = CreateFIFO("myfifo");
	//ConnectFIFO(FF);

	//int a = 0;
	//cin >> a;
	//WriteFIFO(FF, a);
	//cin >> a;
	//WriteFIFO(FF, a);
	//CloseFIFO(FF);

	//cin >> a;

	//FF = CreateFIFO("myfifo");
	//ConnectFIFO(FF);
	//WriteFIFO(FF, a);

	//cin >> a;


	//Example1* exa = new Example1();
	//exa = NULL;
	//exa->Run();
	

	//set_terminate(MyQuit);
	//TestLogic();

	//CString abc("111111应用程序错误了");

	//int a = abc.Find(L"应用程序错误");

	//WCHAR aa[] = L"应用程序错误";
	//
	//if (StrStrIW(aa, L"1程序")!=NULL)
	//{
	//	cout << "行"<<endl;
	//}
	//else
	//{
	//	cout << "不行"<<endl;
	//}


	//system("pause");

//abort();
	
//fun1(3);
	//set_terminate(myquit);
	////func();
	//char str[80] = "This is - www.runoob.com - website";
	//const char s[2] = "-";
	//char* token;

	///* 获取第一个子字符串 */
	//token = strtok(str, s);

	///* 继续获取其他的子字符串 */
	//while (token != NULL) {
	//	printf("%s\n", token);

	//	token = strtok(NULL, s);
	//}

	//string aa;
	//CString a;

	//HMODULE  hDll;
	//hDll = LoadLibrary((L"LeakDetect.dll"));
	//if (NULL == hDll)
	//{
	//	cout << "error1" << endl;
	//}
	//else
	//{
	//	
	//	pFun myfunc;
	//	myfunc = (pFun)GetProcAddress(hDll, "IsPatchInstalled");


	//	if (!myfunc)
	//	{
	//		FreeLibrary(hDll);
	//		cout << "error2" << endl;
	//	}
	//	else
	//	{
	//		while (TRUE)
	//		{
	//			cout << "请输入补丁号(带KB/kb/Kb/kB开头)按回车结束:";
	//			cin >> aa;
	//			a = string2LPTSTR(aa);
	//			if (myfunc(a))
	//			{
	//				cout << "查询结果为: 存在"<< endl;
	//			}
	//			else
	//			{
	//				cout << "查询结果为: 不存在" << endl;
	//			}
	//			cout << endl;	
	//		}
	//	}
	//}

//fun2(3);

	//throw("C++异常");
	//abort();
//#include "stdio.h"
//
//	FILE* fp;
//
//	fp = fopen("1111.txt", "w+");
//
//	fclose(fp);

	_EnablePrivilege(SE_DEBUG_NAME, TRUE);
//CHttpDownload httpp;


	system("pause");
}




//#include <windows.h>
//#include <Tlhelp32.h>
//#include <iostream>
//#include <vector>
//#include <string>
//using namespace std;
//typedef basic_string<TCHAR, char_traits<TCHAR>, allocator<TCHAR> > tstring;
//
//
//int ProcessModule(DWORD);
//int CheckProcessModule(DWORD pid);
//BOOL IsModuleValid(tstring szModuleName);
//int dwCount = 0;
//vector<tstring> patch;
//vector<tstring> checkpatch;
//
//int main(void)
//{
//
//
//	//进程创建完毕后的模块快照
//	ProcessModule(GetCurrentProcessId());
//	cout << "******************当前模块*****************" << endl;
//	for (int i = 0; i < patch.size(); i++)
//	{
//		cout << patch[i].c_str() << endl;
//	}
//	cout << endl << endl;
//
//	LoadLibrary(TEXT("user32.dll"));//加载user32.dll测试
//
//
//	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
//	SetConsoleTextAttribute(hStdout, FOREGROUND_GREEN | FOREGROUND_INTENSITY);
//
//	//检测是否存在不同的模块
//	CheckProcessModule(GetCurrentProcessId());
//	cout << "******************检测模块*****************" << endl;
//	for (int j = 0; j < checkpatch.size(); j++)
//	{
//		cout << checkpatch[j].c_str() << endl;;
//	}
//	cout << endl << endl;
//	if (patch.size() == checkpatch.size())
//	{
//		cout << "没有检测到注入模块" << endl;
//	}
//
//
//
//	system("pause");
//	return 0;
//}
//
//
////获取进程模块
//int ProcessModule(DWORD pid)
//{
//
//	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
//	if (hProcessSnap)
//	{
//		MODULEENTRY32 me32;
//		me32.dwSize = sizeof(MODULEENTRY32);
//		Module32First(hProcessSnap, &me32);//获取进程第一个模块信息
//		do
//		{
//			patch.push_back(me32.szModule);
//			//printf("模块路径:%s\n",me32.szExePath);
//			//printf("模块名:%s\n",me32.szModule);
//			//printf("模块基址:0x%08X\n",(DWORD)me32.modBaseAddr);
//
//		} while (Module32Next(hProcessSnap, &me32));
//		CloseHandle(hProcessSnap);
//		return 0;
//	}
//	else
//	{
//		cout << "获取进程快照失败" << endl;;
//		return 0;
//	}
//	return 0;
//}
//
//int CheckProcessModule(DWORD pid)
//{
//
//	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);
//	if (hProcessSnap)
//	{
//		MODULEENTRY32 me32;
//		me32.dwSize = sizeof(MODULEENTRY32);
//		Module32First(hProcessSnap, &me32);//获取进程第一个模块信息
//		do
//		{
//			checkpatch.push_back(me32.szModule);
//			if (!IsModuleValid(me32.szModule))
//			{
//				cout << "[可疑模块]:" << me32.szExePath << endl;
//				dwCount++;
//			}
//
//			//printf("模块路径:%s\n",me32.szExePath);
//			//printf("模块名:%s\n",me32.szModule);
//			//printf("模块基址:0x%08X\n",(DWORD)me32.modBaseAddr);
//
//		} while (Module32Next(hProcessSnap, &me32));
//		CloseHandle(hProcessSnap);
//		if (dwCount)
//		{
//			cout << "可疑模块数:" << dwCount << endl;
//		}
//		return 0;
//	}
//	else
//	{
//		cout << "获取进程快照失败" << endl;
//		return 0;
//	}
//	return 0;
//}
//
//
//BOOL IsModuleValid(tstring szModuleName)
//{
//	// 遍历起始状态的模块列表
//	for (int i = 0; i < patch.size(); i++)
//	{
//		if (patch[i] == szModuleName)
//			return TRUE;
//	}
//	return FALSE;
//
//}