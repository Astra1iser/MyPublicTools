#ifndef _OS_CHECKER_H_
#define _OS_CHECKER_H_

#include "ScopeLock.h"

class COSChecker
{
public:
	
	typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
	typedef BOOL (WINAPI *LPFN_Wow64DisableWow64FsRedirection)(LPVOID); 
	typedef BOOL (WINAPI *LPFN_Wow64RevertWow64FsRedirection)(LPVOID); 

	static BOOL GetVersionStruct(OSVERSIONINFOEX& info);
	static BOOL GetCurrentVer(DWORD& dwMain,DWORD& dwMinor);
	static CString GetOSString();	//OS的string描述，如winxp、win7这样的描述

	static BOOL IsWow64();
	static BOOL IsWindowsXPLater();

	static BOOL DisableRedirect();		// 禁用重定向
	static BOOL RestoreRedirect();		// 恢复重定向

	static BOOL IsXp();
	static BOOL IsServerOS();

protected:
	
	static BOOL _Init();

	static int m_nWow64;		//-1:uninitialized,0:32bit,other 64bit
	static BOOL m_bInited;
	static OSVERSIONINFOEX m_sOSInfo;
	static MyMutex m_cs;	

	static LPFN_ISWOW64PROCESS m_fnIsWow64Process;
	static LPFN_Wow64DisableWow64FsRedirection m_fnWow64DisableWow64FsRedirection;
	static LPFN_Wow64RevertWow64FsRedirection m_fnWow64RevertWow64FsRedirection;
	static PVOID m_old_redirect;
};











#endif