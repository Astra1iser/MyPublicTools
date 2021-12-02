//ʹ�÷�����
//����:
//CMutexSingle::GetInstance().Lock();
//����:
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
//	HANDLE m_hLockShareFileMap;//��������������
//};




/*
����ϵͳ������ ��ͬ���ƵĻ����岻����, ������2ΪFALSEʱ, ������ͬ�Ļ������ʹ��.Lock()�᷵��FALSE,
������2ΪTRUEʱ, ������ͬ�Ļ������, ��ȴ���һ���������ͷ�(�˷�������Ϊ�߳�ͬ����ʹ��, ��Ч�ʽϵ�, һ����Ϊ��ͬ���̼���ļ���д��ʹ��)
�м�:mutex��Ϊ�ں˼���Ķ���,�������ڵ��߳��еĻ���ȴ�(���̻߳���һ��ᵼ������,���Ƕ���mutex��Ȼ����˼��,���ǵ��е�WaitForSingleObject,�����������0,����û���κ�Ч��),�������÷ǵȴ��ķ�ʽ���ж��Ƿ����ĳ������
ʹ�÷���:

	MutexLock MySystemMutex(L"MutexName",TRUE)//����ϵͳ������(ע��ֻ��������ͬ������Ϊͬһ����,����Ϊ��ͬ����,��ͬ����֮�以������)
	MySystemMutex.Lock();
	......��д�ļ� / �����ļ� / �߳�ͬ��......
	MySystemMutex.Unlock(); //�ͷŻ�����
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