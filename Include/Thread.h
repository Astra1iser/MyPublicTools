#pragma once
#include <ScopeLock.h>
//#include <pthread/include/pthread.h>

#include <thread>
//#include <mutex>
#include <queue>
#include <vector>
//#include <condition_variable>
using namespace std;


//����,����������ʵ��������дrun()����
//class Example : public Thread
//{
//public:
//	//��дRun;
//	void Run() override
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

//class Thread;
//typedef void (*WThreadFunction)(Thread* pobj);
//using WThreadFunction = void(*) (Thread* pobj);

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
    virtual ~Thread();
	void Start();		//����;
	void Stop();		//ֹͣ;
	BOOL WaitExit();	//�ȴ��߳��˳�;
};





//�������̳߳ص������

//// ��������ṹ��
//using callback = void(*)(void*);
//struct Task
//{
//    Task()
//    {
//        function = nullptr;
//        arg = nullptr;
//    }
//    Task(callback f, void* arg)
//    {
//        function = f;
//        this->arg = arg;
//    }
//    callback function;
//    void* arg;
//};
//
//// �������
//class TaskQueue
//{
//public:
//    TaskQueue();
//    ~TaskQueue();
//
//    // �������
//    void addTask(Task& task);
//    void addTask(callback func, void* arg);
//
//    // ȡ��һ������
//    Task takeTask();
//
//    // ��ȡ��ǰ�������������
//    inline int taskNumber()
//    {
//        return m_queue.size();
//    }
//
//private:
//    pthread_mutex_t m_mutex;    // ������
//    std::queue<Task> m_queue;   // �������
//};
//
//
////�����̳߳�
//class ThreadPool
//{
//public:
//    ThreadPool(int min, int max);
//    ~ThreadPool();
//
//    // �������
//    void addTask(Task task);
//    void addTask(callback f, void* a);
//    // ��ȡæ�̵߳ĸ���
//    int getBusyNumber();
//    // ��ȡ���ŵ��̸߳���
//    int getAliveNumber();
//
//private:
//    // �������̵߳�������
//    static void* worker(void* arg);
//    // �������̵߳�������
//    static void* manager(void* arg);
//    //�߳��˳�����
//    void threadExit();
//
//private:
//    pthread_mutex_t m_lock;     //�̳߳��е���
//    pthread_cond_t m_notEmpty;  //�߳��Ƿ�Ϊ��(��������)
//    pthread_t* m_threadIDs;     //�����߳�ID
//    pthread_t m_managerID;      //�������߳�ID
//    TaskQueue* m_taskQ;         //�̳߳��е��������
//    int m_minNum;               //��С�߳�����
//    int m_maxNum;               //����߳�����
//    int m_busyNum;              //�������̸߳���
//    int m_aliveNum;             //�����̸߳���
//    int m_exitNum;              //���˳����̸߳���
//    bool m_shutdown = false;    //�Ƿ������̳߳�
//};



//�������̳߳������(����ʵ�ֵ�)
/*
���ʹ������̳߳���?
������һ������,�̳߳���һ����ռʽ�ظ�ʹ�õ��߳���
void taskFunc(void* arg)
{
    int num = *(int*)arg;
    cout << "thread: " << "number=" << num << endl;
    Sleep(1);
}

int main()
{
    ThreadPool pool(5, 10);
    int i;
    for (i = 0; i < 100; ++i)
    {
        int* num = new int(i + 100);
        pool.AddTask(taskFunc, (void*)num);
    }
    for (; i < 200; ++i)
    {
        Sleep(1);
        int* num = new int(i + 100);
        pool.AddTask(taskFunc, (void*)num);
    }

    getchar();
    return 0;
}

*/

using callback = void(*)(void*);    //����void fun(void* pl)����ָ��callback
//typedef void(*callback)(void*);

//����������еĽṹ��
struct Task
{
public:
	callback function;
	void* arg;
public:
	Task(callback f, void* arg)
	{
		function = f;
		this->arg = arg;
	}
};

//�����̳߳���
class ThreadPool
{
public:
    ThreadPool(/*int min,*/ int max);
    ~ThreadPool();
    void AddTask(callback f, void* arg);
    void AddTask(Task task);
    int GetBusyNum();
    int GetAliveNum();
    vector<BOOL> state;
    struct threadd
    {
        int id;
        ThreadPool* threadpool;
    };

private:
    queue<Task> taskQ;
    thread managerID;   //�������߳�ID
    vector<thread> threadIDs;   //�洢�����̵߳�ID��˳������(LTS)
    int minNum;   //��С�߳���
    int maxNum;   //����߳���
    int busyNum;   //æ���߳���(������)
    int liveNum;    //�����߳���(�����˵���δ�ͷŵ��߳�)
    int needDestoryNum;    //Ҫ���ٵ��߳���
    int NUMBER;

    mutex mutexPool;    //�����̳߳ص���
    /*
    ��ǰ�̵߳���wait()�󽫱��������Һ�������������������ʹ��lock_guard���ܵ���unlock������
    ��������ֻ��ʹ��unique_lock����ֱ������ĳ���̵߳���notify_one����notify_all���ѵ�ǰ�߳�;
    һ����ǰ�̻߳��֪ͨ(notify)��wait()����Ҳ���Զ�����lock()��ͬ����ʹ��lock_guard����
    */
    condition_variable inWaitting;     //��������Ƿ�Ϊ��(��������,ʹ��.wait(lock)����ʱ��ú�ߵ���,����������������ʱ,�ͷź�ߵ���)
    bool isShutdown;    //�Ƿ������̳߳أ�����Ϊ1��������Ϊ0
    static void manager(void* arg);   //�������߳�
    static void worker(void* arg);   //�����̺߳���,�ѱ�������ָ�봫��ȥ
};