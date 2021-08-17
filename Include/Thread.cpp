//例子,在主函数中实例化并重写run()方法
//class Example : public Thread
//{
//public:
//	//重写Run;
//	void Run()
//	{
//		while (this->bRun)
//		{
//			printf("Hello thread1\n");
//			Sleep(1000);
//		}
//	}
//};
//
//int main()
//{
//	Example e;
//	//启动线程;
//	e.Start();
//
//	getchar();
//	return 0;
//}



#pragma once
#include "Thread.h"

Thread::Thread()
{
	bRun = false;
}
void Thread::Start()
{
	if (bRun == true)
		return;
	bRun = true;
	hThread = CreateThread(NULL, 0, Thread::WThreadFunctionLinek, (LPVOID)this, 0, &nthreadID);
	if (hThread == NULL)
	{
		return;
	}
}
void Thread::Stop()
{
	if (bRun == false)
		return;
	bRun = false;
	TerminateThread(hThread, EXIT_FAILURE);
	CloseHandle(hThread);
	hThread = NULL;
}

void Thread::WaitExit()
{
	if (bRun == false)
		return;
	if (hThread == NULL)
		return;
	WaitForSingleObject(hThread, INFINITE);
}

DWORD __stdcall Thread::WThreadFunctionLinek(IN LPVOID Param)
{
	Thread* pThread = ((Thread*)Param);
	pThread->Run();
	pThread->Stop();
	return 0;
}
