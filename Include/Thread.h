#pragma once


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
