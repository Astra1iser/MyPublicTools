//使用方法：
//加锁:
//CMutexSingle::GetInstance().Lock();
//解锁:
//CMutexSingle::GetInstance().UnLock();

#pragma once

//class CMutexSingle
//{
//public:
//	static CMutexSingle& GetInstance();
//
//public:
//	BOOL Lock(CString MutexName);
//	void UnLock();
//
//	void Init(CString MutexName);
//	void Uninit();
//
//private:
//	CMutexSingle();
//	virtual ~CMutexSingle();
//
//	//CMutexSingle(const CMutexSingle& );
//	//CMutexSingle & operator= (const CMutexSingle &);
//
//private:
//	HANDLE m_hLockShareFileMap;//保护共享区数据
//};




/*
创建系统互斥体 相同名称的互斥体不共存, 当参数2为FALSE时, 创建相同的互斥体后使用.Lock()会返回FALSE,
当参数2为TRUE时, 创建相同的互斥体后, 会等待上一个互斥体释放(此方法可作为线程同步锁使用, 但效率较低, 一般作为不同进程间的文件读写锁使用)
切记:mutex作为内核级别的对象,不能用于单线程中的互斥等待(单线程互斥一般会导致死锁,但是对于mutex虽然不会思索,但是当中的WaitForSingleObject,则会立即返回0,导致没有任何效果),但可以用非等待的方式来判断是否处理过某个数据
使用方法:

	MutexLock MySystemMutex(L"MutexName",TRUE)//创建系统互斥体(注意只有名称相同的锁才为同一个锁,否则为不同的锁,不同的锁之间互不干扰)
	MySystemMutex.Lock();
	......读写文件 / 操作文件 / 线程同步......
	MySystemMutex.Unlock(); //释放互斥体
*/
class MutexLock
{
public:
	MutexLock(CString MutexName, BOOL NeedWait = FALSE) :MutexName(L"Global//" + MutexName), NeedWait(NeedWait), g_hMutex(NULL), MutexReleased(TRUE)
	{

	}

	BOOL Lock(BOOL Waitting = FALSE);
	
	BOOL Unlock(HANDLE hd = NULL);

	~MutexLock();

private:
	CString MutexName;
	BOOL NeedWait;
	HANDLE g_hMutex;
	HANDLE m_hMutex;
	BOOL MutexReleased;
};