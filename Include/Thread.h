#pragma once
//����,����������ʵ��������дrun()����
//class Example : public Thread
//{
//public:
//	//��дRun;
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
//	//�����߳�;
//	e.Start();
//
//	getchar();
//	return 0;
//}

class Thread;
typedef void (*WThreadFunction)(Thread* pobj);

//�߳���;
class Thread
{
private:
	HANDLE hThread;			//�߳̾��;
	DWORD nthreadID;		//�߳�ID;
	static DWORD __stdcall WThreadFunctionLinek(IN LPVOID Param); //��̬����
protected:
	bool bRun;
	virtual void Run() = 0;			//�̳в���д;
public:
	Thread();
	void Start();		//����;
	void Stop();		//ֹͣ;
	void WaitExit();	//�ȴ��߳��˳�;
};
