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
#include <Thread.h>


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

}

BOOL Thread::WaitExit()
{
	if (bRun == false)
		return TRUE;
	if (hThread == NULL)
		return TRUE;
	WaitForSingleObject(hThread, INFINITE);
}


DWORD __stdcall Thread::WThreadFunctionLinek(IN LPVOID Param)
{
	Thread* pThread = ((Thread*)Param);
	pThread->Run();
	pThread->Stop();
	return 0;
}

Thread::~Thread()
{
	Stop();
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	hThread = NULL;
}


//TaskQueue::TaskQueue()
//{
//    pthread_mutex_init(&m_mutex, NULL);
//}
//
//TaskQueue::~TaskQueue()
//{
//    pthread_mutex_destroy(&m_mutex);
//}
//
//void TaskQueue::addTask(Task& task)
//{
//    pthread_mutex_lock(&m_mutex);
//    m_queue.push(task); //将传入的任务放到队列尾部
//    pthread_mutex_unlock(&m_mutex);
//}
//
//void TaskQueue::addTask(callback func, void* arg)
//{
//    pthread_mutex_lock(&m_mutex);
//    Task task;
//    task.function = func;
//    task.arg = arg;
//    m_queue.push(task); //将创建的任务放到队列尾部
//    pthread_mutex_unlock(&m_mutex);
//}
//
//Task TaskQueue::takeTask()
//{
//    Task t;
//    pthread_mutex_lock(&m_mutex);
//    if (m_queue.size() > 0)
//    {
//        t = m_queue.front(); //将队列头的值取出来(只读)
//        m_queue.pop(); //将队列头的值弹出
//    }
//    pthread_mutex_unlock(&m_mutex);
//    return t;
//}
//
//
//
//
//ThreadPool::ThreadPool(int minNum, int maxNum)
//{
//    // 实例化任务队列
//    m_taskQ = new TaskQueue;
//    do {
//        // 初始化线程池
//        m_minNum = minNum;
//        m_maxNum = maxNum;
//        m_busyNum = 0;
//        m_aliveNum = minNum;
//
//        // 根据线程的最大上限给线程数组分配内存
//        m_threadIDs = new pthread_t[maxNum];
//        if (m_threadIDs == nullptr)
//        {
//            cout << "new thread_t[] fail...." << endl;;
//            break;
//        }
//        // 初始化
//        memset(m_threadIDs, 0, sizeof(pthread_t) * maxNum);
//        // 初始化互斥锁,条件变量
//        if (pthread_mutex_init(&m_lock, NULL) != 0 ||
//            pthread_cond_init(&m_notEmpty, NULL) != 0)
//        {
//            cout << "init mutex or condition fail..." << endl;
//            break;
//        }
//
//        /////////////////// 创建线程 //////////////////
//        // 根据最小线程个数, 创建线程
//        for (int i = 0; i < minNum; ++i)
//        {
//            pthread_create(&m_threadIDs[i], NULL, worker, this);
//            cout << "创建子线程, ID: " << m_threadIDs[i].x << endl;
//        }
//        // 创建管理者线程, 1个
//        pthread_create(&m_managerID, NULL, manager, this);
//    } while (0);
//
//}
//
//ThreadPool::~ThreadPool()
//{
//    m_shutdown = 1;
//    // 销毁管理者线程
//    pthread_join(m_managerID, NULL);
//    // 唤醒所有消费者线程
//    for (int i = 0; i < m_aliveNum; ++i)
//    {
//        pthread_cond_signal(&m_notEmpty);
//    }
//
//    if (m_taskQ) delete m_taskQ;
//    if (m_threadIDs) delete[]m_threadIDs;
//    pthread_mutex_destroy(&m_lock);
//    pthread_cond_destroy(&m_notEmpty);
//}
//
//void ThreadPool::addTask(Task task)
//{
//    if (m_shutdown)
//    {
//        return;
//    }
//    // 添加任务，不需要加锁，任务队列中有锁
//    m_taskQ->addTask(task);
//    // 唤醒工作的线程
//    pthread_cond_signal(&m_notEmpty);
//}
//
//void ThreadPool::addTask(callback f, void* a)
//{
//	if (m_shutdown)
//	{
//		return;
//	}
//    // 添加任务，不需要加锁，任务队列中有锁
//	m_taskQ->addTask(f, a);
//    // 唤醒工作的线程
//    pthread_cond_signal(&m_notEmpty);
//}
//
//
//int ThreadPool::getAliveNumber()
//{
//    int threadNum = 0;
//    pthread_mutex_lock(&m_lock);
//    threadNum = m_aliveNum;
//    pthread_mutex_unlock(&m_lock);
//    return threadNum;
//}
//
//int ThreadPool::getBusyNumber()
//{
//    int busyNum = 0;
//    pthread_mutex_lock(&m_lock);
//    busyNum = m_busyNum;
//    pthread_mutex_unlock(&m_lock);
//    return busyNum;
//}
//
//
//// 工作线程任务函数
//void* ThreadPool::worker(void* arg)
//{
//    ThreadPool* pool = static_cast<ThreadPool*>(arg); //C++的强制类型转换  其实这里的pool就是this
//    //可以替换为C语言中的 ThreadPool* pool = (ThreadPool*)arg;
//    // 一直不停的工作
//    while (true)
//    {
//        // 访问任务队列(共享资源)加锁
//        pthread_mutex_lock(&pool->m_lock);
//        // 判断任务队列是否为空, 如果为空工作线程阻塞
//        while (pool->m_taskQ->taskNumber() == 0 && !pool->m_shutdown)
//        {
//            cout << "thread " << pthread_self().x << " waiting..." << endl;
//            // 阻塞线程
//            pthread_cond_wait(&pool->m_notEmpty, &pool->m_lock);
//
//            // 解除阻塞之后, 判断是否要销毁线程
//            if (pool->m_exitNum > 0)
//            {
//                pool->m_exitNum--;
//                if (pool->m_aliveNum > pool->m_minNum)
//                {
//                    pool->m_aliveNum--;
//                    pthread_mutex_unlock(&pool->m_lock);
//                    pool->threadExit();
//                }
//            }
//        }
//        // 判断线程池是否被关闭了
//        if (pool->m_shutdown)
//        {
//            pthread_mutex_unlock(&pool->m_lock);
//            pool->threadExit();
//        }
//
//        // 从任务队列中取出一个任务
//        Task task = pool->m_taskQ->takeTask();
//        // 工作的线程+1
//        pool->m_busyNum++;
//        // 线程池解锁
//        pthread_mutex_unlock(&pool->m_lock);
//        // 执行任务
//        cout << "thread " << pthread_self().x << " start working..." << endl;
//        task.function(task.arg);
//        delete task.arg;
//        task.arg = nullptr;
//
//        // 任务处理结束
//        cout << "thread " << pthread_self().x << " end working...";
//        pthread_mutex_lock(&pool->m_lock);
//        pool->m_busyNum--;
//        pthread_mutex_unlock(&pool->m_lock);
//    }
//
//    return nullptr;
//}
//
//
//// 管理者线程任务函数
//void* ThreadPool::manager(void* arg)
//{
//    ThreadPool* pool = static_cast<ThreadPool*>(arg);
//    // 如果线程池没有关闭, 就一直检测
//    while (!pool->m_shutdown)
//    {
//        // 每隔3s检测一次
//        Sleep(3);
//        // 取出线程池中的任务数和线程数量
//        //  取出工作的线程池数量
//        pthread_mutex_lock(&pool->m_lock);
//        int queueSize = pool->m_taskQ->taskNumber();
//        int liveNum = pool->m_aliveNum;
//        int busyNum = pool->m_busyNum;
//        pthread_mutex_unlock(&pool->m_lock);
//
//        // 创建线程
//        const int NUMBER = 2;
//        // 当前任务个数>存活的线程数 && 存活的线程数<最大线程个数
//        if (queueSize > liveNum && liveNum < pool->m_maxNum)
//        {
//            // 线程池加锁
//            pthread_mutex_lock(&pool->m_lock);
//            int num = 0;
//            for (int i = 0; i < pool->m_maxNum && num < NUMBER && pool->m_aliveNum < pool->m_maxNum; ++i)
//            {
//                if (pool->m_threadIDs[i].x == 0)
//                {
//                    pthread_create(&pool->m_threadIDs[i], NULL, worker, pool);
//                    num++;
//                    pool->m_aliveNum++;
//                }
//            }
//            pthread_mutex_unlock(&pool->m_lock);
//        }
//
//        // 销毁多余的线程
//        // 忙线程*2 < 存活的线程数目 && 存活的线程数 > 最小线程数量
//        if (busyNum * 2 < liveNum && liveNum > pool->m_minNum)
//        {
//            pthread_mutex_lock(&pool->m_lock);
//            pool->m_exitNum = NUMBER;
//            pthread_mutex_unlock(&pool->m_lock);
//            for (int i = 0; i < NUMBER; ++i)
//            {
//                pthread_cond_signal(&pool->m_notEmpty);
//            }
//        }
//    }
//    return nullptr;
//}
//
//// 线程退出
//void ThreadPool::threadExit()
//{
//    pthread_t tid = pthread_self();
//    for (int i = 0; i < m_maxNum; ++i)
//    {
//        if (m_threadIDs[i].x == tid.x)
//        {
//            cout << "threadExit() function: thread "<< pthread_self().x << " exiting..." << endl;
//            m_threadIDs[i].x = 0;
//            break;
//        }
//    }
//    pthread_exit(NULL);
//}



