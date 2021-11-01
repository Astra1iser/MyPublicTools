#pragma once
#include <ScopeLock.h>
//#include <pthread/include/pthread.h>

#include <thread>
//#include <mutex>
#include <queue>
#include <vector>
//#include <condition_variable>
using namespace std;


//例子,在主函数中实例化并重写run()方法
//class Example : public Thread
//{
//public:
//	//重写Run;
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
//	//启动线程;
//	e.Start();
//
//	getchar();
//	return 0;
//}

//class Thread;
//typedef void (*WThreadFunction)(Thread* pobj);
//using WThreadFunction = void(*) (Thread* pobj);

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
    virtual ~Thread();
	void Start();		//运行;
	void Stop();		//停止;
	BOOL WaitExit();	//等待线程退出;
};





//以下是线程池的相关类

//// 定义任务结构体
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
//// 任务队列
//class TaskQueue
//{
//public:
//    TaskQueue();
//    ~TaskQueue();
//
//    // 添加任务
//    void addTask(Task& task);
//    void addTask(callback func, void* arg);
//
//    // 取出一个任务
//    Task takeTask();
//
//    // 获取当前队列中任务个数
//    inline int taskNumber()
//    {
//        return m_queue.size();
//    }
//
//private:
//    pthread_mutex_t m_mutex;    // 互斥锁
//    std::queue<Task> m_queue;   // 任务队列
//};
//
//
////定义线程池
//class ThreadPool
//{
//public:
//    ThreadPool(int min, int max);
//    ~ThreadPool();
//
//    // 添加任务
//    void addTask(Task task);
//    void addTask(callback f, void* a);
//    // 获取忙线程的个数
//    int getBusyNumber();
//    // 获取活着的线程个数
//    int getAliveNumber();
//
//private:
//    // 工作的线程的任务函数
//    static void* worker(void* arg);
//    // 管理者线程的任务函数
//    static void* manager(void* arg);
//    //线程退出函数
//    void threadExit();
//
//private:
//    pthread_mutex_t m_lock;     //线程池中的锁
//    pthread_cond_t m_notEmpty;  //线程是否为空(条件变量)
//    pthread_t* m_threadIDs;     //工作线程ID
//    pthread_t m_managerID;      //管理者线程ID
//    TaskQueue* m_taskQ;         //线程池中的任务队列
//    int m_minNum;               //最小线程数量
//    int m_maxNum;               //最大线程数量
//    int m_busyNum;              //工作的线程个数
//    int m_aliveNum;             //存活的线程个数
//    int m_exitNum;              //已退出的线程个数
//    bool m_shutdown = false;    //是否销毁线程池
//};



//以下是线程池相关类(重新实现的)
/*
如何使用这个线程池呢?
下面是一个例子,线程池是一个抢占式重复使用的线程类
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

using callback = void(*)(void*);    //定义void fun(void* pl)函数指针callback
//typedef void(*callback)(void*);

//定义任务队列的结构体
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

//定义线程池类
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
    thread managerID;   //管理者线程ID
    vector<thread> threadIDs;   //存储工作线程的ID的顺序容器(LTS)
    int minNum;   //最小线程数
    int maxNum;   //最大线程数
    int busyNum;   //忙的线程数(工作中)
    int liveNum;    //存活的线程数(创建了但是未释放的线程)
    int needDestoryNum;    //要销毁的线程数
    int NUMBER;

    mutex mutexPool;    //整个线程池的锁
    /*
    当前线程调用wait()后将被阻塞并且函数会解锁互斥量，如果使用lock_guard则不能调用unlock函数，
    所以这里只能使用unique_lock对象，直到另外某个线程调用notify_one或者notify_all唤醒当前线程;
    一旦当前线程获得通知(notify)，wait()函数也是自动调用lock()，同理不能使用lock_guard对象。
    */
    condition_variable inWaitting;     //任务队列是否为空(条件变量,使用.wait(lock)方法时获得后边的锁,当条件变量被唤醒时,释放后边的锁)
    bool isShutdown;    //是否销毁线程池，销毁为1，不销毁为0
    static void manager(void* arg);   //管理者线程
    static void worker(void* arg);   //工作线程函数,把本类对象的指针传进去
};