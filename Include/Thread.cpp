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
//    m_queue.push(task); //�����������ŵ�����β��
//    pthread_mutex_unlock(&m_mutex);
//}
//
//void TaskQueue::addTask(callback func, void* arg)
//{
//    pthread_mutex_lock(&m_mutex);
//    Task task;
//    task.function = func;
//    task.arg = arg;
//    m_queue.push(task); //������������ŵ�����β��
//    pthread_mutex_unlock(&m_mutex);
//}
//
//Task TaskQueue::takeTask()
//{
//    Task t;
//    pthread_mutex_lock(&m_mutex);
//    if (m_queue.size() > 0)
//    {
//        t = m_queue.front(); //������ͷ��ֵȡ����(ֻ��)
//        m_queue.pop(); //������ͷ��ֵ����
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
//    // ʵ�����������
//    m_taskQ = new TaskQueue;
//    do {
//        // ��ʼ���̳߳�
//        m_minNum = minNum;
//        m_maxNum = maxNum;
//        m_busyNum = 0;
//        m_aliveNum = minNum;
//
//        // �����̵߳�������޸��߳���������ڴ�
//        m_threadIDs = new pthread_t[maxNum];
//        if (m_threadIDs == nullptr)
//        {
//            cout << "new thread_t[] fail...." << endl;;
//            break;
//        }
//        // ��ʼ��
//        memset(m_threadIDs, 0, sizeof(pthread_t) * maxNum);
//        // ��ʼ��������,��������
//        if (pthread_mutex_init(&m_lock, NULL) != 0 ||
//            pthread_cond_init(&m_notEmpty, NULL) != 0)
//        {
//            cout << "init mutex or condition fail..." << endl;
//            break;
//        }
//
//        /////////////////// �����߳� //////////////////
//        // ������С�̸߳���, �����߳�
//        for (int i = 0; i < minNum; ++i)
//        {
//            pthread_create(&m_threadIDs[i], NULL, worker, this);
//            cout << "�������߳�, ID: " << m_threadIDs[i].x << endl;
//        }
//        // �����������߳�, 1��
//        pthread_create(&m_managerID, NULL, manager, this);
//    } while (0);
//
//}
//
//ThreadPool::~ThreadPool()
//{
//    m_shutdown = 1;
//    // ���ٹ������߳�
//    pthread_join(m_managerID, NULL);
//    // ���������������߳�
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
//    // ������񣬲���Ҫ�������������������
//    m_taskQ->addTask(task);
//    // ���ѹ������߳�
//    pthread_cond_signal(&m_notEmpty);
//}
//
//void ThreadPool::addTask(callback f, void* a)
//{
//	if (m_shutdown)
//	{
//		return;
//	}
//    // ������񣬲���Ҫ�������������������
//	m_taskQ->addTask(f, a);
//    // ���ѹ������߳�
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
//// �����߳�������
//void* ThreadPool::worker(void* arg)
//{
//    ThreadPool* pool = static_cast<ThreadPool*>(arg); //C++��ǿ������ת��  ��ʵ�����pool����this
//    //�����滻ΪC�����е� ThreadPool* pool = (ThreadPool*)arg;
//    // һֱ��ͣ�Ĺ���
//    while (true)
//    {
//        // �����������(������Դ)����
//        pthread_mutex_lock(&pool->m_lock);
//        // �ж���������Ƿ�Ϊ��, ���Ϊ�չ����߳�����
//        while (pool->m_taskQ->taskNumber() == 0 && !pool->m_shutdown)
//        {
//            cout << "thread " << pthread_self().x << " waiting..." << endl;
//            // �����߳�
//            pthread_cond_wait(&pool->m_notEmpty, &pool->m_lock);
//
//            // �������֮��, �ж��Ƿ�Ҫ�����߳�
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
//        // �ж��̳߳��Ƿ񱻹ر���
//        if (pool->m_shutdown)
//        {
//            pthread_mutex_unlock(&pool->m_lock);
//            pool->threadExit();
//        }
//
//        // �����������ȡ��һ������
//        Task task = pool->m_taskQ->takeTask();
//        // �������߳�+1
//        pool->m_busyNum++;
//        // �̳߳ؽ���
//        pthread_mutex_unlock(&pool->m_lock);
//        // ִ������
//        cout << "thread " << pthread_self().x << " start working..." << endl;
//        task.function(task.arg);
//        delete task.arg;
//        task.arg = nullptr;
//
//        // ���������
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
//// �������߳�������
//void* ThreadPool::manager(void* arg)
//{
//    ThreadPool* pool = static_cast<ThreadPool*>(arg);
//    // ����̳߳�û�йر�, ��һֱ���
//    while (!pool->m_shutdown)
//    {
//        // ÿ��3s���һ��
//        Sleep(3);
//        // ȡ���̳߳��е����������߳�����
//        //  ȡ���������̳߳�����
//        pthread_mutex_lock(&pool->m_lock);
//        int queueSize = pool->m_taskQ->taskNumber();
//        int liveNum = pool->m_aliveNum;
//        int busyNum = pool->m_busyNum;
//        pthread_mutex_unlock(&pool->m_lock);
//
//        // �����߳�
//        const int NUMBER = 2;
//        // ��ǰ�������>�����߳��� && �����߳���<����̸߳���
//        if (queueSize > liveNum && liveNum < pool->m_maxNum)
//        {
//            // �̳߳ؼ���
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
//        // ���ٶ�����߳�
//        // æ�߳�*2 < �����߳���Ŀ && �����߳��� > ��С�߳�����
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
//// �߳��˳�
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

	//�����������������߳�
	inWaitting.notify_all();
	for (int i = 0; i < maxNum; ++i)
	{
		if (threadIDs[i].joinable())
		{
			threadIDs[i].join();
		}
	}

	//�������չ������߳�
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
	//�������
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
	//�������
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
		//��ǰ��������Ƿ�Ϊ��
		while (pool->threadpool->taskQ.empty() && !pool->threadpool->isShutdown)
		{
			//���������߳�
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

		//�ж��̳߳��Ƿ�ر���
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

		//�����������ȡ��һ������
		Task task = pool->threadpool->taskQ.front(); //������ͷ��ֵȡ����(��ֻ����ʽ����)
		pool->threadpool->taskQ.pop();	//����ͷԪ�ص�����
		pool->threadpool->busyNum++;

		//����
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
		//ÿ��3����һ��
		Sleep(3000);

		//ȡ���̳߳�������������͵�ǰ�̵߳�����
		unique_lock<mutex> lk(pool->mutexPool);
		int queuesize = pool->taskQ.size();
		int livenum = pool->liveNum;
		int busynum = pool->busyNum;
		lk.unlock();




		//����߳�
		//����ĸ���>�����̸߳��� && �����߳��� < ����߳���
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

		//�����߳�
		//æ���߳�*2 < �����߳��� && �����߳��� >  ��С���߳���
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
			//�ù������߳���ɱ
			//for (int i = 0; i < pool->NUMBER; ++i) 
			for (int i = 0; i < pool->needDestoryNum; ++i)
			{
				pool->inWaitting.notify_one();
			} 
			//pool->isEmpty.notify_all();
		}
	}
}