ThreadPool::ThreadPool(/*int min,*/ int max)
{
	do
	{
		int min = 0;
		minNum = min;
		maxNum = max;
		busyNum = 0;
		liveNum = min;
		needDestoryNum = 0;

		NUMBER = 2;

		isShutdown = false;

		threadIDs.resize(max);
		state.resize(max);
		for (int i = 0; i < max; i++)
		{
			//state[i] = TRUE;
			state[i] = FALSE;
		}

		managerID = thread(manager, this);

		//for (int i = 0; i < min; ++i)
		//{
		//	threadIDs[i] = thread(worker, this);
		//}
		return;
	} while (0);
}

ThreadPool::~ThreadPool()
{
	isShutdown = true;

	//唤醒阻塞的消费者线程
	inWaitting.notify_all();
	for (int i = 0; i < maxNum; ++i)
	{
		if (threadIDs[i].joinable())
		{
			threadIDs[i].join();
		}
	}

	//阻塞回收管理者线程
	if (managerID.joinable())
	{
		cout << "managerthread" << "id: " << managerID.get_id() << " exit......" << endl;
		managerID.join();
	}
}

void ThreadPool::AddTask(Task t)
{
	unique_lock<mutex> lk(mutexPool);
	if (isShutdown)
	{
		return;
	}
	//添加任务
	taskQ.push(t);
	inWaitting.notify_all();
}

