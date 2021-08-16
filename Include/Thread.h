#pragma once


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
