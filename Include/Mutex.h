//使用方法：
//加锁:
//CMutexSingle::GetInstance().Lock();
//解锁:
//CMutexSingle::GetInstance().UnLock();

#pragma once
#include "stdafx.h"

class CMutexSingle
{
public:
	static CMutexSingle& GetInstance();

public:
	BOOL Lock(CString MutexName);
	void UnLock();

	void Init(CString MutexName);
	void Uninit();

private:
	CMutexSingle();
	virtual ~CMutexSingle();

	//CMutexSingle(const CMutexSingle& );
	//CMutexSingle & operator= (const CMutexSingle &);

private:
	HANDLE m_hLockShareFileMap;//保护共享区数据
};