void ThreadPool::AddTask(callback f, void* a)
{
	unique_lock<mutex> lk(mutexPool);
	if (isShutdown)
	{
		return;
	}
	//添加任务
	taskQ.push(Task(f, a));
	inWaitting.notify_all();
}

int ThreadPool::GetBusyNum()
{
	mutexPool.lock();
	int busy = busyNum;
	mutexPool.unlock();
	return busy;
}

int ThreadPool::GetAliveNum()
{
	mutexPool.lock();
	int alive = liveNum;
	mutexPool.unlock();
	return alive;
}

void ThreadPool::worker(void* arg)
{
	//ThreadPool* pool = static_cast<ThreadPool*>(arg);
	threadd* pool = static_cast<threadd*>(arg);
	int id = pool->id;
	for (int i = 0; i < pool->threadpool->maxNum; i++)
		if (i == id)
			pool->threadpool->state[i] = TRUE;
	while (true)
	{
		unique_lock<mutex> lk(pool->threadpool->mutexPool);
		//当前任务队列是否为空
		while (pool->threadpool->taskQ.empty() && !pool->threadpool->isShutdown)
		{
			//阻塞工作线程
			pool->threadpool->inWaitting.wait(lk);
			if (pool->threadpool->needDestoryNum > 0)
			{
				cout << "thread" <<"["<<id<<"]" <<"id: " << this_thread::get_id() << " waitting......" << endl;
				pool->threadpool->needDestoryNum--;
				if (pool->threadpool->liveNum > pool->threadpool->minNum)
				{
					pool->threadpool->liveNum--;
					//cout << "threadid: " << pthread_self() << " exit......" << endl;
					
					for (int i = 0; i < pool->threadpool->maxNum; i++)
					{
						if (i == id)
						{
							pool->threadpool->state[i] = FALSE;
							break;
						}	
					}
					cout << "thread" << "[" << id << "]" << "id: " << this_thread::get_id() << " exit......" << endl;
					lk.unlock();
					return ;
				}
			}
		}

		//判断线程池是否关闭了
		if (pool->threadpool->isShutdown)
		{
			//cout << "threadid: " << pthread_self() << "exit......" << endl;
			for (int i = 0; i < pool->threadpool->maxNum; i++)
			{
				if (i == id)
				{
					pool->threadpool->state[i] = FALSE;
					break;
				}
			}
			cout << "thread" << "[" << id << "]" << "id: " << this_thread::get_id() << "exit......" << endl;
			return ;
		}

		//从任务队列中取出一个任务
		Task task = pool->threadpool->taskQ.front(); //将队列头的值取出来(以只读形式拷贝)
		pool->threadpool->taskQ.pop();	//将列头元素弹出来
		pool->threadpool->busyNum++;

		//解锁
		lk.unlock();

		//cout << "thread: " << pthread_self() << " start working..." << endl;
		cout << "thread: " <<"["<<id<<"]" <<"id:" << this_thread::get_id() << " start working..." << endl;
		task.function(task.arg);
		//(*task.function)(task.arg);
		free(task.arg);
		task.arg = nullptr;

		//cout << "thread: " << pthread_self() << " end working..." << endl;
		cout << "thread: " << "[" << id << "]" << "id:" << this_thread::get_id() << " end working..." << endl;
		lk.lock();
		pool->threadpool->busyNum--;
		lk.unlock();
	}
}

