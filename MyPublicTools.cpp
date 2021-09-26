// MyPublicTools.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "stdafx.h"
#include <iostream>

MyMutex m_writeLock;


int abcc = 0;

class Example1 : public Thread
{
public:
	//重写Run;
	
	void Run()
	{
		InstanceMutex MySystemMutex(L"mylock",TRUE);
		int a = 0;

		
		while (this->bRun)
		{
			
			printf("Hello thread1\n");
			//Sleep(1000);
			a++;
			//m_writeLock.Lock();
			MySystemMutex.CreateInstanceMutex();
			//int ddd = abcc;
			//ddd++;
			Sleep(30);
			//abcc=ddd;
			abcc++;
			cout << abcc << endl;
			MySystemMutex.DeleteInstanceMutex();
			//m_writeLock.Unlock();
			if (a == 100)
			break;
		}
	}
};

class Example2 : public Thread
{
public:
	//重写Run;
	void Run()
	{
		int a = 0;


		while (this->bRun)
		{

			printf("Hello thread2\n");
			//Sleep(1000);
			a++;
			//m_writeLock.Lock();
			abcc++;
			cout << abcc << endl;
			//m_writeLock.Unlock();
			if (a == 1000)
				break;
		}
	}
};

int main()
{   //https://blog.csdn.net/qq_40945965/article/details/86831178
    //https://www.cnblogs.com/happykoukou/p/6307257.html
    //https://www.cnblogs.com/whwywzhj/p/8478628.html


    //CHttpDownload abc;
    //abc.DownloadToFile(L"https://tools.skylarqa.com/test/yuanchunming/csv数据处理.zip?download=true",L"C:\\Users\\yuanchunming01\\Desktop\\MyPublicTools\\123.html");




	Example1 e1,e3,e4;
	//Example2 e2;

	//启动线程;
	e1.Start();
	//e2.Start();
	e3.Start();
	e4.Start();
	//e2.Start();
	//getchar();
	//cout << abcc << endl;
	system("pause");


	//InstanceMutex abc(L"mysuo");
	//abc.CreateInstanceMutex();
	//system("pause");
	//abc.DeleteInstanceMutex();
	//system("pause");


}