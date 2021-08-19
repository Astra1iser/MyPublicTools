#pragma once
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

class Thread;
typedef void (*WThreadFunction)(Thread* pobj);

//线程类;
class Thread
{
private:
	HANDLE hThread;			//线程句柄;
	DWORD nthreadID;		//线程ID;
	static DWORD __stdcall WThreadFunctionLinek(IN LPVOID Param); //静态方法
protected:
	bool bRun;
	virtual void Run() = 0;			//继承并重写;
public:
	Thread();
	void Start();		//运行;
	void Stop();		//停止;
	void WaitExit();	//等待线程退出;
};