void ThreadPool::manager(void* arg)
{
	ThreadPool* pool = static_cast<ThreadPool*>(arg);
	int NUMBER = 2;
	while (!pool->isShutdown)
	{
		//每隔3秒检测一次
		Sleep(3000);

		//取出线程池中任务的数量和当前线程的数量
		unique_lock<mutex> lk(pool->mutexPool);
		int queuesize = pool->taskQ.size();
		int livenum = pool->liveNum;
		int busynum = pool->busyNum;
		lk.unlock();




		//添加线程
		//任务的个数>存活的线程个数 && 存活的线程数 < 最大线程数
		if (queuesize > livenum && livenum < pool->maxNum)
		{
			lk.lock();
			int count = 0;
			for (int i = 0; i < pool->maxNum; i++)
				if (FALSE == pool->state[i])
				{
					if (pool->threadIDs[i].joinable())
					{
						pool->threadIDs[i].join();
					}
				}
			
			for (int i = 0; i < pool->maxNum && /*count < pool->NUMBER &&*/ pool->liveNum < pool->maxNum; i++)
			{
				threadd pooll{ i,pool };

				//thread::id id = pool->threadIDs[i].get_id();

				//id = pool->threadIDs[i].get_id();

				if (pool->threadIDs[i].get_id() == thread::id())
				{
					pool->threadIDs[i] = thread(worker, &pooll);
					cout << "Create a new thread"<<"["<<i<<"]" << "id = " << pool->threadIDs[i].get_id() << endl;
					count++;
					pool->liveNum++;
				}
			}
			lk.unlock();
		}

		//销毁线程
		//忙的线程*2 < 存活的线程数 && 存活的线程数 >  最小的线程数
		if (busynum /** 2*/ < livenum && livenum >= pool->minNum)
		{
			lk.lock();
			//pool->needDestoryNum = pool->NUMBER;
			pool->needDestoryNum = pool->liveNum - pool->busyNum;
			if (livenum - pool->needDestoryNum < pool->minNum)
			{
				pool->needDestoryNum = pool->liveNum - pool->minNum;
			}
			lk.unlock();
			//让工作的线程自杀
			//for (int i = 0; i < pool->NUMBER; ++i) 
			for (int i = 0; i < pool->needDestoryNum; ++i)
			{
				pool->inWaitting.notify_one();
			} 
			//pool->isEmpty.notify_all();
		}
	}
}